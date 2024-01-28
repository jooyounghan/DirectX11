#include "ACamera.h"
#include "Shaders.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

ACamera::ACamera(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fPitchRadIn,
	const float& fYawRadIn,
	const float& fRollRadIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const float& fFovRadIn,
	const float& fNearZIn,
	const float& fFarZIn,
	const UINT& uiNumQualityLevelsIn,
	DXGI_FORMAT eRTVFormatIn, 
	DXGI_FORMAT eDSVFormatIn
)
	: ViewableRenderTarget(
		fXPos, fYPos, fZPos,
		fPitchRadIn, fYawRadIn, fRollRadIn,
		fFovRadIn, fNearZIn, fFarZIn,
		uiWidthIn, uiHeightIn,
		uiNumQualityLevelsIn,
		eRTVFormatIn
	),
	ViewableDepthStencil(
		fXPos, fYPos, fZPos,
		fPitchRadIn, fYawRadIn, fRollRadIn,
		fFovRadIn, fNearZIn, fFarZIn,
		uiWidthIn, uiHeightIn,
		uiNumQualityLevelsIn,
		eDSVFormatIn
	),
	Viewable(
		fXPos, fYPos, fZPos, 
		fPitchRadIn, fYawRadIn, fRollRadIn,
		(float)uiWidthIn, (float)uiHeightIn, 
		fFovRadIn, fNearZIn, fFarZIn
	),
	AFilter(256, 1, 1),
	SwapChainAccessable()
{
}

ACamera::~ACamera()
{

}

void ACamera::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	ViewableRenderTarget::Resize(uiWidthIn, uiHeightIn);
	ViewableDepthStencil::Resize(uiWidthIn, uiHeightIn);
	AFilter::Resize(uiWidthIn, uiHeightIn);

	// 백버퍼 및 Swap Chain 사이즈 변경
	if (p_back_buffer)
	{
		ResetBackBufferAddress();
		DirectXDevice::pSwapChain->ResizeBuffers(
			0, uiWidth, uiHeight,
			DXGI_FORMAT_UNKNOWN,
			DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
		);

		SetAsBackBufferAddress();
	}
}

void ACamera::SetAsBackBufferAddress()
{
	SwapChainAccessable::SetAsBackBufferAddress();

	if (p_back_buffer != nullptr)
	{ 
		D3D11_TEXTURE2D_DESC desc;
		p_back_buffer->GetDesc(&desc);
		
		if (desc.SampleDesc.Quality != RenderTarget::uiNumQualityLevels ||
			desc.Format != RenderTarget::eFormat
		)
		{
			AFilter::uiWidth = desc.Width;
			AFilter::uiHeight = desc.Height;
			AFilter::uiArraySize = 1;
			AFilter::uiNumQualityLevels = desc.SampleDesc.Quality;
			AFilter::uiBindFlag = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
			AFilter::uiCPUAccess = NULL;
			AFilter::uiMiscFlag = NULL;
			AFilter::eUsage = D3D11_USAGE_DEFAULT;
			AFilter::eFormat = desc.Format;

			AFilter::Resize(AFilter::uiWidth, AFilter::uiHeight);
		}
	}
}

void ACamera::Apply(ID3D11ShaderResourceView** ppInputSRV)
{
	if (p_back_buffer != nullptr)
	{
		D3D11_TEXTURE2D_DESC desc;
		p_back_buffer->GetDesc(&desc);

		if (desc.SampleDesc.Quality != RenderTarget::uiNumQualityLevels ||
			desc.Format != RenderTarget::eFormat
			)
		{
			Shaders& shaders = Shaders::GetInstance();

			ID3D11ComputeShader* pComputeShader = nullptr;

			// Case에 대한 구분 =============================================
			pComputeShader = shaders.GetComputeShader(Shaders::MS16ToSS8CS);
			// ==============================================================	

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
}