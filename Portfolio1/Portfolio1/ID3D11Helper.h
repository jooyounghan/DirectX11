#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h> 
#include <vector>

#include "DefineVar.h"

using Microsoft::WRL::ComPtr;
using std::vector;

extern void Console(const char*);

class ID3D11Helper
{
public:
	static void CreateDeviceAndContext(
		IN const int& iWidth,
		IN const int& iHeight,
		IN bool bWindowed,
		IN HWND hOutputWindow,
		OUT IDXGISwapChain** ppSwapChain,
		OUT ID3D11Device** ppDevice,
		OUT ID3D11DeviceContext** ppDeviceContext);

	static void CreateVSInputLayOut(
		IN ID3D11Device* pDevice,
		IN LPCWSTR pFileName,
		IN const vector<D3D11_INPUT_ELEMENT_DESC>& vInputElementDescs,
		OUT ID3D11VertexShader** ppVertexShader,
		OUT ID3D11InputLayout** ppInputLayout);
	
	template<typename T>
	static void CreateBuffer(
		IN ID3D11Device* pDevice,
		IN const vector<T>& vData,
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

	static void GetBackBuffer(
		IN IDXGISwapChain* pSwapChain,
		OUT ID3D11Texture2D** ppTexture2D
	);

	static void CreatePS(
		IN ID3D11Device* pDevice,
		IN LPCWSTR pFileName,
		OUT ID3D11PixelShader** ppPixelShader
	);

	static void CreateRenderTargetView(
		IN ID3D11Device* pDevice,
		IN ID3D11Resource* pResource,
		OUT ID3D11RenderTargetView** ppRenderTargetView
	);

	static void CreateShaderResoureView(
		IN ID3D11Device* pDevice,
		IN ID3D11Resource* pResource,
		OUT ID3D11ShaderResourceView** ppShaderResourceView
	);

	static void CreateDepthStencilView(
		IN ID3D11Device* pDevice,
		IN ID3D11Resource* pResource,
		OUT ID3D11DepthStencilView** ppDepthStencilView
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

	static void SetViewPort(
		IN const float& fTopLeftX,
		IN const float& fTopLeftY,
		IN const float& fWidth,
		IN const float& fHeight,
		IN const float& fMinDepth,
		IN const float& fMaxDepth,
		IN ID3D11DeviceContext* pDeviceContext,
		OUT D3D11_VIEWPORT* pScreenViewPort
	);
};

template<typename T>
void ID3D11Helper::CreateBuffer(
	IN ID3D11Device* pDevice,
	IN const vector<T>& vData,
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
		Console("버퍼를 생성하는데 실패하였습니다.");
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
		Console("버퍼를 생성하는데 실패하였습니다.");
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