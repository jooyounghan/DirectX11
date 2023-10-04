#include "CubeModel.h"
#include "ID3D11Helper.h"

using namespace std;

CubeModel::CubeModel(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const float& fCenterX,
	const float& fCenterY,
	const float& fCenterZ,
	const float& fRadius,
	const unsigned short& usLevel
) : ModelInterface(cpDeviceIn, cpDeviceContextIn)
{
	vector<uint32_t> vIndex;
	vector<Vertex> vVertex;

	for (unsigned short latitudeIdx = 0; latitudeIdx < usLevel; ++latitudeIdx)
	{
		const float& fLatitudeLow = DirectX::XM_PIDIV2 / usLevel * latitudeIdx;
		const float& fLatitudeHigh = DirectX::XM_PIDIV2 / usLevel * (latitudeIdx + 1);
		const float& fLatitudeLowTextureCord = (latitudeIdx / (float)usLevel) / 2.f;
		const float& fLatitudeHighTextureCord = ((latitudeIdx + 1) / (float)usLevel) / 2.f;

		for (unsigned short longitudeIdx = 0; longitudeIdx <= usLevel * 2; ++longitudeIdx)
		{
			Vertex sTempVertex;

			const float& fLongitudeLow = DirectX::XM_PIDIV2 / (usLevel * 2) * longitudeIdx;
			const float& fLongitudeTextureCord = longitudeIdx / (float)(usLevel * 2);

			sTempVertex.sPosVec = { fRadius * cosf(fLongitudeLow) * cosf(fLatitudeLow), fRadius * cosf(fLongitudeLow) * sinf(fLatitudeLow), fRadius * sinf(fLatitudeLow) };
			sTempVertex.sTexCoord = { fLongitudeTextureCord, 0.5f + fLatitudeLowTextureCord };
			sTempVertex.sNorVec = { cosf(fLongitudeLow) * cosf(fLatitudeLow), cosf(fLongitudeLow) * sinf(fLatitudeLow), sinf(fLatitudeLow) };
			vVertex.emplace_back(sTempVertex);

			sTempVertex.sPosVec = { fRadius * cosf(fLongitudeLow) * cosf(fLatitudeHigh), fRadius * cosf(fLongitudeLow) * sinf(fLatitudeHigh), fRadius * sinf(fLatitudeHigh) };
			sTempVertex.sTexCoord = { fLongitudeTextureCord, 0.5f + fLatitudeHighTextureCord };
			sTempVertex.sNorVec = { cosf(fLongitudeLow) * cosf(fLatitudeHigh), cosf(fLongitudeLow) * sinf(fLatitudeHigh), sinf(fLatitudeHigh) };
			vVertex.emplace_back(sTempVertex);

			sTempVertex.sPosVec = { fRadius * cosf(fLongitudeLow) * cosf(-fLatitudeLow), fRadius * cosf(fLongitudeLow) * sinf(-fLatitudeLow), fRadius * sinf(-fLatitudeLow) };
			sTempVertex.sTexCoord = { fLongitudeTextureCord, 0.5f - fLatitudeLowTextureCord };
			sTempVertex.sNorVec = { cosf(fLongitudeLow) * cosf(-fLatitudeLow), cosf(fLongitudeLow) * sinf(-fLatitudeLow), sinf(-fLatitudeLow) };
			vVertex.emplace_back(sTempVertex);

			sTempVertex.sPosVec = { fRadius * cosf(fLongitudeLow) * cosf(-fLatitudeHigh), fRadius * cosf(fLongitudeLow) * sinf(-fLatitudeHigh), fRadius * sinf(-fLatitudeHigh) };
			sTempVertex.sTexCoord = { fLongitudeTextureCord, 0.5f - fLatitudeHighTextureCord };
			sTempVertex.sNorVec = { cosf(fLongitudeLow) * cosf(-fLatitudeHigh), cosf(fLongitudeLow) * sinf(-fLatitudeHigh), sinf(-fLatitudeHigh) };
			vVertex.emplace_back(sTempVertex);
		}

		for (unsigned short longitudeIdx = 0; longitudeIdx < usLevel * 2; ++longitudeIdx)
		{
			const unsigned short& usOffSetIdx = 4 * longitudeIdx;
			vIndex.push_back(usOffSetIdx + 0);
			vIndex.push_back(usOffSetIdx + 1);
			vIndex.push_back(usOffSetIdx + 4);
			vIndex.push_back(usOffSetIdx + 4);
			vIndex.push_back(usOffSetIdx + 1);
			vIndex.push_back(usOffSetIdx + 5);

			vIndex.push_back(usOffSetIdx + 3);
			vIndex.push_back(usOffSetIdx + 2);
			vIndex.push_back(usOffSetIdx + 7);
			vIndex.push_back(usOffSetIdx + 7);
			vIndex.push_back(usOffSetIdx + 2);
			vIndex.push_back(usOffSetIdx + 6);
		}

	}

	ui32IndexCount = UINT(vIndex.size());

	sTransformationProperties.xmvTranslation.m128_f32[0] = fCenterX;
	sTransformationProperties.xmvTranslation.m128_f32[1] = fCenterY;
	sTransformationProperties.xmvTranslation.m128_f32[2] = fCenterZ;

	ID3D11Helper::CreateBuffer(cpDevice.Get(), vIndex, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, 0, cpIndexBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(cpDevice.Get(), vVertex, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, 0, cpVertexBuffer.GetAddressOf());
}

CubeModel::~CubeModel()
{
}
