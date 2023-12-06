#include "RasterizationState.h"
#include "ID3D11Helper.h"

RasterizerState::RasterizerState(ID3D11Device* pDeviceIn, ID3D11DeviceContext* pDeviceContextIn)
	: pDeviceContext(pDeviceContextIn)
{
	ID3D11Helper::CreateRasterizerState(pDeviceIn, D3D11_FILL_MODE::D3D11_FILL_SOLID, D3D11_CULL_MODE::D3D11_CULL_BACK, true, cpSolidRS.GetAddressOf());
	ID3D11Helper::CreateRasterizerState(pDeviceIn, D3D11_FILL_MODE::D3D11_FILL_WIREFRAME, D3D11_CULL_MODE::D3D11_CULL_BACK, true, cpWireframeState.GetAddressOf());
	SetSolidRS();
}

void RasterizerState::SetSolidRS() { pDeviceContext->RSSetState(cpSolidRS.Get()); }
void RasterizerState::SetWireFrameRS() { pDeviceContext->RSSetState(cpWireframeState.Get()); }
