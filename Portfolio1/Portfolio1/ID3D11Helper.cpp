#include "ID3D11Helper.h"

#include "DepthStencilState.h"

using namespace std;

void ID3D11Helper::Init(IN ID3D11Device* pDevice, IN ID3D11DeviceContext* pDeviceContext)
{
	DepthStencilState::Init(pDevice);
}

void ID3D11Helper::CreateDeviceAndContext(
	IN const UINT& iWidth,
	IN const UINT& iHeight,
	IN bool bWindowed,
	IN HWND hOutputWindow,
	OUT UINT&	uiNumLevelQuality,
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

	cpTempDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &uiNumLevelQuality);


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

	sSwapChainDesc.SampleDesc.Count = 1;
	sSwapChainDesc.SampleDesc.Quality = 0;

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
	sSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
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

void ID3D11Helper::CreateGS(IN ID3D11Device* pDevice, IN LPCWSTR pFileName, OUT ID3D11GeometryShader** ppGeometryShader)
{
	ComPtr<ID3DBlob> cpShaderBlob;
	ComPtr<ID3DBlob> cpErrorBlob;

	HRESULT hResult = D3DCompileFromFile(
		pFileName,
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"gs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		NULL,
		cpShaderBlob.GetAddressOf(),
		cpErrorBlob.GetAddressOf()
	);

	if (FAILED(hResult))
	{
		Console("Geometry Shader를 컴파일하는데 실패하였습니다.");
		return;
	}

	hResult = pDevice->CreateGeometryShader(
		cpShaderBlob->GetBufferPointer(),
		cpShaderBlob->GetBufferSize(),
		NULL,
		ppGeometryShader
	);

	if (FAILED(hResult))
	{
		Console("Geometry Shader 생성하는데 실패하였습니다.");
		return;
	}
}

void ID3D11Helper::CreateHS(IN ID3D11Device* pDevice, IN LPCWSTR pFileName, OUT ID3D11HullShader** ppHullShader)
{
	ComPtr<ID3DBlob> cpShaderBlob;
	ComPtr<ID3DBlob> cpErrorBlob;

	HRESULT hResult = D3DCompileFromFile(
		pFileName,
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"hs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		NULL,
		cpShaderBlob.GetAddressOf(),
		cpErrorBlob.GetAddressOf()
	);

	if (FAILED(hResult))
	{
		Console("Hull Shader를 컴파일하는데 실패하였습니다.");
		return;
	}

	hResult = pDevice->CreateHullShader(
		cpShaderBlob->GetBufferPointer(),
		cpShaderBlob->GetBufferSize(),
		NULL,
		ppHullShader
	);

	if (FAILED(hResult))
	{
		Console("Hull Shader 생성하는데 실패하였습니다.");
		return;
	}
}

