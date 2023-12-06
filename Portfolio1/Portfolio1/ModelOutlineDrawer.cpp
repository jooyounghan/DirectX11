#include "ModelOutlineDrawer.h"

#include "ID3D11Helper.h"
#include "PickableModelInterface.h"

#include "DepthStencilState.h"

#include <vector>

using namespace std;

ModelOutlineDrawer::ModelOutlineDrawer(
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn
)
	: LightlessDrawer(pDeviceIn, pDeviceContextIn)
{
	vector<D3D11_INPUT_ELEMENT_DESC> vInputElemDesc{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	ID3D11Helper::CreateVSInputLayOut(pDevice, L"BaseModelVS.hlsl", vInputElemDesc, cpBaseVertexShader.GetAddressOf(), cpBaseInputLayout.GetAddressOf());
	ID3D11Helper::CreatePS(pDevice, L"OutlinerPS.hlsl", cpBasePixelShader.GetAddressOf());

}

ModelOutlineDrawer::~ModelOutlineDrawer()
{
}

void ModelOutlineDrawer::Draw(CameraInterface* pCamera, ModelInterface* pModel)
{
	PickableModelInterface* pPickable = dynamic_cast<PickableModelInterface*>(pModel);
	if (pPickable != nullptr)
	{
		pPickable->ScaleUp(0.1f, 0.1f, 0.1f);
		LightlessDrawer::Draw(pCamera, pModel);
		pPickable->ScaleUp(-0.1f, -0.1f, -0.1f);
	}
}

void ModelOutlineDrawer::SetIAInputLayer()
{
	pDeviceContext->IASetInputLayout(cpBaseInputLayout.Get());
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ModelOutlineDrawer::SetShader()
{
	pDeviceContext->VSSetShader(cpBaseVertexShader.Get(), 0, 0);
	pDeviceContext->PSSetShader(cpBasePixelShader.Get(), 0, 0);
}

void ModelOutlineDrawer::SetOMState()
{
	DepthStencilState& depthStencilState = DepthStencilState::GetInstance(pDevice);
	pDeviceContext->OMSetDepthStencilState(depthStencilState.pGetDSS(DepthStencilState::DrawNotEqualOption), 1);
}


void ModelOutlineDrawer::ResetDrawer()
{
	pDeviceContext->PSSetShader(nullptr, 0, 0);
	pDeviceContext->VSSetShader(nullptr, 0, 0);

	DepthStencilState& depthStencilState = DepthStencilState::GetInstance(pDevice);
	pDeviceContext->OMSetDepthStencilState(depthStencilState.pGetDSS(DepthStencilState::DefaultOption), 0);
}