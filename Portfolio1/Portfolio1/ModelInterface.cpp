#include "ModelInterface.h"
#include "TransformProperties.h"
#include "ID3D11Helper.h"
#include <atomic>
#include <algorithm>

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

void ModelInterface::MakeSquareVertexIndexSet(ModelInterface* pModelInterface, const float& fCenterX, const float& fCenterY, const float& fCenterZ, const float& fLen, const bool& bReverse)
{
	if (pModelInterface != nullptr)
	{
		vector<uint32_t> vIndex{
			0, 1, 2,
			3, 4, 5,

			6, 7, 8,
			9, 10, 11,

			12, 13, 14,
			15, 16, 17,

			18, 19, 20,
			21, 22, 23,

			24, 25, 26,
			27, 28, 29,

			30, 31, 32,
			33, 34, 35,
		};

		pModelInterface->ui32IndexCount = UINT(vIndex.size());

		pModelInterface->upTransformationProperties->xmvTranslation.m128_f32[0] = fCenterX;
		pModelInterface->upTransformationProperties->xmvTranslation.m128_f32[1] = fCenterY;
		pModelInterface->upTransformationProperties->xmvTranslation.m128_f32[2] = fCenterZ;

		vector<Vertex> vVertex{
			{{-fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {0.f, 1.f}, {0.f, 0.f, -1.f, 0.f}},
			{{fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {1.f, 0.f}, {0.f, 0.f, -1.f, 0.f}},
			{{fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {1.f, 1.f}, {0.f, 0.f, -1.f, 0.f}},
			{{-fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {0.f, 1.f}, {0.f, 0.f, -1.f, 0.f}},
			{{-fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {0.f, 0.f}, {0.f, 0.f, -1.f, 0.f}},
			{{fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {1.f, 0.f}, {0.f, 0.f, -1.f, 0.f}},


			{{fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {0.f, 1.f}, {1.f, 0.f, 0.f, 0.f}},
			{{fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {0.f, 0.f}, {1.f, 0.f, 0.f, 0.f}},
			{{fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {1.f, 1.f}, {1.f, 0.f, 0.f, 0.f}},
			{{fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {1.f, 1.f}, {1.f, 0.f, 0.f, 0.f}},
			{{fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {0.f, 0.f}, {1.f, 0.f, 0.f, 0.f}},
			{{fLen / 2.f, fLen / 2.f, fLen / 2.f}, {1.f, 0.f}, {1.f, 0.f, 0.f, 0.f}},


			{{-fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {1.f, 1.f}, {-1.f, 0.f, 0.f, 0.f}},
			{{-fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {0.f, 1.f}, {-1.f, 0.f, 0.f, 0.f}},
			{{-fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {1.f, 0.f}, {-1.f, 0.f, 0.f, 0.f}},
			{{-fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {0.f, 1.f}, {-1.f, 0.f, 0.f, 0.f}},
			{{-fLen / 2.f, fLen / 2.f, fLen / 2.f}, {0.f, 0.f}, {-1.f, 0.f, 0.f, 0.f}},
			{{-fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {1.f, 0.f}, {-1.f, 0.f, 0.f, 0.f}},


			{{-fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {0.f, 0.f}, {0.f, -1.f, 0.f, 0.f}},
			{{fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {1.f, 0.f}, {0.f, -1.f, 0.f, 0.f}},
			{{fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {1.f, 1.f}, {0.f, -1.f, 0.f, 0.f}},
			{{-fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {0.f, 0.f}, {0.f, -1.f, 0.f, 0.f}},
			{{fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {1.f, 1.f}, {0.f, -1.f, 0.f, 0.f}},
			{{-fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {0.f, 1.f}, {0.f, -1.f, 0.f, 0.f}},


			{{-fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {0.f, 1.f}, {0.f, 1.f, 0.f, 0.f}},
			{{-fLen / 2.f, fLen / 2.f, fLen / 2.f}, {0.f, 0.f}, {0.f, 1.f, 0.f, 0.f}},
			{{fLen / 2.f, fLen / 2.f, fLen / 2.f}, {1.f, 0.f}, {0.f, 1.f, 0.f, 0.f}},
			{{-fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {0.f, 1.f}, {0.f, 1.f, 0.f, 0.f}},
			{{fLen / 2.f, fLen / 2.f, fLen / 2.f}, {1.f, 0.f}, {0.f, 1.f, 0.f, 0.f}},
			{{fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {1.f, 1.f}, {0.f, 1.f, 0.f, 0.f}},


			{{-fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {1.f, 1.f}, {0.f, 0.f, 1.f, 0.f}},
			{{fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {0.f, 1.f}, {0.f, 0.f, 1.f, 0.f}},
			{{-fLen / 2.f, fLen / 2.f, fLen / 2.f}, {1.f, 0.f}, {0.f, 0.f, 1.f, 0.f}},
			{{fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {0.f, 1.f}, {0.f, 0.f, 1.f, 0.f}},
			{{fLen / 2.f, fLen / 2.f, fLen / 2.f}, {0.f, 0.f}, {0.f, 0.f, 1.f, 0.f}},
			{{-fLen / 2.f, fLen / 2.f, fLen / 2.f}, {1.f, 0.f}, {0.f, 0.f, 1.f, 0.f}}
		};


		if (bReverse)
		{
			reverse(vIndex.begin(), vIndex.end());
		}

		ID3D11Helper::CreateBuffer(pModelInterface->cpDevice.Get(), vIndex, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, 0, pModelInterface->cpIndexBuffer.GetAddressOf());
		ID3D11Helper::CreateBuffer(pModelInterface->cpDevice.Get(), vVertex, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, 0, pModelInterface->cpVertexBuffer.GetAddressOf());
	}
}

void ModelInterface::MakeSphereVertexIndexSet(
	ModelInterface* pModelInterface,
	const float& fCenterX,
	const float& fCenterY,
	const float& fCenterZ,
	const float& fRadius,
	const unsigned short& usLevel
)
{
	if (pModelInterface != nullptr)
	{
		vector<uint32_t> vIndex;
		vector<Vertex> vVertex;

		for (unsigned short latitudeIdx = 0; latitudeIdx < usLevel; ++latitudeIdx)
		{
			const float& fLatitudeLow = DirectX::XM_PIDIV2 / usLevel * latitudeIdx;
			const float& fLatitudeHigh = DirectX::XM_PIDIV2 / usLevel * (latitudeIdx + 1);
			const float& fLatitudeLowTextureCord = (latitudeIdx / (float)usLevel) / 2.f;
			const float& fLatitudeHighTextureCord = ((latitudeIdx + 1) / (float)usLevel) / 2.f;

			const unsigned short& usLatitudeOffset = (unsigned short)vVertex.size();

			for (unsigned short longitudeIdx = 0; longitudeIdx <= usLevel * 2; ++longitudeIdx)
			{
				Vertex sTempVertex;

				const float& fLongitudeLow = DirectX::XM_2PI / (usLevel * 2) * longitudeIdx;
				const float& fLongitudeTextureCord = longitudeIdx / (float)(usLevel * 2);

				sTempVertex.sPosVec = { fRadius * cosf(fLongitudeLow) * cosf(fLatitudeLow), fRadius * sinf(fLatitudeLow), fRadius * cosf(fLatitudeLow) * sinf(fLongitudeLow) };
				sTempVertex.sTexCoord = { fLongitudeTextureCord, 0.5f + fLatitudeLowTextureCord };
				sTempVertex.sNorVec = { cosf(fLongitudeLow) * cosf(fLatitudeLow), sinf(fLatitudeLow), cosf(fLatitudeLow) * sinf(fLongitudeLow) };
				vVertex.emplace_back(sTempVertex);

				sTempVertex.sPosVec = { fRadius * cosf(fLongitudeLow) * cosf(fLatitudeHigh), fRadius * sinf(fLatitudeHigh), fRadius * cosf(fLatitudeHigh) * sinf(fLongitudeLow) };
				sTempVertex.sTexCoord = { fLongitudeTextureCord, 0.5f + fLatitudeHighTextureCord };
				sTempVertex.sNorVec = { cosf(fLongitudeLow) * cosf(fLatitudeHigh), sinf(fLatitudeHigh), cosf(fLatitudeHigh) * sinf(fLongitudeLow) };
				vVertex.emplace_back(sTempVertex);

				sTempVertex.sPosVec = { fRadius * cosf(fLongitudeLow) * cosf(-fLatitudeLow), fRadius * sinf(-fLatitudeLow), fRadius * cosf(-fLatitudeLow) * sinf(fLongitudeLow) };
				sTempVertex.sTexCoord = { fLongitudeTextureCord, 0.5f - fLatitudeLowTextureCord };
				sTempVertex.sNorVec = { cosf(fLongitudeLow) * cosf(-fLatitudeLow), sinf(-fLatitudeLow), cosf(-fLatitudeLow) * sinf(fLongitudeLow) };
				vVertex.emplace_back(sTempVertex);

				sTempVertex.sPosVec = { fRadius * cosf(fLongitudeLow) * cosf(-fLatitudeHigh), fRadius * sinf(-fLatitudeHigh), fRadius * cosf(-fLatitudeHigh) * sinf(fLongitudeLow) };
				sTempVertex.sTexCoord = { fLongitudeTextureCord, 0.5f - fLatitudeHighTextureCord };
				sTempVertex.sNorVec = { cosf(fLongitudeLow) * cosf(-fLatitudeHigh), sinf(-fLatitudeHigh), cosf(-fLatitudeHigh) * sinf(fLongitudeLow) };
				vVertex.emplace_back(sTempVertex);
			}

			for (unsigned short longitudeIdx = 0; longitudeIdx < usLevel * 2; ++longitudeIdx)
			{
				const unsigned short& usLongitudeOffset = 4 * longitudeIdx + usLatitudeOffset;
				vIndex.push_back(usLongitudeOffset + 0);
				vIndex.push_back(usLongitudeOffset + 1);
				vIndex.push_back(usLongitudeOffset + 4);
				vIndex.push_back(usLongitudeOffset + 4);
				vIndex.push_back(usLongitudeOffset + 1);
				vIndex.push_back(usLongitudeOffset + 5);

				vIndex.push_back(usLongitudeOffset + 3);
				vIndex.push_back(usLongitudeOffset + 2);
				vIndex.push_back(usLongitudeOffset + 7);
				vIndex.push_back(usLongitudeOffset + 7);
				vIndex.push_back(usLongitudeOffset + 2);
				vIndex.push_back(usLongitudeOffset + 6);
			}

		}

		pModelInterface->ui32IndexCount = UINT(vIndex.size());

		pModelInterface->upTransformationProperties->xmvTranslation.m128_f32[0] = fCenterX;
		pModelInterface->upTransformationProperties->xmvTranslation.m128_f32[1] = fCenterY;
		pModelInterface->upTransformationProperties->xmvTranslation.m128_f32[2] = fCenterZ;

		ID3D11Helper::CreateBuffer(pModelInterface->cpDevice.Get(), vIndex, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, 0, pModelInterface->cpIndexBuffer.GetAddressOf());
		ID3D11Helper::CreateBuffer(pModelInterface->cpDevice.Get(), vVertex, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, 0, pModelInterface->cpVertexBuffer.GetAddressOf());
	}
}
