#include "MeshFile.h"

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

MeshFile::MeshFile(
	const std::string& strFileLabelIn,
	const size_t uiMeshCountIn,
	const bool& bIsGLTFIn,
	const std::shared_ptr<BoneFile>& spBoneFileIn
)
	: IFile(strFileLabelIn), 
	bIsInitialized(false), bIsGLTF(bIsGLTFIn),
	spBoneFile(spBoneFileIn)
{
	vMeshData.resize(uiMeshCountIn);
}

void MeshFile::Initialize()
{
	DirectX::XMFLOAT3 vmin(1E9, 1E9, 1E9);
	DirectX::XMFLOAT3 vmax(-1E9, -1E9, -1E9);
	if (!bIsInitialized)
	{
		for (auto meshData : vMeshData)
		{
			for (auto& v : meshData.vVertices)
			{
				vmin.x = DirectX::XMMin(vmin.x, v.x);
				vmin.y = DirectX::XMMin(vmin.y, v.y);
				vmin.z = DirectX::XMMin(vmin.z, v.z);

				vmax.x = DirectX::XMMax(vmax.x, v.x);
				vmax.y = DirectX::XMMax(vmax.y, v.y);
				vmax.z = DirectX::XMMax(vmax.z, v.z);
			}

			float dx = vmax.x - vmin.x, dy = vmax.y - vmin.y, dz = vmax.z - vmin.z;
			float scale = 1.f / DirectX::XMMax(DirectX::XMMax(dx, dy), dz);

			DirectX::XMFLOAT3 translation;
			translation.x = -(vmin.x + vmax.x) * 0.5f;
			translation.y = -(vmin.y + vmax.y) * 0.5f;
			translation.z = -(vmin.z + vmax.z) * 0.5f;

			for (auto& v : meshData.vVertices)
			{
				v.x = (v.x + translation.x) * scale;
				v.y = (v.y + translation.y) * scale;
				v.z = (v.z + translation.z) * scale;
			}

			meshData.CreateBuffer();
		}
		UpdateTangents();
		bIsInitialized = true;
	}
}

void MeshFile::UpdateTangents()
{
	for (auto& meshData : vMeshData)
	{
		HRESULT hResult = DirectX::ComputeTangentFrame(
			meshData.vIndices.data(),
			meshData.vIndices.size() / 3,
			meshData.vVertices.data(),
			meshData.vNormals.data(),
			meshData.vTexcoords.data(),
			meshData.vVertices.size(),
			meshData.vTangents.data(),
			nullptr
		);
		if (FAILED(hResult)) { std::cout << "Computing Tangent Frame Failed" << std::endl; }
	}
}