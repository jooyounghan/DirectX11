#include "FilterInterface.h"

using namespace std;
using namespace Microsoft::WRL;

FilterInterface::FilterInterface(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	ComPtr<ID3D11Texture2D>& cpInputTexture2DIn,
	const UINT& uiWidthIn, const UINT& uiHeightIn
)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn), cpInputTexture2D(cpInputTexture2DIn), uiWidth(uiWidthIn), uiHeight(uiHeightIn)
{
	vFilterVertices = vector<FilterVertex>({
		{ {-1.f, 1.f}, {0.f, 0.f} },
		{ {1.f, 1.f}, {1.f, 0.f} },
		{ {1.f, -1.f}, {1.f, 1.f} },
		{ {-1.f, -1.f}, {0.f, 1.f} } 
	});

	vIndices = vector<unsigned int>{ 0, 2, 3, 2, 0, 1 };

}
