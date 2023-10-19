#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <directxmath/DirectXMath.h>
#include <functional>

enum MoveDir;

class CameraInterface
{
public:
	void (*Update)();
	void (*Resize)(const float&);
	void (*WipeOut)(const DirectX::XMVECTOR&);
	std::function<void()> SetRSState;
	std::function<void()> SetVSConstantBuffers;
	std::function<void()> SetHSConstantBuffers;
	std::function<void()> SetDSConstantBuffers;
	std::function<void()> SetGSConstantBuffers;
	std::function<void()> SetPSConstantBuffers;
	std::function<void()> OMSetRenderTargets;

public:
	std::function<struct ModelIDData()> GetPointedModelID;
	std::function<void(MoveDir)> StartMove;
	std::function<void(MoveDir)> StopMove;
	std::function<void(const int&, const int&)> SetFromMouseXY;
	std::function<void()> SwitchFirstView;

public:
	std::function<void()> PostProcess;
};

template <typename CameraImpl>
class CameraAdapter : public CameraInterface
{
public:
	template<typename ...Args>
	CameraAdapter<CameraImpl>(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		Microsoft::WRL::ComPtr<IDXGISwapChain>& cpSwapChainIn,
		Args... args
	);

public:
	CameraImpl cameraImpl;
};

template<typename CameraImpl>
template<typename ...Args>
CameraAdapter<CameraImpl>::CameraAdapter(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	Microsoft::WRL::ComPtr<IDXGISwapChain>& cpSwapChainIn,
	Args... args
)
	: cameraImpl(CameraImpl(cpDeviceIn, cpDeviceContextIn, cpSwapChainIn, args...))
{
	Update = &CameraImpl::Update;
	Resize = &CameraImpl::Resize;
	WipeOut = &CameraImpl::WipeOut;
	SetRSState = [this]() { cameraImpl.SetRSState(); };
	SetVSConstantBuffers = [this]() { cameraImpl.SetVSConstantBuffers(); };
	SetHSConstantBuffers = [this]() { cameraImpl.SetHSConstantBuffers(); };
	SetDSConstantBuffers = [this]() { cameraImpl.SetDSConstantBuffers(); };
	SetGSConstantBuffers = [this]() { cameraImpl.SetGSConstantBuffers(); };
	SetPSConstantBuffers = [this]() { cameraImpl.SetPSConstantBuffers(); };
	OMSetRenderTargets = [this]() { cameraImpl.OMSetRenderTargets(); };

	GetPointedModelID = [this]() { return cameraImpl.GetPointedModelID(); };
	StartMove = [this](MoveDir moveDir) {cameraImpl.StartMove(moveDir); };
	StopMove = [this](MoveDir moveDir) {cameraImpl.StopMove(moveDir); };
	SetFromMouseXY = [this](const int& iMouseX, const int& iMouseY) {cameraImpl.SetFromMouseXY(iMouseX, iMouseY); };
	SwitchFirstView = [this]() { cameraImpl.SwitchFirstView(); };

	PostProcess = [this]() { cameraImpl.PostProcess(); };
};