void ID3D11Helper::CreateDS(IN ID3D11Device* pDevice, IN LPCWSTR pFileName, OUT ID3D11DomainShader** ppDomainShader)
{
	ComPtr<ID3DBlob> cpShaderBlob;
	ComPtr<ID3DBlob> cpErrorBlob;

	HRESULT hResult = D3DCompileFromFile(
		pFileName,
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"ds_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		NULL,
		cpShaderBlob.GetAddressOf(),
		cpErrorBlob.GetAddressOf()
	);

	if (FAILED(hResult))
	{
		Console("Domain Shader를 컴파일하는데 실패하였습니다.");
		return;
	}

	hResult = pDevice->CreateDomainShader(
		cpShaderBlob->GetBufferPointer(),
		cpShaderBlob->GetBufferSize(),
		NULL,
		ppDomainShader
	);

	if (FAILED(hResult))
	{
		Console("Domain Shader 생성하는데 실패하였습니다.");
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

void ID3D11Helper::CreateDepthStencilView(IN ID3D11Device* pDevice, IN const UINT& uiWidth, IN const UINT& uiHeight, IN const UINT& uiNumQualityLevels, IN OUT ID3D11Texture2D** ppDepthStencilTexture2D,  OUT ID3D11DepthStencilView** ppDepthStencilView)
{
	D3D11_TEXTURE2D_DESC sDepthStencilTextureDesc;
	AutoZeroMemory(sDepthStencilTextureDesc);
	sDepthStencilTextureDesc.Width = uiWidth;
	sDepthStencilTextureDesc.Height = uiHeight;
	sDepthStencilTextureDesc.MipLevels = 1;
	sDepthStencilTextureDesc.ArraySize = 1;
	sDepthStencilTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if (uiNumQualityLevels > 0) {
		sDepthStencilTextureDesc.SampleDesc.Count = 4; // how many multisamples
		sDepthStencilTextureDesc.SampleDesc.Quality = uiNumQualityLevels - 1;
	}
	else {
		sDepthStencilTextureDesc.SampleDesc.Count = 1; // how many multisamples
		sDepthStencilTextureDesc.SampleDesc.Quality = 0;
	}
	sDepthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	sDepthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	sDepthStencilTextureDesc.CPUAccessFlags = 0;
	sDepthStencilTextureDesc.MiscFlags = 0;

	HRESULT hResult = pDevice->CreateTexture2D(&sDepthStencilTextureDesc, NULL, ppDepthStencilTexture2D);

	if (FAILED(hResult))
	{
		Console("Depth Stencil Texture 2D를 생성하는데 실패하였습니다.");
		return;
	}

	hResult = pDevice->CreateDepthStencilView(*ppDepthStencilTexture2D, NULL, ppDepthStencilView);

	if (FAILED(hResult))
	{
		Console("Depth Stencil Shader Resource View를 생성하는데 실패하였습니다.");
		return;
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
	sDepthStencilDesc.BackFace = sBackStencilOpDesc;

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
	if (pFloat4 == NULL)
	{
		memset(sSamplerDesc.BorderColor, 0, sizeof(FLOAT) * 4);
	}
	else
	{
		memcpy(sSamplerDesc.BorderColor, pFloat4, sizeof(FLOAT) * 4);
	}
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

void ID3D11Helper::CreateTexture2D(
	IN ID3D11Device* pDevice,
	IN ID3D11DeviceContext* pDeviceContext,
	IN const UINT& uiWidth,
	IN const UINT& uiHeight,
	IN DXGI_FORMAT eFormat,
	IN const uint8_t* const pImageRawData,
	OUT ID3D11Texture2D** ppTexture2D)
{
	ComPtr<ID3D11Texture2D> cpStagingTexture = CreateStagingTexture2D(pDevice, pDeviceContext, uiWidth, uiHeight, eFormat, pImageRawData);

	D3D11_TEXTURE2D_DESC sTexture2DDesc;
	AutoZeroMemory(sTexture2DDesc);
	sTexture2DDesc.Width = uiWidth;
	sTexture2DDesc.Height = uiHeight;
	sTexture2DDesc.MipLevels = 0;
	sTexture2DDesc.ArraySize = 1;
	sTexture2DDesc.Format = eFormat;
	sTexture2DDesc.SampleDesc.Count = 1;
	sTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	sTexture2DDesc.CPUAccessFlags = NULL;
	sTexture2DDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	sTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;

	HRESULT hResult = pDevice->CreateTexture2D(&sTexture2DDesc, NULL, ppTexture2D);
	if (FAILED(hResult))
	{
		Console("Texture2D를 생성하는데 실패하였습니다.");
	}

	pDeviceContext->CopySubresourceRegion(*ppTexture2D, 0, 0, 0, 0,
		cpStagingTexture.Get(), 0, NULL);
}

void ID3D11Helper::CreateTexture2D(IN ID3D11Device* pDevice, IN const D3D11_TEXTURE2D_DESC sTexture2DDesc, OUT ID3D11Texture2D** ppTexture2D)
{
	HRESULT hResult = pDevice->CreateTexture2D(&sTexture2DDesc, nullptr, ppTexture2D);
	if (FAILED(hResult))
	{
		Console("Texture2D를 생성하는데 실패하였습니다.");
	}
}

void ID3D11Helper::CreateTexture2D(
	IN ID3D11Device* pDevice,
	IN UINT uiWidth,
	IN UINT uiHeight,
	IN UINT uiArraySize,
	IN UINT uiNumQualityLevels,
	IN UINT uiBindFlag,
	IN UINT uiCPUAccess,
	IN UINT uiMiscFlag,
	IN D3D11_USAGE eUsage,
	IN DXGI_FORMAT eFormat,
	OUT ID3D11Texture2D** ppTexture2D
)
{
	D3D11_TEXTURE2D_DESC sTexture2DDesc;
	AutoZeroMemory(sTexture2DDesc);
	sTexture2DDesc.Width = uiWidth;
	sTexture2DDesc.Height = uiHeight;
	sTexture2DDesc.ArraySize = uiArraySize;
	sTexture2DDesc.MipLevels = 1;
	if (uiNumQualityLevels > 0) {
		sTexture2DDesc.SampleDesc.Count = 4; // how many multisamples
		sTexture2DDesc.SampleDesc.Quality = uiNumQualityLevels - 1;
	}
	else {
		sTexture2DDesc.SampleDesc.Count = 1; // how many multisamples
		sTexture2DDesc.SampleDesc.Quality = 0;
	}
	sTexture2DDesc.BindFlags = uiBindFlag;
	sTexture2DDesc.CPUAccessFlags = uiCPUAccess;
	sTexture2DDesc.MiscFlags = uiMiscFlag;
	sTexture2DDesc.Usage = eUsage;
	sTexture2DDesc.Format = eFormat;
	HRESULT hResult = pDevice->CreateTexture2D(&sTexture2DDesc,nullptr, ppTexture2D);
	if (FAILED(hResult))
	{
		Console("Texture2D를 생성하는데 실패하였습니다.");
	}
}


ComPtr<ID3D11Texture2D> ID3D11Helper::CreateStagingTexture2D(
	IN ID3D11Device* pDevice,
	IN ID3D11DeviceContext* pDeviceContext,
	IN const UINT& uiWidth,
	IN const UINT& uiHeight,
	IN DXGI_FORMAT eFormat,
	IN const uint8_t* const pImageRawData)
{
	D3D11_TEXTURE2D_DESC sTexture2DDesc;
	AutoZeroMemory(sTexture2DDesc);
	sTexture2DDesc.Width = uiWidth;
	sTexture2DDesc.Height = uiHeight;
	sTexture2DDesc.MipLevels = 1;
	sTexture2DDesc.ArraySize = 1;
	sTexture2DDesc.Format = eFormat;
	sTexture2DDesc.SampleDesc.Count = 1;
	sTexture2DDesc.BindFlags = NULL;
	sTexture2DDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	sTexture2DDesc.MiscFlags = NULL;
	sTexture2DDesc.Usage = D3D11_USAGE_STAGING;

	ComPtr<ID3D11Texture2D> cpStagingTexture;
	HRESULT hResult = pDevice->CreateTexture2D(&sTexture2DDesc, NULL, cpStagingTexture.GetAddressOf());
	if (FAILED(hResult))
	{
		Console("Texture2D를 생성하는데 실패하였습니다.");
	}

	D3D11_MAPPED_SUBRESOURCE ms;
	pDeviceContext->Map(cpStagingTexture.Get(), NULL, D3D11_MAP_WRITE, NULL, &ms);

	uint8_t* pData = (uint8_t*)ms.pData;
	memcpy(pData, pImageRawData, uiWidth * uiHeight * 4 * sizeof(uint8_t));
	pDeviceContext->Unmap(cpStagingTexture.Get(), NULL);

	return cpStagingTexture;
}