#include "MeshFile.h"
#include "DefineVar.h"
#include "FileManipulator.h"

using namespace std;
using namespace DirectX;

void Mesh::CreateBuffer()
{
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, vVertices, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpVerticesBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, vTexcoords, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpTexcoordsBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, vNormals, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpNormalsBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, vTangents, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpTangentsBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, vIndices, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, NULL, NULL, cpInicesBuffer.GetAddressOf());

	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, vBlendWeight1, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpBlendWeight1Buffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, vBlendWeight2, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpBlendWeight2Buffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, vBlendIndices1, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpBlendIndices1Buffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(DirectXDevice::pDevice, vBlendIndices2, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, NULL, NULL, cpBlendIndices2Buffer.GetAddressOf());
}

void Mesh::UpdateTangent()
{
	HRESULT hResult = ComputeTangentFrame(
		vIndices.data(),
		vIndices.size() / 3,
		vVertices.data(),
		vNormals.data(),
		vTexcoords.data(),
		vVertices.size(),
		vTangents.data(),
		nullptr
	);
	if (FAILED(hResult)) { std::cout << "Computing Tangent Frame Failed" << std::endl; }
}

MeshFile::MeshFile(
	const std::string& strFileLabelIn,
	const size_t uiMeshCountIn,
	const bool& bIsGLTFIn,
	const std::shared_ptr<BoneFile>& spBoneFileIn
)
	: IFile(strFileLabelIn), 
	bIsInitialized(false), bIsGLTF(bIsGLTFIn),
	spBoneFile(spBoneFileIn),
	xmmNormalizedMatrix(XMMatrixIdentity())
{
	for (size_t meshIdx = 0; meshIdx < uiMeshCountIn; ++meshIdx)
	{
		const string strTmpMaterial = "TempMaterial" + to_string(meshIdx + 1);
		vMaterials.push_back(make_shared<MaterialFile>(strTmpMaterial, bIsGLTFIn));
	}

	vMeshData.resize(uiMeshCountIn);
}

void MeshFile::Initialize()
{
	XMFLOAT3 vmin(1E9, 1E9, 1E9);
	XMFLOAT3 vmax(-1E9, -1E9, -1E9);
	if (!bIsInitialized)
	{
		for (auto& meshData : vMeshData)
		{
			for (auto& v : meshData.vVertices)
			{
				vmin.x = XMMin(vmin.x, v.x);
				vmin.y = XMMin(vmin.y, v.y);
				vmin.z = XMMin(vmin.z, v.z);

				vmax.x = XMMax(vmax.x, v.x);
				vmax.y = XMMax(vmax.y, v.y);
				vmax.z = XMMax(vmax.z, v.z);
			}
		}

		XMFLOAT3 translation;
		translation.x = -(vmin.x + vmax.x) * 0.5f;
		translation.y = -(vmin.y + vmax.y) * 0.5f;
		translation.z = -(vmin.z + vmax.z) * 0.5f;

		float dx = vmax.x - vmin.x, dy = vmax.y - vmin.y, dz = vmax.z - vmin.z;
		float scale = 1.f / XMMax(XMMax(dx, dy), dz);


		xmmNormalizedMatrix =
			XMMatrixTranslation(translation.x, translation.y, translation.z)
			* XMMatrixScaling(scale, scale, scale);

		for (auto& meshData : vMeshData)
		{
			for (auto& v : meshData.vVertices)
			{
				v.x = (v.x + translation.x) * scale;
				v.y = (v.y + translation.y) * scale;
				v.z = (v.z + translation.z) * scale;
			}
			meshData.UpdateTangent();
			meshData.CreateBuffer();
		}
		
		bIsInitialized = true;
	}
}

void MeshFile::AcceptFileAsList(FileManipulator* pFileManipulator)
{
	pFileManipulator->ShowAsList(*this, DRAG_DROP_MESH_KEY);
}