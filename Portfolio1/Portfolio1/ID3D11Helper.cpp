#include "ID3D11Helper.h"

void ID3D11Helper::CreateDeviceAndContext(
	IN const int& iWidth, 
	IN const int& iHeight,
	IN bool bWindowed,
	IN HWND hOutputWindow,
	OUT IDXGISwapChain** ppSwapChain,
	OUT ID3D11Device** ppDevice,
	OUT ID3D11DeviceContext** ppDeviceContext
)
{
	DXGI_SWAP_CHAIN_DESC sSwapChainDesc;
	AutoZeroMemory(sSwapChainDesc);
	sSwapChainDesc.BufferDesc.Width = iWidth;
	sSwapChainDesc.BufferDesc.Height = iHeight;
	sSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	sSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	sSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	sSwapChainDesc.SampleDesc.Count = 1;
	/*sSwapChainDesc.SampleDesc.Quality = 0;*/

	sSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sSwapChainDesc.BufferCount = 2;
	sSwapChainDesc.OutputWindow = hOutputWindow;
	sSwapChainDesc.Windowed = bWindowed;

	sSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	//const D3D_FEATURE_LEVEL pFeatureLevel[3] = { D3D_FEATURE_LEVEL_11_0 , D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_9_3 };
	const D3D_FEATURE_LEVEL pFeatureLevel[1] = { D3D_FEATURE_LEVEL_11_0/* , D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_9_3 */};
	HRESULT hResult = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		pFeatureLevel /* D3D_FEATURE_LEVEL_11_0, 10_1, 10_0, 9_3, 9_2, 9_1 */,
		1,
		D3D11_SDK_VERSION,
		&sSwapChainDesc,
		ppSwapChain,
		ppDevice,
		NULL,
		ppDeviceContext
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

	hResult = pDevice->CreateInputLayout(vInputElementDescs.data(), vInputElementDescs.size(), cpShaderBlob->GetBufferPointer(), cpShaderBlob->GetBufferSize(), ppInputLayout);
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
	//HRESULT hResult = pDevice->CreateShaderResourceView(pResource, NULL, ppShaderResourceView);
	//if (FAILED(hResult))
	//{
	//	Console("Shader Resource View를 생성하는데 실패하였습니다.");
	//}
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
