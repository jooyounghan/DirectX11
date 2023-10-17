#pragma once

#include <directxmath/DirectXMath.h>
#include <functional>

enum MoveDir;

class CameraInterface
{
public:
	std::function<void()> Update;
	std::function<void(const float&)> Resize;
	std::function<void(const DirectX::XMVECTOR& xmvClearColor)> WipeOut;
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
};

template <typename CameraImpl>
class CameraAdapter : public CameraInterface
{
public:
	template<typename ...Args>
	CameraAdapter<CameraImpl>(Args&... args);

public:
	CameraImpl cameraImpl;
};

template<typename CameraImpl>
template<typename ...Args>
CameraAdapter<CameraImpl>::CameraAdapter(Args& ...args)
	: cameraImpl(CameraImpl(args...))
{
	Update = [this]() { cameraImpl.Update(); };
	Resize = [this](const float& fAspectRatioIn) { cameraImpl.Resize(fAspectRatioIn); };
	WipeOut = [this](const DirectX::XMVECTOR& xmvClearColor) { cameraImpl.WipeOut(xmvClearColor); };
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
};
