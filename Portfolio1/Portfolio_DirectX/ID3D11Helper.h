#pragma once
#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>

#include "DefineVar.h"
#include "Console.h"

#include <vector>

using Microsoft::WRL::ComPtr;

class ID3D11Helper
{
public:
	static void CreateDeviceAndContext(
		IN const UINT& uiWidthIn,
		IN const UINT& uiHeightIn,
		IN const DXGI_FORMAT& eFormatIn,
		IN const bool& bWindowed,
		IN HWND hOutputWindow,
		OUT ComPtr<ID3D11Device>& cpDevice,
		OUT ComPtr<ID3D11DeviceContext>& cpDeviceContext,
		OUT ComPtr<IDXGISwapChain>& cpSwapChain
	);

	static void CreateVSInputLayOut(
		IN ID3D11Device* pDevice,
		IN LPCWSTR pFileName,
		IN const std::vector<D3D11_INPUT_ELEMENT_DESC>& vInputElementDescs,
		OUT ID3D11VertexShader** ppVertexShader,
		OUT ID3D11InputLayout** ppInputLayout
	);
	
	template<typename T>
	static void CreateBuffer(
		IN ID3D11Device* pDevice,
		IN const std::vector<T>& vData,
		IN D3D11_USAGE eUsage,
		IN UINT /*D3D11_BIND_FLAG*/ uiBindFlag,
		IN UINT /*D3D11_CPU_ACCESS_FLAG*/ eCpuAccess,
		IN UINT /*D3D11_RESOURCE_MISC_FLAG*/ eMiscFlag,
		OUT ID3D11Buffer** ppBuffer
	);

	template<typename T>
	static void CreateBuffer(
		IN ID3D11Device* pDevice,
		IN const T& data,
		IN D3D11_USAGE eUsage,
		IN UINT /*D3D11_BIND_FLAG*/ uiBindFlag,
		IN UINT /*D3D11_CPU_ACCESS_FLAG*/ eCpuAccess,
		IN UINT /*D3D11_RESOURCE_MISC_FLAG*/ eMiscFlag,
		OUT ID3D11Buffer** ppBuffer
	);

	template<typename T>
	static void UpdateBuffer(
		IN ID3D11DeviceContext* pDeviceContext,
		IN const T& data,
		IN D3D11_MAP mapFlag,
		OUT ID3D11Resource* pResource
	);

	template<typename T>
	static void UpdateBuffer(
		IN ID3D11DeviceContext* pDeviceContext,
		IN const std::vector<T>& vData,
		IN D3D11_MAP mapFlag,
		OUT ID3D11Resource* pResource
	);

	static bool GetBackBuffer(
		IN IDXGISwapChain* pSwapChain,
		OUT ID3D11Texture2D** ppTexture2D
	);

	static void CreateVS(
		IN ID3D11Device* pDevice,
		IN LPCWSTR pFileName,
		OUT ID3D11VertexShader** ppVertexShader
	);

	static void CreatePS(
		IN ID3D11Device* pDevice,
		IN LPCWSTR pFileName,
		OUT ID3D11PixelShader** ppPixelShader
	);

	static void CreateGS(
		IN ID3D11Device* pDevice,
		IN LPCWSTR pFileName,
		OUT ID3D11GeometryShader** ppGeometryShader
	);

	static void CreateHS(
		IN ID3D11Device* pDevice,
		IN LPCWSTR pFileName,
		OUT ID3D11HullShader** ppHullShader
	);

	static void CreateDS(
		IN ID3D11Device* pDevice,
		IN LPCWSTR pFileName,
		OUT ID3D11DomainShader** ppDomainShader
	);

	static void CreateCS(
		IN ID3D11Device* pDevice,
		IN LPCWSTR pFileName,
		OUT ID3D11ComputeShader** ppComputeShader
	);

	static void CreateRenderTargetView(
		IN ID3D11Device* pDevice,
		IN ID3D11Resource* pResource,
		OUT ID3D11RenderTargetView** ppRenderTargetView
	);

	static void CreateShaderResoureView(
		IN ID3D11Device* pDevice,
		IN ID3D11Resource* pResource,
		OUT ID3D11ShaderResourceView** ppShaderResourceView,
		IN D3D11_SHADER_RESOURCE_VIEW_DESC* sSRVDesc = NULL
	);

	static void CreateUnorderedAccessView(
		IN ID3D11Device* pDevice,
		IN ID3D11Resource* pResource,
		OUT ID3D11UnorderedAccessView** ppUnorderedAccessView
	);

	static void CreateDepthStencilView(
		IN ID3D11Device* pDevice,
		IN	ID3D11Texture2D* pDepthStencilTexture2D,
		OUT ID3D11DepthStencilView** ppDepthStencilView,
		IN D3D11_DEPTH_STENCIL_VIEW_DESC* sDSVDesc = NULL
	);

	static void CreateRasterizerState(
		IN ID3D11Device* pDevice,
		IN D3D11_FILL_MODE eFillMode,
		IN D3D11_CULL_MODE eCullMode,
		IN BOOL				bMultiSampling,
		OUT ID3D11RasterizerState** ppRasterizerState
	);

	static void CreateDepthStencilState
	(
		IN ID3D11Device*						pDevice,
		IN BOOL									bDepthEnable,
		IN D3D11_COMPARISON_FUNC				eDepthFunc,
		IN BOOL									bStencilEnable,
		IN const D3D11_DEPTH_STENCILOP_DESC&	sFrontStencilOpDesc,
		IN const D3D11_DEPTH_STENCILOP_DESC&	sBackStencilOpDesc,
		OUT ID3D11DepthStencilState**			ppDepthStencilState
	);

