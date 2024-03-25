#include "MirrorModel.h"

#include "ModelManipulator.h"
#include "ModelRenderer.h"
#include "NormalVectorRenderer.h"
#include "LightRenderer.h"

#include "MeshFile.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"
#include "FileLoader.h"

using namespace std;

uint32_t MirrorModel::uiMirrorModelIdx = 1;

MirrorModel::MirrorModel(
	const float& fMirrorWidthIn,
	const float& fMirrorHeightIn,
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fPitchDegIn,
	const float& fYawDegIn,
	const float& fRollDegIn,
	const float& fFovDegreeIn,
	const float& fNearZIn,
	const float& fFarZIn
)
	: 
	fMirrorWidth(fMirrorWidthIn),
	fMirrorHeight(fMirrorHeightIn),
	IMesh(FileLoader::LoadPlaneMesh(fMirrorWidthIn, fMirrorHeightIn)),
	ViewableRenderTarget(
		fXPos, fYPos, fZPos,
		fPitchDegIn, fYawDegIn, fRollDegIn,
		fFovDegreeIn,
		fNearZIn, fFarZIn,
		(UINT)(fMirrorWidthIn * gMirrorTextureHeight / fMirrorHeightIn) , (UINT)gMirrorTextureHeight,
		0, DXGI_FORMAT_R8G8B8A8_UNORM
	),
	ViewableDepthStencil(
		fXPos, fYPos, fZPos,
		fPitchDegIn, fYawDegIn, fRollDegIn,
		fFovDegreeIn,
		fNearZIn, fFarZIn,
		(UINT)(fMirrorWidthIn* gMirrorTextureHeight / fMirrorHeightIn), (UINT)gMirrorTextureHeight,
		0, DXGI_FORMAT_D32_FLOAT
	),
	Viewable(
		fXPos, fYPos, fZPos,
		fPitchDegIn, fYawDegIn, fRollDegIn,
		(UINT)(fMirrorWidthIn* gMirrorTextureHeight / fMirrorHeightIn), (UINT)gMirrorTextureHeight,
		fFovDegreeIn, fNearZIn, fFarZIn
	),
	IRectangle((UINT)(fMirrorWidthIn* gMirrorTextureHeight / fMirrorHeightIn), (UINT)gMirrorTextureHeight),
	IMovable(fXPos, fYPos, fZPos),
	IAngleAdjustable(fPitchDegIn, fYawDegIn, fRollDegIn)
{
	AutoZeroMemory(sMirrorProperties);
	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice,
		sMirrorProperties, D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		NULL, cpMirrorProperties.GetAddressOf()
	);

	SetObjectName("Mirror Model " + to_string(uiMirrorModelIdx));
	uiMirrorModelIdx++;

}

MirrorModel::~MirrorModel()
{
}

void MirrorModel::Draw()
{
	DirectXDevice::pDeviceContext->DrawIndexed((UINT)spMeshFile->vIndices.size(), NULL, NULL);
}

void MirrorModel::SetAsRenderTarget()
{
	DirectXDevice::pDeviceContext->OMSetRenderTargets(1, cpRTV.GetAddressOf(), cpDSV.Get());
	DirectXDevice::pDeviceContext->RSSetViewports(1, &sViewPort);
}

void MirrorModel::ResetAsRenderTarget()
{
	DirectXDevice::pDeviceContext->OMSetRenderTargets(1, &pNullRTV, nullptr);
	DirectXDevice::pDeviceContext->RSSetViewports(1, &nullViewPort);
}

void MirrorModel::UpdateModel(const float& fDelta)
{
	UpdateTranformationMatrix();
	Viewable::UpdateViewToPerspective();
	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sMirrorProperties, D3D11_MAP_WRITE_DISCARD,
		cpMirrorProperties.Get()
	);
}

void MirrorModel::AcceptModelAsList(ModelManipulator* pModelManipulator)
{
	pModelManipulator->SetModelAsList(*this);
}

void MirrorModel::AcceptModelManipulating(ModelManipulator* pModelManipulator)
{
	pModelManipulator->ManipulateModel(*this);
}

void MirrorModel::AcceptModelRendering(ModelRenderer* pModelRenderer)
{
	pModelRenderer->RenderModel(*this);
}

void MirrorModel::AcceptNormalVectorRendering(NormalVectorRenderer* pNormalVectorRenderer)
{
	pNormalVectorRenderer->RenderNormal(*this);
}

void MirrorModel::AcceptRenderingLightMap(LightRenderer* pLightRenderer)
{
	pLightRenderer->RenderLightMap(*this);
}
