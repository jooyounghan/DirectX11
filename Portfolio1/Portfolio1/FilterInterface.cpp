#include "FilterInterface.h"
#include "ID3D11Helper.h"

using namespace std;
using namespace Microsoft::WRL;

FilterInfo::FilterInfo(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn
)
{
	vFilterVertices = vector<FilterVertex>({
	{ {-1.f, 1.f, 0.f, 1.f}, {0.f, 0.f} },
	{ {1.f, 1.f, 0.f, 1.f}, {1.f, 0.f} },
	{ {1.f, -1.f, 0.f, 1.f}, {1.f, 1.f} },
	{ {-1.f, -1.f, 0.f, 1.f}, {0.f, 1.f} }
		});

	vIndices = vector<unsigned int>{ 0, 2, 3, 2, 0, 1 };

	ID3D11Helper::CreateBuffer(cpDeviceIn.Get(), vFilterVertices, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpVertexBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(cpDeviceIn.Get(), vIndices, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, NULL, NULL, cpIndexBuffer.GetAddressOf());

	ID3D11Helper::CreateSampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, NULL, cpDeviceIn.Get(), cpPSSamplerState.GetAddressOf());
}

FilterInfo& FilterInfo::GetIncetance(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn)
{
	static FilterInfo s(cpDeviceIn);
	return s;
}

FilterInterface::FilterInterface(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const D3D11_VIEWPORT& sScreenViewportIn
)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn), sScreenViewport(sScreenViewportIn)
{}
