#include "ACamera.h"
#include "Shaders.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"
#include "CameraManipulator.h"

#include <algorithm>

using namespace DirectX;

ACamera::ACamera(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fPitchDegIn,
	const float& fYawDegIn,
	const float& fRollDegIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const float& fFovDegIn,
	const float& fNearZIn,
	const float& fFarZIn,
	const UINT& uiNumQualityLevelsIn,
	DXGI_FORMAT eRTVFormatIn, 
	DXGI_FORMAT eDSVFormatIn
)
	: ViewableRenderTarget(
		fXPos, fYPos, fZPos,
		fPitchDegIn, fYawDegIn, fRollDegIn,
		fFovDegIn, fNearZIn, fFarZIn,
		uiWidthIn, uiHeightIn,
		uiNumQualityLevelsIn,
		eRTVFormatIn
	),
	ViewableDepthStencil(
		fXPos, fYPos, fZPos,
		fPitchDegIn, fYawDegIn, fRollDegIn,
		fFovDegIn, fNearZIn, fFarZIn,
		uiWidthIn, uiHeightIn,
		uiNumQualityLevelsIn,
		eDSVFormatIn
	),
	Viewable(
		fXPos, fYPos, fZPos, 
		fPitchDegIn, fYawDegIn, fRollDegIn,
		(float)uiWidthIn, (float)uiHeightIn, 
		fFovDegIn, fNearZIn, fFarZIn
	),
	AFilter(
		uiWidthIn, uiHeightIn, 1, 0,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS,
		NULL, NULL, D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		256, 1, 1
	), isLinkedWithBackBuffer(false)
{
}

ACamera::~ACamera()
{

}

void ACamera::ManageKeyBoardInput(const float& fDelay)
{
	if (pPressed[EKeyCode::F])
	{
		XMMATRIX xmRotationMat = GetTransformedMatrix();
		XMVECTOR xmvDirection = XMVector4Transform(xmvDefaultDirection, xmRotationMat);
		XMVECTOR xmvRight = XMVector4Transform(xmvDefaultRight, xmRotationMat);

		if (pPressed[EKeyCode::W]) xmvPosition += xmvDirection * 0.1f;
		if (pPressed[EKeyCode::S]) xmvPosition -= xmvDirection * 0.1f;
		if (pPressed[EKeyCode::D]) xmvPosition += xmvRight * 0.1f;
		if (pPressed[EKeyCode::A]) xmvPosition -= xmvRight * 0.1f;
	}
}

void ACamera::ManageMouseInput(const int& iMouseXIn, const int& iMouseYIn)
{
	if (pPressed[EKeyCode::F])
	{
		const float& fWidth = (float)uiWidth;
		const float& fHeight = (float)uiHeight;
		float iMouseXNDC = 2.f * (std::clamp((float)iMouseXIn, 0.f, fWidth) / fWidth) - 1.f;
		float iMouseYNDC = 2.f * (std::clamp((float)iMouseYIn, 0.f, fHeight) / fHeight) - 1.f;

		sAnglesDegree.fPitchDeg = 90.f * iMouseYNDC;
		sAnglesDegree.fYawDeg = 180.f * iMouseXNDC;
	}
}

void ACamera::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	ViewableRenderTarget::Resize(uiWidthIn, uiHeightIn);
	ViewableDepthStencil::Resize(uiWidthIn, uiHeightIn);
	AFilter::Resize(uiWidthIn, uiHeightIn);

	// 백버퍼 및 Swap Chain 사이즈 변경
	if (isLinkedWithBackBuffer)
	{
		DirectXDevice::ResetBackBuffer();

		DirectXDevice::pSwapChain->ResizeBuffers(
			0, uiWidth, uiHeight,
			DXGI_FORMAT_UNKNOWN,
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
		);

		DirectXDevice::SetBackBuffer();
	}
}

void ACamera::AcceptFilterList(CameraManipulator* pCameraManipulator) 
{
	pCameraManipulator->VisitFilterList(*this);
}

void ACamera::Apply(ID3D11ShaderResourceView** ppInputSRV)
{
	if (isLinkedWithBackBuffer)
	{
		D3D11_TEXTURE2D_DESC desc;
		DirectXDevice::pBackBuffer->GetDesc(&desc);

		const bool bIsMSToSS = desc.SampleDesc.Quality != RenderTarget::uiNumQualityLevels;
		const bool bIsFormatResolve = desc.Format != RenderTarget::eFormat;

		Shaders& shaders = Shaders::GetInstance();
		ID3D11ComputeShader* pComputeShader = nullptr;

		if (bIsMSToSS)
		{
			pComputeShader = shaders.GetComputeShader(Shaders::MS16ToSS8CS);
		}
		else if (bIsFormatResolve)
		{
			pComputeShader = shaders.GetComputeShader(Shaders::TypeResolveCS);
			DirectXDevice::pDeviceContext->CSSetSamplers(0, 1, DirectXDevice::ppClampSampler);
		}

		if (bIsMSToSS || bIsFormatResolve)
		{
			DirectXDevice::pDeviceContext->CSSetShader(pComputeShader, NULL, NULL);
			DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, ppInputSRV);
			DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, AFilter::cpUAV.GetAddressOf(), nullptr);
			DirectXDevice::pDeviceContext->Dispatch(
				uiWidth % uiThreadGroupCntX ? uiWidth / uiThreadGroupCntX + 1 : uiWidth / uiThreadGroupCntX,
				uiHeight % uiThreadGroupCntY ? uiHeight / uiThreadGroupCntY + 1 : uiHeight / uiThreadGroupCntY,
				uiThreadGroupCntZ);
			SetUAVBarrier();
		}
	}
}

void ACamera::SetUAVBarrier()
{
	ID3D11ShaderResourceView* pReleaseAndGetAddressOfSRV = nullptr;
	ID3D11UnorderedAccessView* pReleaseAndGetAddressOfUAV = nullptr;
	ID3D11SamplerState* pSampler = nullptr;

	DirectXDevice::pDeviceContext->CSSetShader(nullptr, NULL, NULL);
	DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, &pReleaseAndGetAddressOfSRV);
	DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, &pReleaseAndGetAddressOfUAV, nullptr);
	DirectXDevice::pDeviceContext->CSSetSamplers(0, 1, &pSampler);
}