#include "AStaticMesh.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"
#include "ModelManipulator.h"
#include <DirectXMesh.h>

using namespace std;

AStaticMesh::AStaticMesh() 
	: IModel(), ATransformerable()
{

}

AStaticMesh::~AStaticMesh() {}

std::tuple<std::vector<UINT>, std::vector<UINT>, std::vector<ID3D11Buffer*>> AStaticMesh::GetInputInfo()
{
	std::vector<UINT> uiStrides = { sizeof(DirectX::XMFLOAT3), sizeof(DirectX::XMFLOAT2),sizeof(DirectX::XMFLOAT3),sizeof(DirectX::XMFLOAT3) };
	std::vector<UINT> uiOffsets = { 0, 0, 0, 0 };
	std::vector<ID3D11Buffer*> vertexBuffers = {
		cpVerticesBuffer.Get(),
		cpTexcoordsBuffer.Get(),
		cpNormalsBuffer.Get(),
		cpTangentsBuffer.Get()
	};
	return std::make_tuple(uiStrides, uiOffsets, vertexBuffers);
}

void AStaticMesh::Draw()
{
	DirectXDevice::pDeviceContext->DrawIndexed((UINT)spIndicesData.size(), NULL, NULL);
}

void AStaticMesh::UpdateModel(const float& fDelta)
{
	UpdateTranformationMatrix();
	ID3D11Helper::UpdateBuffer(DirectXDevice::pDeviceContext, sTransformation, D3D11_MAP_WRITE_DISCARD, cpTransformationBuffer.Get());
}

void AStaticMesh::AcceptModelManipulating(ModelManipulator* pModelManipulator)
{
	pModelManipulator->ManipulateModel(*this);
}

void AStaticMesh::ScaleUp(const float& fXup, const float& fYUp, const float& fZUp)
{
	sScales.fScaleX += fXup;
	sScales.fScaleY += fYUp;
	sScales.fScaleZ += fZUp;

	UpdateTranformationMatrix();
	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext, 
		sTransformation, 
		D3D11_MAP_WRITE_DISCARD, 
		cpTransformationBuffer.Get()
	);
}

void AStaticMesh::CreateCubeModel(
	std::vector<DirectX::XMFLOAT3>& inputVerticesIn,
	std::vector<DirectX::XMFLOAT2>& inputTexcoordsIn,
	std::vector<DirectX::XMFLOAT3>& inputNormalsIn,
	std::vector<DirectX::XMFLOAT3>&	inputTangentsIn,
	std::vector<uint32_t>& indexData,
	const float& fRadius, 
	const bool& bReverse, 
	const uint32_t& usLevel
)
{
	for (uint32_t latitudeIdx = 0; latitudeIdx < usLevel; ++latitudeIdx)
	{
		const float& fLatitudeLow = DirectX::XM_PIDIV2 / usLevel * latitudeIdx;
		const float& fLatitudeHigh = DirectX::XM_PIDIV2 / usLevel * (latitudeIdx + 1);
		const float& fLatitudeLowTextureCord = (latitudeIdx / (float)usLevel) / 2.f;
		const float& fLatitudeHighTextureCord = ((latitudeIdx + 1) / (float)usLevel) / 2.f;

		const uint32_t& usLatitudeOffset = (uint32_t)inputVerticesIn.size();

		for (uint32_t longitudeIdx = 0; longitudeIdx <= usLevel * 2; ++longitudeIdx)
		{
			const float& fLongitudeLow = DirectX::XM_2PI / (usLevel * 2) * longitudeIdx;
			const float& fLongitudeTextureCord = longitudeIdx / (float)(usLevel * 2);

			inputVerticesIn.emplace_back(fRadius * cosf(fLongitudeLow) * cosf(fLatitudeLow), fRadius * sinf(fLatitudeLow), fRadius * cosf(fLatitudeLow) * sinf(fLongitudeLow));
			inputTexcoordsIn.emplace_back(fLongitudeTextureCord, 0.5f + fLatitudeLowTextureCord);
			inputNormalsIn.emplace_back(cosf(fLongitudeLow) * cosf(fLatitudeLow), sinf(fLatitudeLow), cosf(fLatitudeLow) * sinf(fLongitudeLow));

			inputVerticesIn.emplace_back(fRadius * cosf(fLongitudeLow) * cosf(fLatitudeHigh), fRadius * sinf(fLatitudeHigh), fRadius * cosf(fLatitudeHigh) * sinf(fLongitudeLow));
			inputTexcoordsIn.emplace_back(fLongitudeTextureCord, 0.5f + fLatitudeHighTextureCord);
			inputNormalsIn.emplace_back(cosf(fLongitudeLow) * cosf(fLatitudeHigh), sinf(fLatitudeHigh), cosf(fLatitudeHigh) * sinf(fLongitudeLow));

			inputVerticesIn.emplace_back(fRadius * cosf(fLongitudeLow) * cosf(-fLatitudeLow), fRadius * sinf(-fLatitudeLow), fRadius * cosf(-fLatitudeLow) * sinf(fLongitudeLow));
			inputTexcoordsIn.emplace_back(fLongitudeTextureCord, 0.5f - fLatitudeLowTextureCord);
			inputNormalsIn.emplace_back(cosf(fLongitudeLow) * cosf(-fLatitudeLow), sinf(-fLatitudeLow), cosf(-fLatitudeLow) * sinf(fLongitudeLow));

			inputVerticesIn.emplace_back(fRadius * cosf(fLongitudeLow) * cosf(-fLatitudeHigh), fRadius * sinf(-fLatitudeHigh), fRadius * cosf(-fLatitudeHigh) * sinf(fLongitudeLow));
			inputTexcoordsIn.emplace_back(fLongitudeTextureCord, 0.5f - fLatitudeHighTextureCord);
			inputNormalsIn.emplace_back(cosf(fLongitudeLow) * cosf(-fLatitudeHigh), sinf(-fLatitudeHigh), cosf(-fLatitudeHigh) * sinf(fLongitudeLow));
		}

		for (uint32_t longitudeIdx = 0; longitudeIdx < usLevel * 2; ++longitudeIdx)
		{
			const uint32_t& usLongitudeOffset = 4 * longitudeIdx + usLatitudeOffset;
			indexData.push_back(usLongitudeOffset + 0);
			indexData.push_back(usLongitudeOffset + 1);
			indexData.push_back(usLongitudeOffset + 4);
			indexData.push_back(usLongitudeOffset + 4);
			indexData.push_back(usLongitudeOffset + 1);
			indexData.push_back(usLongitudeOffset + 5);

			indexData.push_back(usLongitudeOffset + 3);
			indexData.push_back(usLongitudeOffset + 2);
			indexData.push_back(usLongitudeOffset + 7);
			indexData.push_back(usLongitudeOffset + 7);
			indexData.push_back(usLongitudeOffset + 2);
			indexData.push_back(usLongitudeOffset + 6);
		}
	}

	inputTangentsIn.resize(inputNormalsIn.size());

	HRESULT hResult = DirectX::ComputeTangentFrame(
		indexData.data(),
		indexData.size() / 3,
		inputVerticesIn.data(),
		inputNormalsIn.data(),
		inputTexcoordsIn.data(),
		inputVerticesIn.size(),
		inputTangentsIn.data(),
		nullptr
	);

	if (bReverse)
	{
		std::reverse(indexData.begin(), indexData.end());
	}
}
