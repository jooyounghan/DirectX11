#include "IModel.h"
#include "ID3D11Helper.h"
#include "MathematicalHelper.h"

using namespace DirectX;

IModel::IModel(ComPtr<ID3D11Device>& cpDeviceIn, ComPtr<ID3D11DeviceContext>& cpDeviceContextIn)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn)
{
	ModelTransform::Init(&sModelTransformation);

	ID3D11Helper::CreateBuffer(
		cpDevice.Get(),
		ModelTransform::GetAffineTransformMatrix(sModelTransformation),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		0,
		cpModelMatrixBuffer.GetAddressOf()
	);
}