#include "PickableModel.h"
#include "TransformProperties.h"
#include "ModelID.h"
#include "ID3D11Helper.h"

using namespace std;
using namespace DirectX;

PickableModel::PickableModel(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
)
	: ModelInterface(cpDeviceIn, cpDeviceContextIn), upModelID(make_unique<ModelID>(cpDevice.Get())),
	upTransformationProperties(make_unique<TransformProperties>()), upTransformationBufferData(make_unique<TransformationBufferData>())
{
	XMMATRIX transformMatrix = TransformProperties::GetAffineTransformMatrix(upTransformationProperties.get());
	ID3D11Helper::CreateBuffer(
		cpDevice.Get(),
		TransformationBufferData::CreateTransfomredMatrix(transformMatrix),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		0,
		cpTransformationDataBuffer.GetAddressOf()
	);
}

PickableModel::~PickableModel()
{
}

void PickableModel::Update(const float& fDelta)
{
	XMMATRIX transformMatrix = TransformProperties::GetAffineTransformMatrix(upTransformationProperties.get());
	ID3D11Helper::UpdateBuffer(
		cpDeviceContext.Get(),
		TransformationBufferData::CreateTransfomredMatrix(transformMatrix),
		D3D11_MAP_WRITE_DISCARD,
		cpTransformationDataBuffer.Get()
	);
}

void PickableModel::Render()
{
	cpDeviceContext->DrawIndexed(ui32IndexCount, 0, 0);
}

void PickableModel::ScaleUp(const float& x, const float& y, const float& z)
{
	upTransformationProperties->xmvScale.m128_f32[0] += x;
	upTransformationProperties->xmvScale.m128_f32[1] += y;
	upTransformationProperties->xmvScale.m128_f32[2] += z;

	XMMATRIX transformMatrix = TransformProperties::GetAffineTransformMatrix(upTransformationProperties.get());

	ID3D11Helper::UpdateBuffer(
		cpDeviceContext.Get(),
		TransformationBufferData::CreateTransfomredMatrix(transformMatrix),
		D3D11_MAP_WRITE_DISCARD,
		cpTransformationDataBuffer.Get()
	);
}


void PickableModel::MakePlaneVertexIndexSet(
	PickableModel* pPickableModel,
	XMVECTOR& xmvDirection,
	XMVECTOR& xmvUp,
	const float& fCenterX,
	const float& fCenterY,
	const float& fCenterZ,
	const float& fWidth,
	const float& fHeight
)
{
	if (pPickableModel != nullptr)
	{
		pPickableModel->upTransformationProperties->xmvTranslation.m128_f32[0] = fCenterX;
		pPickableModel->upTransformationProperties->xmvTranslation.m128_f32[1] = fCenterY;
		pPickableModel->upTransformationProperties->xmvTranslation.m128_f32[2] = fCenterZ;

		ModelInterface::MakePlaneVertexIndexSet(pPickableModel, xmvDirection, xmvUp, fCenterX, fCenterY, fCenterZ, fWidth, fHeight);
	}
}

void PickableModel::MakeSquareVertexIndexSet(
	PickableModel* pPickableModel, 
	const float& fCenterX, 
	const float& fCenterY, 
	const float& fCenterZ, 
	const float& fLen, 
	const bool& bReverse
)
{
	if (pPickableModel != nullptr)
	{
		pPickableModel->upTransformationProperties->xmvTranslation.m128_f32[0] = fCenterX;
		pPickableModel->upTransformationProperties->xmvTranslation.m128_f32[1] = fCenterY;
		pPickableModel->upTransformationProperties->xmvTranslation.m128_f32[2] = fCenterZ;

		ModelInterface::MakeSquareVertexIndexSet(pPickableModel, fCenterX, fCenterY, fCenterZ, fLen, bReverse);
	}
}

void PickableModel::MakeSphereVertexIndexSet(
	PickableModel* pPickableModel,
	const float& fCenterX,
	const float& fCenterY,
	const float& fCenterZ,
	const float& fRadius,
	const unsigned short& usLevel
)
{
	if (pPickableModel != nullptr)
	{
		pPickableModel->upTransformationProperties->xmvTranslation.m128_f32[0] = fCenterX;
		pPickableModel->upTransformationProperties->xmvTranslation.m128_f32[1] = fCenterY;
		pPickableModel->upTransformationProperties->xmvTranslation.m128_f32[2] = fCenterZ;
		ModelInterface::MakeSphereVertexIndexSet(pPickableModel, fCenterX, fCenterY, fCenterZ, fRadius, usLevel);
	}
}
