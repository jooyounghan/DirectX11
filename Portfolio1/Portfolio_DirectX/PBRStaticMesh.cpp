#include "PBRStaticMesh.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "ModelManipulator.h"
#include "ModelRenderer.h"
#include "LightRenderer.h"
#include "NormalVectorRenderer.h"

using namespace std;

uint32_t PBRStaticMesh::uiPBRStaticMeshlIdx = 1;

PBRStaticMesh::PBRStaticMesh(
	const std::shared_ptr<MeshFile>& spMeshFileIn
)
	: IMesh(spMeshFileIn), ATransformerable(),
	IMovable(0.f, 0.f, 0.f),
	IScalable(),
	IAngleAdjustable(0.f, 0.f, 0.f)
{
	AutoZeroMemory(sPBRConstant);

	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, sPBRConstant, D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, NULL,
		cpPBRConstantBuffer.GetAddressOf()
	);
	const size_t meshNums = spMeshFile->GetMeshNums();
	const bool& isGltf = spMeshFile->IsGLTF();

	SetObjectName(spMeshFile->GetFileLabel() + to_string(uiPBRStaticMeshlIdx));
	uiPBRStaticMeshlIdx++;
}

PBRStaticMesh::~PBRStaticMesh()
{
}

void PBRStaticMesh::UpdateModel(const float& fDelta)
{
	ATransformerable::UpdatePosition();
	ATransformerable::UpdateTranformationMatrix();
	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sPBRConstant, D3D11_MAP_WRITE_DISCARD,
		cpPBRConstantBuffer.Get()
	);
}

void PBRStaticMesh::AcceptModelAsList(ModelManipulator* pModelManipulator)
{
	pModelManipulator->SetModelAsList(*this);
}

void PBRStaticMesh::AcceptModelManipulating(ModelManipulator* pModelManipulator)
{
	pModelManipulator->ManipulateModel(*this);
}

void PBRStaticMesh::AcceptModelRendering(ModelRenderer* pModelRenderer)
{
	pModelRenderer->RenderModel(*this);
}

void PBRStaticMesh::AcceptNormalVectorRendering(NormalVectorRenderer* pNormalVectorRenderer)
{
	pNormalVectorRenderer->RenderNormal(*this);
}

void PBRStaticMesh::AcceptRenderingLightMap(LightRenderer* pLightRnederer)
{
	pLightRnederer->RenderLightMap(*this);
}
