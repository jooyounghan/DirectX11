#include "CubeModel.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

CubeModel::CubeModel(
	const float& fCenterX,
	const float& fCenterY,
	const float& fCenterZ,
	const float& fRadius,
	const bool& bReverse,
	const size_t& usLevel
)
	: IPBRStaticMesh()
{
	xmvPosition.m128_f32[0] = fCenterX;
	xmvPosition.m128_f32[1] = fCenterY;
	xmvPosition.m128_f32[2] = fCenterZ;

	InputLayout temp;
	temp.vertex = { -1.f, -1.f, 0.f };
	temp.texcoord = { 1.f, 0.f };
	inputData.push_back(temp);
	temp.vertex = { 0.f, 1.f, 0.f };
	temp.texcoord = { 1.f, 1.f };
	inputData.push_back(temp);
	temp.vertex = { 1.f, -1.f, 0.f };
	temp.texcoord = { 0.f, 1.f };
	inputData.push_back(temp);

	indexData = { 0, 1, 2 };

	//for (size_t latitudeIdx = 0; latitudeIdx < usLevel; ++latitudeIdx)
	//{
	//	const float& fLatitudeLow = DirectX::XM_PIDIV2 / usLevel * latitudeIdx;
	//	const float& fLatitudeHigh = DirectX::XM_PIDIV2 / usLevel * (latitudeIdx + 1);
	//	const float& fLatitudeLowTextureCord = (latitudeIdx / (float)usLevel) / 2.f;
	//	const float& fLatitudeHighTextureCord = ((latitudeIdx + 1) / (float)usLevel) / 2.f;

	//	const size_t& usLatitudeOffset = inputData.size();

	//	for (size_t longitudeIdx = 0; longitudeIdx <= usLevel * 2; ++longitudeIdx)
	//	{
	//		InputLayout sInput;

	//		const float& fLongitudeLow = DirectX::XM_2PI / (usLevel * 2) * longitudeIdx;
	//		const float& fLongitudeTextureCord = longitudeIdx / (float)(usLevel * 2);

	//		sInput.vertex = { fRadius * cosf(fLongitudeLow) * cosf(fLatitudeLow), fRadius * sinf(fLatitudeLow), fRadius * cosf(fLatitudeLow) * sinf(fLongitudeLow) };
	//		sInput.texcoord = { fLongitudeTextureCord, 0.5f + fLatitudeLowTextureCord };
	//		sInput.normal = { cosf(fLongitudeLow) * cosf(fLatitudeLow), sinf(fLatitudeLow), cosf(fLatitudeLow) * sinf(fLongitudeLow) };
	//		inputData.emplace_back(sInput);

	//		sInput.vertex = { fRadius * cosf(fLongitudeLow) * cosf(fLatitudeHigh), fRadius * sinf(fLatitudeHigh), fRadius * cosf(fLatitudeHigh) * sinf(fLongitudeLow) };
	//		sInput.texcoord = { fLongitudeTextureCord, 0.5f + fLatitudeHighTextureCord };
	//		sInput.normal = { cosf(fLongitudeLow) * cosf(fLatitudeHigh), sinf(fLatitudeHigh), cosf(fLatitudeHigh) * sinf(fLongitudeLow) };
	//		inputData.emplace_back(sInput);

	//		sInput.vertex = { fRadius * cosf(fLongitudeLow) * cosf(-fLatitudeLow), fRadius * sinf(-fLatitudeLow), fRadius * cosf(-fLatitudeLow) * sinf(fLongitudeLow) };
	//		sInput.texcoord = { fLongitudeTextureCord, 0.5f - fLatitudeLowTextureCord };
	//		sInput.normal = { cosf(fLongitudeLow) * cosf(-fLatitudeLow), sinf(-fLatitudeLow), cosf(-fLatitudeLow) * sinf(fLongitudeLow) };
	//		inputData.emplace_back(sInput);

	//		sInput.vertex = { fRadius * cosf(fLongitudeLow) * cosf(-fLatitudeHigh), fRadius * sinf(-fLatitudeHigh), fRadius * cosf(-fLatitudeHigh) * sinf(fLongitudeLow) };
	//		sInput.texcoord = { fLongitudeTextureCord, 0.5f - fLatitudeHighTextureCord };
	//		sInput.normal = { cosf(fLongitudeLow) * cosf(-fLatitudeHigh), sinf(-fLatitudeHigh), cosf(-fLatitudeHigh) * sinf(fLongitudeLow) };
	//		inputData.emplace_back(sInput);
	//	}

	//	for (size_t longitudeIdx = 0; longitudeIdx < usLevel * 2; ++longitudeIdx)
	//	{
	//		const size_t& usLongitudeOffset = 4 * longitudeIdx + usLatitudeOffset;
	//		indexData.push_back(usLongitudeOffset + 0);
	//		indexData.push_back(usLongitudeOffset + 1);
	//		indexData.push_back(usLongitudeOffset + 4);
	//		indexData.push_back(usLongitudeOffset + 4);
	//		indexData.push_back(usLongitudeOffset + 1);
	//		indexData.push_back(usLongitudeOffset + 5);

	//		indexData.push_back(usLongitudeOffset + 3);
	//		indexData.push_back(usLongitudeOffset + 2);
	//		indexData.push_back(usLongitudeOffset + 7);
	//		indexData.push_back(usLongitudeOffset + 7);
	//		indexData.push_back(usLongitudeOffset + 2);
	//		indexData.push_back(usLongitudeOffset + 6);
	//	}
	//}
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, inputData, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, inputBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, indexData, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, NULL, NULL, indexBuffer.GetAddressOf());
}

CubeModel::~CubeModel()
{
}

void CubeModel::Draw()
{
	DirectXDevice::pDeviceContext->DrawIndexed((UINT)indexData.size(), NULL, NULL);
}

void CubeModel::Update(const float& fDelta)
{
	UpdateTranformMat();
	ID3D11Helper::UpdateBuffer(DirectXDevice::pDeviceContext, sTransformation, D3D11_MAP_WRITE_DISCARD, cpTransformationBuffer.Get());
}
