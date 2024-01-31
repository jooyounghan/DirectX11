#include "ID3D11Helper.h"

using namespace std;

void ID3D11Helper::CreateDeviceAndContext(
	IN const UINT& uiWidthIn,
	IN const UINT& uiHeightIn,
	IN const DXGI_FORMAT& eFormatIn,
	IN const bool& bWindowed,
	IN HWND hOutputWindow,
	OUT ComPtr<ID3D11Device>& cpDevice,
	OUT ComPtr<ID3D11DeviceContext>& cpDeviceContext,
	OUT ComPtr<IDXGISwapChain>& cpSwapChain
)
{
	const D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	const D3D_FEATURE_LEVEL pFeatureLevel[3] = { D3D_FEATURE_LEVEL_11_0 , D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_9_3 };
	D3D_FEATURE_LEVEL featureLevel;

	DXGI_SWAP_CHAIN_DESC sSwapChainDesc;
	AutoZeroMemory(sSwapChainDesc);
	sSwapChainDesc.BufferDesc.Width = uiWidthIn;
	sSwapChainDesc.BufferDesc.Height = uiHeightIn;
	sSwapChainDesc.BufferDesc.RefreshRate.Numerator = 240;
	sSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	sSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	sSwapChainDesc.SampleDesc.Count = 1;
	sSwapChainDesc.SampleDesc.Quality = 0;

	sSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sSwapChainDesc.BufferCount = 2;
	sSwapChainDesc.OutputWindow = hOutputWindow;
	sSwapChainDesc.Windowed = bWindowed;
	sSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hResult = D3D11CreateDeviceAndSwapChain(
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
		Console::AssertPrint("Device와 Device Context를 생성하는데 실패하였습니다.");
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
		Console::AssertPrint("Vertex Shader를 컴파일하는데 실패하였습니다.");
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
		Console::AssertPrint("Vertex Shader를 생성하는데 실패하였습니다.");
		return;
	}

	hResult = pDevice->CreateInputLayout(vInputElementDescs.data(), (UINT)vInputElementDescs.size(), cpShaderBlob->GetBufferPointer(), cpShaderBlob->GetBufferSize(), ppInputLayout);
	if (FAILED(hResult))
	{
		Console::AssertPrint("InputLayout을 생성하는데 실패하였습니다.");
		return;
	}
}

bool ID3D11Helper::GetBackBuffer(IN IDXGISwapChain* pSwapChain, OUT ID3D11Texture2D** ppTexture2D)
{
	HRESULT hResult = pSwapChain->GetBuffer(0, IID_PPV_ARGS(ppTexture2D));
	if (FAILED(hResult))
	{
		Console::AssertPrint("Back Buffer를 불러오는데 실패하였습니다.");
		return false;
	}
	return true;
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
		Console::AssertPrint("Pixel Shader를 컴파일하는데 실패하였습니다.");
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
		Console::AssertPrint("Pixel Shader 생성하는데 실패하였습니다.");
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
		Console::AssertPrint("Geometry Shader를 컴파일하는데 실패하였습니다.");
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
		Console::AssertPrint("Geometry Shader 생성하는데 실패하였습니다.");
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
		Console::AssertPrint("Hull Shader를 컴파일하는데 실패하였습니다.");
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
		Console::AssertPrint("Hull Shader 생성하는데 실패하였습니다.");
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
		Console::AssertPrint("Domain Shader를 컴파일하는데 실패하였습니다.");
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
		Console::AssertPrint("Domain Shader 생성하는데 실패하였습니다.");
		return;
	}
}

void ID3D11Helper::CreateCS(IN ID3D11Device* pDevice, IN LPCWSTR pFileName, OUT ID3D11ComputeShader** ppComputeShader)
{
	ComPtr<ID3DBlob> cpShaderBlob;
	ComPtr<ID3DBlob> cpErrorBlob;

	HRESULT hResult = D3DCompileFromFile(
		pFileName,
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"cs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		NULL,
		cpShaderBlob.GetAddressOf(),
		cpErrorBlob.GetAddressOf()
	);

	if (FAILED(hResult))
	{
		Console::AssertPrint("Domain Shader를 컴파일하는데 실패하였습니다.");
		return;
	}

	hResult = pDevice->CreateComputeShader(
		cpShaderBlob->GetBufferPointer(),
		cpShaderBlob->GetBufferSize(),
		NULL,
		ppComputeShader
	);

	if (FAILED(hResult))
	{
		Console::AssertPrint("Compute Shader 생성하는데 실패하였습니다.");
		return;
	}
}

void ID3D11Helper::CreateRenderTargetView(IN ID3D11Device* pDevice, IN ID3D11Resource* pResource, OUT ID3D11RenderTargetView** ppRenderTargetView)
{
	HRESULT hResult = pDevice->CreateRenderTargetView(pResource, NULL, ppRenderTargetView);
	if (FAILED(hResult))
	{
		Console::AssertPrint("Render Target View를 생성하는데 실패하였습니다.");
	}
}

