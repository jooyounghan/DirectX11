#pragma once

#include <DirectXMesh.h>
#include <vector>
#include <memory>

#include "MaterialFile.h"

class MeshFile : public IFile, public std::enable_shared_from_this<MeshFile>
{
	friend class AIBLMesh;
	friend class MirrorModel;

	friend class ModelRenderVertexShader;
	friend class NormalVectorVertexShader;
	friend class BasicVertexShader;

	friend class FileLoader;

public:
	MeshFile(
		const std::string& strFileLabelIn
	);
	virtual ~MeshFile();

protected:
	std::vector<DirectX::XMFLOAT3>		vVertices;
	std::vector<DirectX::XMFLOAT2>		vTexcoords;
	std::vector<DirectX::XMFLOAT3>		vNormals;
	std::vector<DirectX::XMFLOAT3>		vTangents;
	std::vector<uint32_t>				vIndices;

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpVerticesBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpTexcoordsBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpNormalsBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpTangentsBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpInicesBuffer;

public:
	inline std::vector<DirectX::XMFLOAT3>& GetVerticesRef() { return vVertices; }

public:
	inline UINT GetIndicesCount() { return (UINT)vIndices.size(); }

public:
	void CreateBuffers();

public:
	void UpdateTangents();

protected:
	std::shared_ptr<MaterialFile> spMaterial;

public:
	void SetMaterial(const std::shared_ptr<MaterialFile>& spMaterialIn) { spMaterial = spMaterialIn; }
	std::shared_ptr<MaterialFile> GetMaterial() { return spMaterial; }

private:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator) override {};
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() override { return nullptr; }
};