	static void CreateSampler(
		IN D3D11_FILTER					eFilter,
		IN D3D11_TEXTURE_ADDRESS_MODE	eTextureAddressMode,
		IN FLOAT						pFloat4[4],
		IN ID3D11Device*				pDevice,
		OUT ID3D11SamplerState**		ppSamplerState
	);

	static void CreateBlendState(
		IN ID3D11Device* pDevice,
		IN D3D11_BLEND_DESC* pBlendDesc,
		OUT ID3D11BlendState** ppBlendState
	);

	static void CreateTexture2D(
		IN ID3D11Device*				pDevice,
		IN const UINT&					uiWidth,
		IN const UINT&					uiHeight,
		IN const UINT&					uiBindFlag,
		IN const UINT&					uiCPUAccess,
		IN const UINT&					uiMiscFlag,
		IN D3D11_USAGE					eUsage,
		IN DXGI_FORMAT					eFormat,
		IN uint8_t*						pImageRawData,
		OUT ID3D11Texture2D**			ppTexture2D
	);

	static void CreateTexture2D(
		IN ID3D11Device* pDevice,
		IN const UINT& uiWidth,
		IN const UINT& uiHeight,
		IN const UINT& uiArraySize,
		IN const UINT& uiNumQualityLevels,
		IN const UINT& uiBindFlag,
		IN const UINT& uiCPUAccess,
		IN const UINT& uiMiscFlag,
		IN D3D11_USAGE eUsage,
		IN DXGI_FORMAT eFormat,
		OUT ID3D11Texture2D** ppTexture2D
	);
};

template<typename T>
void ID3D11Helper::CreateBuffer(
	IN ID3D11Device* pDevice,
	IN const std::vector<T>& vData,
	IN D3D11_USAGE eUsage,
	IN UINT /*D3D11_BIND_FLAG*/ uiBindFlag,
	IN UINT /*D3D11_CPU_ACCESS_FLAG*/ eCpuAccess,
	IN UINT /*D3D11_RESOURCE_MISC_FLAG*/ eMiscFlag, 
	OUT ID3D11Buffer** ppBuffer
)
{
	D3D11_BUFFER_DESC sBufferDesc;
	AutoZeroMemory(sBufferDesc);
	sBufferDesc.ByteWidth = UINT(vData.size()) * sizeof(T);
	sBufferDesc.Usage = eUsage;
	sBufferDesc.BindFlags = uiBindFlag;
	sBufferDesc.CPUAccessFlags = eCpuAccess;
	sBufferDesc.MiscFlags = eMiscFlag;
	sBufferDesc.StructureByteStride = sizeof(T);

	D3D11_SUBRESOURCE_DATA sSubResourceData;
	AutoZeroMemory(sSubResourceData);
	sSubResourceData.pSysMem = vData.data();
	sSubResourceData.SysMemPitch = 0;
	sSubResourceData.SysMemSlicePitch = 0;

	HRESULT hResult = pDevice->CreateBuffer(&sBufferDesc, &sSubResourceData, ppBuffer);
	if (FAILED(hResult))
	{
		Console::AssertPrint("버퍼를 생성하는데 실패하였습니다.");
	}
}

template<typename T>
void ID3D11Helper::CreateBuffer(
	IN ID3D11Device* pDevice,
	IN const T& data,
	IN D3D11_USAGE eUsage,
	IN UINT uiBindFlag,
	IN UINT eCpuAccess,
	IN UINT eMiscFlag,
	OUT ID3D11Buffer** ppBuffer)
{
	D3D11_BUFFER_DESC sBufferDesc;
	AutoZeroMemory(sBufferDesc);
	sBufferDesc.ByteWidth = sizeof(T);
	sBufferDesc.Usage = eUsage;
	sBufferDesc.BindFlags = uiBindFlag;
	sBufferDesc.CPUAccessFlags = eCpuAccess;
	sBufferDesc.MiscFlags = eMiscFlag;
	sBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA sSubResourceData;
	AutoZeroMemory(sSubResourceData);
	sSubResourceData.pSysMem = &data;
	sSubResourceData.SysMemPitch = 0;
	sSubResourceData.SysMemSlicePitch = 0;

	HRESULT hResult = pDevice->CreateBuffer(&sBufferDesc, &sSubResourceData, ppBuffer);
	if (FAILED(hResult))
	{
		Console::AssertPrint("버퍼를 생성하는데 실패하였습니다.");
	}
}

template<typename T>
void ID3D11Helper::UpdateBuffer(
	IN ID3D11DeviceContext* pDeviceContext,
	IN const T& data,
	IN D3D11_MAP mapFlag,
	OUT ID3D11Resource* pResource
)
{
	D3D11_MAPPED_SUBRESOURCE ms;
	pDeviceContext->Map(pResource, NULL, mapFlag, NULL, &ms);
	memcpy(ms.pData, &data, sizeof(decltype(data)));
	pDeviceContext->Unmap(pResource, NULL);
}

template<typename T>
void ID3D11Helper::UpdateBuffer(
	IN ID3D11DeviceContext* pDeviceContext,
	IN const std::vector<T>& data,
	IN D3D11_MAP mapFlag,
	OUT ID3D11Resource* pResource
)
{
	D3D11_MAPPED_SUBRESOURCE ms;
	pDeviceContext->Map(pResource, NULL, mapFlag, NULL, &ms);
	memcpy(ms.pData, data.data(), data.size() * sizeof(T));
	pDeviceContext->Unmap(pResource, NULL);
}