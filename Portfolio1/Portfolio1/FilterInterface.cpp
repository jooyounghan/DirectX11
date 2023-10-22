#include "FilterInterface.h"
#include "ID3D11Helper.h"

using namespace std;
using namespace Microsoft::WRL;

FilterInfo::FilterInfo(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn)
	: cpDevice(cpDeviceIn)
{
	vFilterVertices = vector<FilterVertex>({
	{ {-1.f, 1.f}, {0.f, 0.f} },
	{ {1.f, 1.f}, {1.f, 0.f} },
	{ {1.f, -1.f}, {1.f, 1.f} },
	{ {-1.f, -1.f}, {0.f, 1.f} }
		});

	vIndices = vector<unsigned int>{ 0, 2, 3, 2, 0, 1 };

	ID3D11Helper::CreateBuffer(cpDevice.Get(), vFilterVertices, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpVertexBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(cpDevice.Get(), vIndices, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, NULL, NULL, cpIndexBuffer.GetAddressOf());

	ID3D11Helper::CreateSampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, NULL, cpDevice.Get(), cpPSSamplerState.GetAddressOf());
}

FilterInfo& FilterInfo::GetIncetance(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn)
{
	static FilterInfo s(cpDeviceIn);
	return s;
}

FilterInterface::FilterInterface(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn
)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn), uiWidth(uiWidthIn), uiHeight(uiHeightIn)
{
}


