#include "ICamera.h"

#include <memory>

std::shared_ptr<ICamera>	ICamera::DefaultCamera = nullptr;
const float					ICamera::DefaultClearColor[4] = { 0.f, 0.f, 0.f, 1.f };

using namespace DirectX;

ICamera::ICamera(ComPtr<ID3D11Device>& cpDeviceIn, 
	ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	ComPtr<IDXGISwapChain>& cpSwapChainIn, const float& fAspectRatioIn
)
	: cpDevice(cpDeviceIn),
	cpDeviceContext(cpDeviceContextIn),
	cpSwapChain(cpSwapChainIn),
	fAspectRatio(fAspectRatioIn)
{
	ID3D11Helper::GetBackBuffer(cpSwapChain.Get(), cpBackBuffer.GetAddressOf());
	ID3D11Helper::CreateRenderTargetView(cpDevice.Get(), cpBackBuffer.Get(), cpRenderTargetView.GetAddressOf());
	
	AutoZeroMemory(sCameraInfo);
	sCameraInfo.xmvCameraPosition = XMVectorSet(0.f, 0.f, -10.f, 0.f);
	sCameraInfo.xmvCameraDirection = XMVectorSet(0.3f, 0.f, 1.f, 0.f);
	sCameraInfo.xmvCameraUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	sCameraInfo.fFovAngle = XMConvertToRadians(70.f);
	sCameraInfo.fAspectRatio = fAspectRatio;
	sCameraInfo.fNearZ = 0.01f;
	sCameraInfo.fFarZ = 100.f;

	XMMATRIX xmmViewProjTransposed = GetViewProjTransposed();

	ID3D11Helper::CreateBuffer(
		cpDevice.Get(),
		xmmViewProjTransposed,
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE, 0,
		cpCameraConstantBuffer.GetAddressOf()
	);

}

void ICamera::Update()
{
	XMMATRIX xmmViewProjTransposed = GetViewProjTransposed();
	ID3D11Helper::UpdateBuffer(cpDeviceContext.Get(), xmmViewProjTransposed, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, cpCameraConstantBuffer.Get());

	cpDeviceContext->VSSetConstantBuffers(0, 1, cpCameraConstantBuffer.GetAddressOf());
}

void ICamera::WipeOut(const float fcolor[4])
{
	cpDeviceContext->ClearRenderTargetView(cpRenderTargetView.Get(), fcolor);
	cpDeviceContext->OMSetRenderTargets(1, cpRenderTargetView.GetAddressOf(), nullptr);

}

XMMATRIX ICamera::GetViewProjTransposed()
{
	XMMATRIX test = XMMatrixLookToLH(sCameraInfo.xmvCameraPosition, sCameraInfo.xmvCameraDirection, sCameraInfo.xmvCameraUp);

	// 위치 좌표 벡터에 모델 -> 뷰 -> 프로젝션 행렬의 순서로 연산이 되어야 한다.
	// DirectX11에서는 Row Major Order가 사용되기 때문에,
	// 위치 좌표 벡터 * 모델 행렬 * 뷰 행렬 * 프로젝션 행렬의 형태가 되어야 한다.
	// DirectX11에서 사용하던 Row Major Order가 쉐이더에서는 Column Major Order로 변환되므로,
	// 프로젝션 행렬 * 뷰 행렬 * 모델 행렬 * 위치 좌표 벡터의 형태가 되어야 한다.

	return XMMatrixLookToLH(sCameraInfo.xmvCameraPosition, sCameraInfo.xmvCameraDirection, sCameraInfo.xmvCameraUp) * 
		XMMatrixPerspectiveFovLH(sCameraInfo.fFovAngle, sCameraInfo.fAspectRatio, sCameraInfo.fNearZ, sCameraInfo.fFarZ)
	;
}