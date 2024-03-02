#pragma once
#include "IFile.h"
#include <DirectXMesh.h>
#include <vector>

class MeshFile : public IFile, public std::enable_shared_from_this<MeshFile>
{
public:
	MeshFile(
		const std::string& strFilePathIn,
		const std::string& strFileNameIn
	);
	virtual ~MeshFile();

public:
	std::vector<DirectX::XMFLOAT3>		vVertices;
	std::vector<DirectX::XMFLOAT2>		vTexcoords;
	std::vector<DirectX::XMFLOAT3>		vNormals;
	std::vector<DirectX::XMFLOAT3>		vTangents;
	std::vector<uint32_t>				vIndices;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpVerticesBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpTexcoordsBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpNormalsBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpTangentsBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpInicesBuffer;


protected:
	bool bIsGltf;

public:
	inline const bool& IsGLTF() { return bIsGltf; }
	inline void SetIsGLTF(const bool& bIsGltfIn) { bIsGltf = bIsGltfIn; }

public:
	void CreateBuffers();
	void UpdateTangents();

private:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator) override {};
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() override { return nullptr; }
};
