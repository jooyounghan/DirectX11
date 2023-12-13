#include "ModelInterface.h"
#include "ID3D11Helper.h"
#include <atomic>
#include <algorithm>

using namespace std;
using namespace DirectX;

ModelInterface::ModelInterface(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn
)
	: pDevice(pDeviceIn), pDeviceContext(pDeviceContextIn)
{
}

ModelInterface::~ModelInterface()
{

}

void ModelInterface::MakePlaneVertexIndexSet(
	ModelInterface* pModelInterface,
	const float& fWidth,
	const float& fHeight,
	const float& fWrapWidth,
	const float& fWrapHeight
)
{
	if (pModelInterface != nullptr)
	{
		vector<uint32_t> vIndex{
			0, 1, 2,
			0, 2, 3,
		};

		pModelInterface->ui32IndexCount = UINT(vIndex.size());

		vector<Vertex> vVertex{
			{{-(fWidth / 2.f), (fHeight / 2.f), 0.f}, {fWidth / fWrapWidth, 0.f}, {0.f, 0.f, 1.f, 0.f}},
			{{-(fWidth / 2.f), -(fHeight / 2.f), 0.f}, {fWidth / fWrapWidth, fHeight / fWrapHeight}, {0.f, 0.f, 1.f, 0.f}},
			{{(fWidth / 2.f), -(fHeight / 2.f), 0.f}, {0.f, fHeight / fWrapHeight}, {0.f, 0.f, 1.f, 0.f}},
			{{(fWidth / 2.f), (fHeight / 2.f), 0.f}, {0.f, 0.f}, {0.f, 0.f, 1.f, 0.f}}
		};

		ID3D11Helper::CreateBuffer(pModelInterface->pDevice, vIndex, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, 0, pModelInterface->cpIndexBuffer.GetAddressOf());
		ID3D11Helper::CreateBuffer(pModelInterface->pDevice, vVertex, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, 0, pModelInterface->cpVertexBuffer.GetAddressOf());
	}
}

void ModelInterface::MakeSquareVertexIndexSet(
	ModelInterface* pModelInterface,
	const float& fLen,
	const bool& bReverse
)
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

		ID3D11Helper::CreateBuffer(pModelInterface->pDevice, vIndex, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, 0, pModelInterface->cpIndexBuffer.GetAddressOf());
		ID3D11Helper::CreateBuffer(pModelInterface->pDevice, vVertex, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, 0, pModelInterface->cpVertexBuffer.GetAddressOf());
	}
}

void ModelInterface::MakeSphereVertexIndexSet(
	ModelInterface* pModelInterface,
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

		ID3D11Helper::CreateBuffer(pModelInterface->pDevice, vIndex, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, 0, pModelInterface->cpIndexBuffer.GetAddressOf());
		ID3D11Helper::CreateBuffer(pModelInterface->pDevice, vVertex, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, 0, pModelInterface->cpVertexBuffer.GetAddressOf());
	}
}
