#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <directxmath/DirectXMath.h>
#include <memory>

enum MoveDir
{
	Forward,
	Left,
	Backward,
	Right,
	NUM_DIR
};

class CameraInterface
{
public:
	CameraInterface(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
		const UINT& uiWidthIn, const UINT& uiHeightIn,
		const UINT& uiNumLevelQualityIn, const DXGI_FORMAT& eCameraFormatIn,
		const float& fPosXIn, const float& fPosYIn, const float& fPosZIn,
		const float& fFovAngleIn, const float& fNearZIn, const float& fFarZIn
	);
	virtual ~CameraInterface();
	
public:
	static const DirectX::XMVECTOR			DefaultDirection;
	static const DirectX::XMVECTOR			DefaultUp;
	static const DirectX::XMVECTOR			DefaultRight;

protected:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
	IDXGISwapChain* pSwapChain;

public:
	UINT			uiWidth;
	UINT			uiHeight;
	UINT			uiNumLevelQuality;

public:
	unsigned int			uiMouseLocation[2];
	float					fRoll;
	float					fPitch;
	float					fYaw;
	float					fFovRadian;
	float					fAspectRatio;
	float					fNearZ;
	float					fFarZ;
	float					fMoveSpeed;
	float					fMouseMovablePitchAngleDegree;
	float					fMouseMovableYawAngleDegree;
	bool					bMoveDirection[MoveDir::NUM_DIR];
	bool					bFirstView;

public:
	struct
	{
		DirectX::XMVECTOR			xmvCameraPosition;
		DirectX::XMMATRIX			xmmViewProjMat;
		DirectX::XMMATRIX			xmmInvViewProjMat;
	} sCameraViewProjData;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpCameraViewProjConstantBuffer;

protected:
	D3D11_VIEWPORT sCameraViewport;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpBackBuffer;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		cpBackBufferRTV;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			cpDepthStencilTexture2D;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	cpDepthStencilView;

protected:
	DXGI_FORMAT										eCameraFormat;
	DXGI_FORMAT										eBackBufferFormat;

public:
	void SetAsMainCamera(IDXGISwapChain* pSwapChainIn);
	void ResetFromMainCamera();
	void SetRenderResources();

public:
	inline DXGI_FORMAT GetRenderedTextureFormat() { return eCameraFormat; }
	inline ID3D11Texture2D* GetRenderedTexture() { return cpCameraOutputTexture.Get(); }

public:
	 inline ID3D11Texture2D* GetOutputTexture2D() { return cpBackBuffer.Get(); }
	 inline ID3D11Texture2D** GetAddressOfOutputTexture2D() { return cpBackBuffer.GetAddressOf(); }

	 inline ID3D11RenderTargetView* GetOutputRTV() { return cpBackBufferRTV.Get(); }
	 inline ID3D11RenderTargetView** GetAddressOfOutputRTV() { return cpBackBufferRTV.GetAddressOf(); }

public:
	std::unique_ptr<class PostProcess> upPostProcess;

public:
	void DoPostProcess();

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpCameraOutputTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		cpCameraOutputRTV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	cpCameraOutputSRV;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpModelIDTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		cpModelIDRTV;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpModelIDMSToSS;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpModelIDStagedTexture;

public:
	unsigned int GetPointedModelIDAsRGBA();

public:
	inline void StartMove(MoveDir moveDir) { bMoveDirection[moveDir] = true; }
	inline void StopMove(MoveDir moveDir) { bMoveDirection[moveDir] = false; }

public:
	void SetFromMouseXY(const int& iMouseX, const int& iMouseY);

public:
	inline void SwitchFirstView() { bFirstView = !bFirstView; }

public:
	void WipeOut();

#pragma region Virtual
// Virtual Function ==============================================
public:
	virtual void Update(const float& fDelta);
	virtual void Resize(
		const UINT& uiWidthIn,
		const UINT& uiHeightIn,
		const float& fAspectRatioIn
	);
// ==============================================================
#pragma endregion

#pragma region Interface
// Interface Function ============================================
public:
	virtual void SetConstantBuffers() = 0;
	virtual void OMSetRenderTargets() = 0;
	virtual void ResetCamera() = 0;
// ==============================================================
#pragma endregion
};