void ID3D11Helper::CreateShaderResoureView(
	IN ID3D11Device* pDevice, 
	IN ID3D11Resource* pResource, 
	OUT ID3D11ShaderResourceView** ppShaderResourceView,
	IN D3D11_SHADER_RESOURCE_VIEW_DESC* sSRVDesc
)
{
	HRESULT hResult = pDevice->CreateShaderResourceView(pResource, sSRVDesc, ppShaderResourceView);
	if (FAILED(hResult))
	{
		Console::AssertPrint("Shader Resource View를 생성하는데 실패하였습니다.");
	}
}

void ID3D11Helper::CreateUnorderedAccessView(IN ID3D11Device* pDevice, IN ID3D11Resource* pResource, OUT ID3D11UnorderedAccessView** ppUnorderedAccessView)
{
	HRESULT hResult = pDevice->CreateUnorderedAccessView(pResource, NULL, ppUnorderedAccessView);
	if (FAILED(hResult))
	{
		Console::AssertPrint("Unordered Access View를 생성하는데 실패하였습니다.");
	}
}

void ID3D11Helper::CreateDepthStencilView(
	IN ID3D11Device* pDevice, 
	IN ID3D11Texture2D* pDepthStencilTexture2D, 
	OUT ID3D11DepthStencilView** ppDepthStencilView,
	IN D3D11_DEPTH_STENCIL_VIEW_DESC* sDSVDesc
)
{
	HRESULT hResult = pDevice->CreateDepthStencilView(pDepthStencilTexture2D, sDSVDesc, ppDepthStencilView);

	if (FAILED(hResult))
	{
		Console::AssertPrint("Depth Stencil View를 생성하는데 실패하였습니다.");
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
		Console::AssertPrint("RasterizerState를 생성하는데 실패하였습니다.");
	}
}

void ID3D11Helper::CreateDepthStencilState(IN ID3D11Device* pDevice, IN BOOL bDepthEnable, IN D3D11_COMPARISON_FUNC eDepthFunc, IN BOOL bStencilEnable, IN const D3D11_DEPTH_STENCILOP_DESC& sFrontStencilOpDesc, IN const D3D11_DEPTH_STENCILOP_DESC& sBackStencilOpDesc, OUT ID3D11DepthStencilState** ppDepthStencilState)
{
	D3D11_DEPTH_STENCIL_DESC sDepthStencilDesc;
	AutoZeroMemory(sDepthStencilDesc);
	sDepthStencilDesc.DepthEnable = bDepthEnable;
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
		Console::AssertPrint("Depth Stencil State를 생성하는데 실패하였습니다.");
	}
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
		Console::AssertPrint("샘플러를 생성하는데 실패하였습니다.");
	}

}

void ID3D11Helper::CreateTexture2D(
	IN ID3D11Device* pDevice,
	IN const UINT& uiWidth, IN const UINT& uiHeight, 
	IN const UINT& uiBindFlag, 
	IN const UINT& uiCPUAccess,
	IN const UINT& uiMiscFlag, 
	IN D3D11_USAGE eUsage, 
	IN DXGI_FORMAT eFormat, 
	IN uint8_t* pImageRawData,
	OUT ID3D11Texture2D** ppTexture2D
)
{
	D3D11_TEXTURE2D_DESC sTexture2DDesc;
	AutoZeroMemory(sTexture2DDesc);
	sTexture2DDesc.Width = uiWidth;
	sTexture2DDesc.Height = uiHeight;
	sTexture2DDesc.ArraySize = 1;
	sTexture2DDesc.MipLevels = 1;
	sTexture2DDesc.SampleDesc.Count = 1;
	sTexture2DDesc.SampleDesc.Quality = 0;
	sTexture2DDesc.BindFlags = uiBindFlag;
	sTexture2DDesc.CPUAccessFlags = uiCPUAccess;
	sTexture2DDesc.MiscFlags = uiMiscFlag;
	sTexture2DDesc.Usage = eUsage;
	sTexture2DDesc.Format = eFormat;

	D3D11_SUBRESOURCE_DATA pData;
	pData.pSysMem = pImageRawData;

	switch (eFormat)
	{
	case DXGI_FORMAT_R16G16B16A16_UINT:
		pData.SysMemPitch = uiWidth * 8;
		pData.SysMemSlicePitch = uiWidth * uiHeight * 8;
		break;
	case DXGI_FORMAT_R8G8B8A8_UINT:
	default:
		pData.SysMemPitch = uiWidth * 4;
		pData.SysMemSlicePitch = uiWidth * uiHeight * 4;
		break;
	}

	HRESULT hResult = pDevice->CreateTexture2D(&sTexture2DDesc, &pData, ppTexture2D);
	if (FAILED(hResult))
	{
		Console::AssertPrint("Texture2D를 생성하는데 실패하였습니다.");
	}
}

void ID3D11Helper::CreateTexture2D(
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
		Console::AssertPrint("Texture2D를 생성하는데 실패하였습니다.");
	}
}