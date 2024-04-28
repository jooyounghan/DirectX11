#pragma once

#include <DirectXMesh.h>
#include <vector>
#include <memory>
#include <iostream>
#include <concepts>

#include "IFile.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"
#include "FileLoader.h"
#include "BoneFile.h"
#include "MaterialFile.h"

class Mesh
{
public:
	Mesh() {};
	~Mesh() {};

public:
	std::vector<DirectX::XMFLOAT3>			vVertices;
	std::vector<DirectX::XMFLOAT2>			vTexcoords;
	std::vector<DirectX::XMFLOAT3>			vNormals;
	std::vector<DirectX::XMFLOAT3>			vTangents;
	std::vector<uint32_t>					vIndices;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpVerticesBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpTexcoordsBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpNormalsBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpTangentsBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpInicesBuffer;

	std::vector<DirectX::XMFLOAT4>			vBlendWeight1;
	std::vector<DirectX::XMFLOAT4>			vBlendWeight2;
	std::vector<DirectX::XMUINT4>			vBlendIndices1;
	std::vector<DirectX::XMUINT4>			vBlendIndices2;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpBlendWeight1Buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpBlendWeight2Buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpBlendIndices1Buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpBlendIndices2Buffer;

public:
	void CreateBuffer();
	void UpdateTangent();
};

class MeshFile : public IFile, public std::enable_shared_from_this<MeshFile>
{
public:
	MeshFile(
		const std::string& strFileLabelIn, 
		const size_t uiMeshCountIn, 
		const bool& bIsGLTFIn,
		const std::shared_ptr<BoneFile>& spBoneFileIn = nullptr
	);
	virtual ~MeshFile() {};

protected:
	bool bIsInitialized;
	bool bIsGLTF;

public:
	inline const bool& IsGLTF() { return bIsGLTF; }

protected:
	std::shared_ptr<BoneFile> spBoneFile;

public:
	inline bool IsSkeletal() { return spBoneFile.get() != nullptr; }
	inline BoneFile* GetBoneFile() { return spBoneFile.get(); }

protected:
	std::vector<Mesh> vMeshData;

protected:
	DirectX::XMMATRIX xmmNormalizedMatrix;

public:
	inline const DirectX::XMMATRIX& GetNormalizedMatrix() { return xmmNormalizedMatrix; }

public:
	void Initialize();

public:
	inline size_t GetMeshNums() { return vMeshData.size(); }
	inline Mesh& GetMeshData(const size_t& uiMeshIdx) { return vMeshData[uiMeshIdx]; }

private:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator) override;
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() override { return nullptr; }
};

