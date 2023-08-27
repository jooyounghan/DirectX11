#include "ID3D11Helper.h"
#include "FileLoader.h"

using namespace std;

void ID3D11Helper::CreateDeviceAndContext(
	IN const UINT& iWidth,
	IN const UINT& iHeight,
	IN bool bWindowed,
	IN HWND hOutputWindow,
	OUT ComPtr<IDXGISwapChain>& cpSwapChain,
	OUT ComPtr<ID3D11Device>& cpDevice,
	OUT ComPtr<ID3D11DeviceContext>& cpDeviceContext
)
{
	const D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	const D3D_FEATURE_LEVEL pFeatureLevel[3] = { D3D_FEATURE_LEVEL_11_0 , D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_9_3 };
	D3D_FEATURE_LEVEL featureLevel;

	ComPtr<ID3D11Device> cpTempDevice;
	ComPtr<ID3D11DeviceContext> cpTempDeviceContext;

	HRESULT hResult = D3D11CreateDevice(
		NULL,
		driverType,
		NULL,
		createDeviceFlags,
		pFeatureLevel,
		ARRAYSIZE(pFeatureLevel),
		D3D11_SDK_VERSION,
		cpTempDevice.GetAddressOf(),
		&featureLevel,
		cpTempDeviceContext.GetAddressOf()
	);

	if (FAILED(hResult))
	{
		Console("Device와 Device Context를 생성하는데 실패하였습니다.");
		return;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
		Console("D3D가 11을 지원하지 않습니다.");
		return;
	}

	UINT numQualityLevels = 0;

	cpTempDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4,
		&numQualityLevels);


	hResult = cpTempDevice.As(&cpDevice);
	if (FAILED(hResult))
	{
		Console("임시 Device 객체 As가 실패하였습니다.");
		return;
	}

	hResult = cpTempDeviceContext.As(&cpDeviceContext);
	if (FAILED(hResult))
	{
		Console("임시 Device Context 객체 As가 실패하였습니다.");
		return;
	}

	DXGI_SWAP_CHAIN_DESC sSwapChainDesc;
	AutoZeroMemory(sSwapChainDesc);
	sSwapChainDesc.BufferDesc.Width = iWidth;
	sSwapChainDesc.BufferDesc.Height = iHeight;
	sSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	sSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	sSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	sSwapChainDesc.SampleDesc.Count = numQualityLevels > 0 ? 4 : 1;
	sSwapChainDesc.SampleDesc.Quality = numQualityLevels > 0 ? numQualityLevels - 1 : 0;

	sSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sSwapChainDesc.BufferCount = 2;
	sSwapChainDesc.OutputWindow = hOutputWindow;
	sSwapChainDesc.Windowed = bWindowed;

	/*
	Memo
	DXGI_SWAP_EFFECT_DISCARD의 경우,
	새로운 프레임을 화면에 보여주기 전에 현재 화면 버퍼를 폐기하고
	새로운 렌더링 결과를 새로운 화면 버퍼에 렌더링한다.
	이전 프레임의 화면 버퍼를 먼저 폐기하기에 약간의 지연이 발생할 수 있다.

	DXGI_SWAP_EFFECT_FLIP_DISCARD의 경우,
	백 버퍼와 프론트 버퍼(화면에 보여지는 버퍼)를 분리하여 사용하며,
	백 버퍼의 렌더링 작업이 완료되면 프론트 버퍼와 스왑하는 방식으로 동작한다.
	이로 인해 백 버퍼의 렌더링 작업과 프론트 버퍼의 화면 표시 작업이 비동기적으로 이루어지므로,
	더 부드러운 화면 전환을 제공한다.
	다중 샘플링이나 부분 Present가 불가능하다.

	멀티샘플링을 적용하기 위해서는 멀티샘플링을 적용한 백버퍼를 렌더링한 후,
	이를 비-멀티샘플링된 백 버퍼에 Resolve한다.
	이후에 Resolve된 백 버퍼를 프론트 버퍼로 스왑한다.
	*/
	sSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	hResult = D3D11CreateDeviceAndSwapChain(
		NULL,
		driverType,
		NULL,
		createDeviceFlags,
		pFeatureLevel,
		1,
		D3D11_SDK_VERSION,
		&sSwapChainDesc,
		cpSwapChain.GetAddressOf(),
		cpDevice.GetAddressOf(),
		&featureLevel,
		cpDeviceContext.GetAddressOf()
	);
	
	if (FAILED(hResult))
	{
		Console("Device와 Device Context를 생성하는데 실패하였습니다.");
	}
}

