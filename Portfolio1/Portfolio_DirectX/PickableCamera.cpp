#include "PickableCamera.h"
#include "ID3D11Helper.h"
#include "MathematicalHelper.h"
#include "DirectXDevice.h"

PickableCamera::PickableCamera(

	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const UINT& uiWidthIn, const UINT& uiHeightIn,
	const float& fFovRadIn,
	const float& fNearZIn,
	const float& fFarZIn,
	const UINT& uiNumQualityLevelsIn,
	DXGI_FORMAT eRTVFormatIn,
	DXGI_FORMAT eDSVFormatIn
)
	: AFilteredCamera(
		fXPos, fYPos, fZPos,
		uiWidthIn, uiHeightIn,
		fFovRadIn, fNearZIn, fFarZIn,
		uiNumQualityLevelsIn,
		eRTVFormatIn, eDSVFormatIn
	), IRectangle(uiWidthIn, uiHeightIn, 1, 0)
{
}

PickableCamera::~PickableCamera()
{
}

void PickableCamera::ClearRTV()
{
	DirectXDevice::pDeviceContext->ClearRenderTargetView(cpRTV.Get(), IRenderTarget::fClearColor);
}

void PickableCamera::ClearDSV()
{
	DirectXDevice::pDeviceContext->ClearDepthStencilView(cpDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 0.f, NULL);
}

void PickableCamera::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	uiWidth = uiWidthIn;
	uiHeight = uiHeightIn;

	cpDSV.Reset();
	IDepthStencil::cpTexture2D.Reset();

	cpSRV.Reset();
	cpRTV.Reset();
	IRenderTarget::cpTexture2D.Reset();

	ASwapChainAccessable::cpTexture2D.Reset();
	if (IsSwapChainAccesssed)
	{
		DirectXDevice::pSwapChain->ResizeBuffers(0, uiWidth, uiHeight, DXGI_FORMAT_UNKNOWN, 0);
		SetAsSwapChainBackBuffer();
	}

	ID3D11Helper::CreateTexture2D(
		DirectXDevice::pDevice,
		uiWidth, uiHeight,
		uiArraySize, uiNumQualityLevels,
		D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
		NULL, NULL, D3D11_USAGE_DEFAULT, IRenderTarget::eFormat, IRenderTarget::cpTexture2D.GetAddressOf()
	);
	ID3D11Helper::CreateRenderTargetView(DirectXDevice::pDevice, IRenderTarget::cpTexture2D.Get(), cpRTV.GetAddressOf());
	ID3D11Helper::CreateShaderResoureView(DirectXDevice::pDevice, IRenderTarget::cpTexture2D.Get(), cpSRV.GetAddressOf());


	ID3D11Helper::CreateTexture2D(
		DirectXDevice::pDevice,
		uiWidth, uiHeight,
		uiArraySize, uiNumQualityLevels,
		D3D11_BIND_DEPTH_STENCIL,
		NULL, NULL, D3D11_USAGE_DEFAULT, IDepthStencil::eFormat, IDepthStencil::cpTexture2D.GetAddressOf()
	);
	ID3D11Helper::CreateDepthStencilView(DirectXDevice::pDevice, IDepthStencil::cpTexture2D.Get(), cpDSV.GetAddressOf());
}

void PickableCamera::Update(const float& fDelta)
{
	UpdateViewProj();
}

DirectX::XMMATRIX PickableCamera::GetTranformMat()
{
	return MathematicalHelper::MakeAffineTransformation(
		1.f, 1.f, 1.f,
		sAngles.fPitch, sAngles.fYaw, sAngles.fRoll,
		xmvPosition.m128_f32[0], xmvPosition.m128_f32[1], xmvPosition.m128_f32[2]
	);
}
