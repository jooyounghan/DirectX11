#include "FilterInterface.h"
#include "ID3D11Helper.h"

using namespace std;
using namespace Microsoft::WRL;

FilterInfo::FilterInfo(
	ID3D11Device* pDeviceIn
)
{
	vFilterVertices = vector<FilterVertex>({
	{ {-1.f, 1.f, 0.f, 1.f}, {0.f, 0.f} },
	{ {1.f, 1.f, 0.f, 1.f}, {1.f, 0.f} },
	{ {1.f, -1.f, 0.f, 1.f}, {1.f, 1.f} },
	{ {-1.f, -1.f, 0.f, 1.f}, {0.f, 1.f} }
		});

	vIndices = vector<UINT>{ 0, 2, 3, 2, 0, 1 };

	ID3D11Helper::CreateBuffer(pDeviceIn, vFilterVertices, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpVertexBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(pDeviceIn, vIndices, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, NULL, NULL, cpIndexBuffer.GetAddressOf());

	ID3D11Helper::CreateVSInputLayOut(
		pDeviceIn,
		L"FilterVS.hlsl",
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
		},
		cpFilterVS.GetAddressOf(),
		cpFilterInputLayout.GetAddressOf()
	);

	ID3D11Helper::CreateSampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, NULL, pDeviceIn, cpPSSamplerState.GetAddressOf());
}

FilterInfo& FilterInfo::GetIncetance(ID3D11Device* pDeviceIn)
{
	static FilterInfo s(pDeviceIn);
	return s;
}

FilterInterface::FilterInterface(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn,
	const D3D11_VIEWPORT& sScreenViewportIn
)
	: pDevice(pDeviceIn), pDeviceContext(pDeviceContextIn), sScreenViewport(sScreenViewportIn)
{}