void ID3D11Helper::CreateVSInputLayOut(IN ID3D11Device* pDevice, IN LPCWSTR pFileName, IN const vector<D3D11_INPUT_ELEMENT_DESC>& vInputElementDescs, OUT ID3D11VertexShader** ppVertexShader, OUT ID3D11InputLayout** ppInputLayout)
{
	ComPtr<ID3DBlob> cpShaderBlob;
	ComPtr<ID3DBlob> cpErrorBlob;

	HRESULT hResult = D3DCompileFromFile(pFileName,
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		NULL,
		cpShaderBlob.GetAddressOf(),
		cpErrorBlob.GetAddressOf()
	);

	if (FAILED(hResult))
	{
		Console("Vertex Shader를 컴파일하는데 실패하였습니다.");
		return;
	}

	hResult = pDevice->CreateVertexShader(
		cpShaderBlob->GetBufferPointer(),
		cpShaderBlob->GetBufferSize(),
		NULL,
		ppVertexShader
	);
	if (FAILED(hResult))
	{
		Console("Vertex Shader를 생성하는데 실패하였습니다.");
		return;
	}

	hResult = pDevice->CreateInputLayout(vInputElementDescs.data(), (UINT)vInputElementDescs.size(), cpShaderBlob->GetBufferPointer(), cpShaderBlob->GetBufferSize(), ppInputLayout);
	if (FAILED(hResult))
	{
		Console("InputLayout을 생성하는데 실패하였습니다.");
		return;
	}
}

void ID3D11Helper::GetBackBuffer(IN IDXGISwapChain* pSwapChain, OUT ID3D11Texture2D** ppTexture2D)
{
	HRESULT hResult = pSwapChain->GetBuffer(0, IID_PPV_ARGS(ppTexture2D));
	if (FAILED(hResult))
	{
		Console("Back Buffer를 불러오는데 실패하였습니다.");
	}
}

void ID3D11Helper::CreatePS(IN ID3D11Device* pDevice, IN LPCWSTR pFileName, OUT ID3D11PixelShader** ppPixelShader)
{
	ComPtr<ID3DBlob> cpShaderBlob;
	ComPtr<ID3DBlob> cpErrorBlob;

	HRESULT hResult = D3DCompileFromFile(
		pFileName,
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"ps_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		NULL,
		cpShaderBlob.GetAddressOf(),
		cpErrorBlob.GetAddressOf()
	);

	if (FAILED(hResult))
	{
		Console("Pixel Shader를 컴파일하는데 실패하였습니다.");
		return;
	}

	hResult = pDevice->CreatePixelShader(
		cpShaderBlob->GetBufferPointer(),
		cpShaderBlob->GetBufferSize(),
		NULL,
		ppPixelShader
	);

	if (FAILED(hResult))
	{
		Console("Pixel Shader 생성하는데 실패하였습니다.");
		return;
	}
}

void ID3D11Helper::CreateRenderTargetView(IN ID3D11Device* pDevice, IN ID3D11Resource* pResource, OUT ID3D11RenderTargetView** ppRenderTargetView)
{
	HRESULT hResult = pDevice->CreateRenderTargetView(pResource, NULL, ppRenderTargetView);
	if (FAILED(hResult))
	{
		Console("Render Target View를 생성하는데 실패하였습니다.");
	}
}

void ID3D11Helper::CreateShaderResoureView(IN ID3D11Device* pDevice, IN ID3D11Resource* pResource, OUT ID3D11ShaderResourceView** ppShaderResourceView)
{
	HRESULT hResult = pDevice->CreateShaderResourceView(pResource, NULL, ppShaderResourceView);
	if (FAILED(hResult))
	{
		Console("Shader Resource View를 생성하는데 실패하였습니다.");
	}
}

void ID3D11Helper::CreateDepthStencilView(IN ID3D11Device* pDevice, IN ID3D11Resource* pResource, OUT ID3D11DepthStencilView** ppDepthStencilView)
{
	HRESULT hResult = pDevice->CreateDepthStencilView(pResource, NULL, ppDepthStencilView);
	if (FAILED(hResult))
	{
		Console("Depth Stencil View를 생성하는데 실패하였습니다.");
	}
}

void ID3D11Helper::CreateRasterizerState(IN ID3D11Device* pDevice, IN D3D11_FILL_MODE eFillMode, IN D3D11_CULL_MODE eCullMode, IN BOOL bMultiSampling, OUT ID3D11RasterizerState** ppRasterizerState)
{
	D3D11_RASTERIZER_DESC sRasterizerDesc;
	AutoZeroMemory(sRasterizerDesc);
	sRasterizerDesc.FillMode = eFillMode;
	sRasterizerDesc.CullMode = eCullMode;
	sRasterizerDesc.FrontCounterClockwise = false;
	sRasterizerDesc.DepthClipEnable = true;
	sRasterizerDesc.MultisampleEnable = bMultiSampling;

	HRESULT hResult = pDevice->CreateRasterizerState(&sRasterizerDesc, ppRasterizerState);
	if (FAILED(hResult))
	{
		Console("RasterizerState를 생성하는데 실패하였습니다.");
	}
}

