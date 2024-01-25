#include "ACamera.h"
#include "DirectXDevice.h"
#include "ID3D11Helper.h"
#include "Shaders.h"

ACamera::ACamera(
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
	: ViewableRenderTarget(
		fXPos, fYPos, fZPos,
		fFovRadIn, fNearZIn, fFarZIn,
		uiWidthIn, uiHeightIn,
		uiNumQualityLevelsIn,
		eRTVFormatIn, eDSVFormatIn
	),
	IFilter(256, 1, 1),
	ASwapChainAccessable(),
	IRectangle(
		uiWidthIn,
		uiHeightIn
	)
{
}

ACamera::~ACamera()
{

}

void ACamera::Resize(const UINT& uiWidthIn, const UINT& uiHeightIn)
{
	ViewableRenderTarget::Resize(uiWidthIn, uiHeightIn);

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
	ASwapChainAccessable::SetAsBackBufferAddress();

	if (p_back_buffer != nullptr)
	{ 
		D3D11_TEXTURE2D_DESC desc;
		p_back_buffer->GetDesc(&desc);
		
		if (desc.SampleDesc.Quality != ARenderTarget::uiNumQualityLevels ||
			desc.Format != ARenderTarget::eFormat
		)
		{
			IFilter::uiArraySize = 1;
			IFilter::uiNumQualityLevels = desc.SampleDesc.Quality;
			IFilter::uiBindFlag = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
			IFilter::uiCPUAccess = NULL;
			IFilter::uiMiscFlag = NULL;
			IFilter::eUsage = D3D11_USAGE_DEFAULT;
			IFilter::eFormat = desc.Format;

			CreateTexture2DAndSRV<IFilter>();

			ID3D11Helper::CreateUnorderedAccessView(
				DirectXDevice::pDevice,
				IFilter::cpTexture2D.Get(),
				IFilter::cpUAV.GetAddressOf()
			);
		}
	}
}

void ACamera::Apply(ID3D11ShaderResourceView** ppInputSRV)
{
	if (p_back_buffer != nullptr)
	{
		D3D11_TEXTURE2D_DESC desc;
		p_back_buffer->GetDesc(&desc);

		if (desc.SampleDesc.Quality != ARenderTarget::uiNumQualityLevels ||
			desc.Format != ARenderTarget::eFormat
			)
		{
			Shaders& shaders = Shaders::GetInstance();

			ID3D11ComputeShader* pComputeShader = nullptr;

			// Case에 대한 구분 =============================================
			pComputeShader = shaders.GetComputeShader(Shaders::MS16ToSS8ComputeShader);
			// ==============================================================	

			DirectXDevice::pDeviceContext->CSSetShader(pComputeShader, NULL, NULL);
			DirectXDevice::pDeviceContext->CSSetShaderResources(0, 1, ppInputSRV);
			DirectXDevice::pDeviceContext->CSSetUnorderedAccessViews(0, 1, IFilter::cpUAV.GetAddressOf(), nullptr);
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