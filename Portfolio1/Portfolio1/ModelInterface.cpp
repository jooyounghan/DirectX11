#include "ModelInterface.h"
#include "TransformProperties.h"
#include "EnumVar.h"
#include "ID3D11Helper.h"
#include <atomic>

using namespace std;
using namespace DirectX;

ModelInterface::ModelInterface(
	ComPtr<ID3D11Device>& cpDeviceIn,
	ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn), upTransformationProperties(make_unique<TransformProperties>()), upTransformationBufferData(make_unique<TransformationBufferData>())
{
	XMMATRIX transformMatrix = TransformProperties::GetAffineTransformMatrix(upTransformationProperties.get());
	ID3D11Helper::CreateBuffer(
		cpDevice.Get(),
		TransformationBufferData::CreateTransfomredMatrix(&transformMatrix),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		0,
		cpTransformationDataBuffer.GetAddressOf()
	);
}

ModelInterface::~ModelInterface()
{

}

void ModelInterface::Update()
{
	XMMATRIX transformMatrix = TransformProperties::GetAffineTransformMatrix(upTransformationProperties.get());
	ID3D11Helper::UpdateBuffer(
		cpDeviceContext.Get(),
		TransformationBufferData::CreateTransfomredMatrix(&transformMatrix),
		D3D11_MAP_WRITE_DISCARD,
		cpTransformationDataBuffer.Get()
	);
}

void ModelInterface::Render()
{
	cpDeviceContext->DrawIndexed(ui32IndexCount, 0, 0);
}

void ModelInterface::ScaleUp(const float& x, const float& y, const float& z)
{
	upTransformationProperties->xmvScale.m128_f32[0] += x;
	upTransformationProperties->xmvScale.m128_f32[1] += y;
	upTransformationProperties->xmvScale.m128_f32[2] += z;

	XMMATRIX transformMatrix = TransformProperties::GetAffineTransformMatrix(upTransformationProperties.get());

	ID3D11Helper::UpdateBuffer(
		cpDeviceContext.Get(),
		TransformationBufferData::CreateTransfomredMatrix(&transformMatrix),
		D3D11_MAP_WRITE_DISCARD,
		cpTransformationDataBuffer.Get()
	);
}