void ID3D11Helper::CreateDepthStencilState(IN ID3D11Device* pDevice, IN BOOL bDepthEnable, IN D3D11_COMPARISON_FUNC eDepthFunc, IN BOOL bStencilEnable, IN const D3D11_DEPTH_STENCILOP_DESC& sFrontStencilOpDesc, IN const D3D11_DEPTH_STENCILOP_DESC& sBackStencilOpDesc, OUT ID3D11DepthStencilState** ppDepthStencilState)
{
	D3D11_DEPTH_STENCIL_DESC sDepthStencilDesc;
	sDepthStencilDesc.DepthEnable = bDepthEnable;
	/*Turn on writes to the depth - stencil buffer.*/
	sDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	sDepthStencilDesc.DepthFunc = eDepthFunc;
	sDepthStencilDesc.StencilEnable = bStencilEnable;
	sDepthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	sDepthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	sDepthStencilDesc.FrontFace = sFrontStencilOpDesc;
	sDepthStencilDesc.FrontFace = sBackStencilOpDesc;

	HRESULT hResult = pDevice->CreateDepthStencilState(&sDepthStencilDesc, ppDepthStencilState);

	if (FAILED(hResult))
	{
		Console("Depth Stencil State를 생성하는데 실패하였습니다.");
	}
}

void ID3D11Helper::SetViewPort(
	IN const float& fTopLeftX,
	IN const float& fTopLeftY,
	IN const float& fWidth,
	IN const float& fHeight,
	IN const float& fMinDepth,
	IN const float& fMaxDepth,
	IN ID3D11DeviceContext* pDeviceContext,
	OUT D3D11_VIEWPORT* pScreenViewPort)
{
	D3D11_VIEWPORT& screenViewport = *pScreenViewPort;

	AutoZeroMemory(screenViewport);
	screenViewport.TopLeftX = fTopLeftX;
	screenViewport.TopLeftY = fTopLeftY;
	screenViewport.Width = float(fWidth);
	screenViewport.Height = float(fHeight);
	screenViewport.MinDepth = fMinDepth;
	screenViewport.MaxDepth = fMaxDepth;
	pDeviceContext->RSSetViewports(1, pScreenViewPort);
}

void ID3D11Helper::CreateSampler(IN D3D11_FILTER eFilter, IN D3D11_TEXTURE_ADDRESS_MODE eTextureAddressMode, IN FLOAT pFloat4[4], IN ID3D11Device* pDevice, OUT ID3D11SamplerState** ppSamplerState)
{
	D3D11_SAMPLER_DESC sSamplerDesc;
	AutoZeroMemory(sSamplerDesc);
	sSamplerDesc.Filter = eFilter;
	if (sSamplerDesc.Filter == D3D11_FILTER_ANISOTROPIC || sSamplerDesc.Filter == D3D11_FILTER_COMPARISON_ANISOTROPIC)
	{
		sSamplerDesc.MaxAnisotropy = 1;
	}
	sSamplerDesc.AddressU = eTextureAddressMode;
	sSamplerDesc.AddressV = eTextureAddressMode;
	sSamplerDesc.AddressW = eTextureAddressMode;
	memcpy(sSamplerDesc.BorderColor, pFloat4, sizeof(FLOAT) * 4);
	sSamplerDesc.MipLODBias = 0;
	sSamplerDesc.MinLOD = 0;
	sSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	sSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	HRESULT hResult = pDevice->CreateSamplerState(&sSamplerDesc, ppSamplerState);
	if (FAILED(hResult))
	{
		Console("샘플러를 생성하는데 실패하였습니다.");
	}

}

void ID3D11Helper::CreateTexture2D(IN ID3D11Device* pDevice, IN ImageContainer* pImageContainer, OUT ID3D11Texture2D** ppTexture2D)
{
	D3D11_TEXTURE2D_DESC sTexture2DDesc;
	AutoZeroMemory(sTexture2DDesc);
	sTexture2DDesc.Width = pImageContainer->uiWidth;
	sTexture2DDesc.Height = pImageContainer->uiHeight;
	sTexture2DDesc.MipLevels = 1;
	sTexture2DDesc.ArraySize = 1;
	sTexture2DDesc.SampleDesc.Count = 1;
	sTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sTexture2DDesc.CPUAccessFlags = NULL;
	sTexture2DDesc.MiscFlags = NULL;
	sTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;

	const unsigned int& uiChannel = pImageContainer->uiChannel;
	switch (uiChannel)
	{
	case 1:
		break;
		sTexture2DDesc.Format = DXGI_FORMAT_R8_UINT;
	case 2:
		sTexture2DDesc.Format = DXGI_FORMAT_R8G8_UINT;
		break;
	case 3:
		pImageContainer->ExtendChannel(4);
	case 4:
		sTexture2DDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	}

	D3D11_SUBRESOURCE_DATA sSubResource;
	sSubResource.pSysMem = pImageContainer->pData;
	sSubResource.SysMemPitch = pImageContainer->uiWidth * pImageContainer->uiChannel;
	sSubResource.SysMemSlicePitch = 0;

	HRESULT hResult = pDevice->CreateTexture2D(&sTexture2DDesc, &sSubResource, ppTexture2D);
	if (FAILED(hResult))
	{
		Console("Texture2D를 생성하는데 실패하였습니다.");
	}
}
