#pragma once

#include "IFile.h"
#include "NormalImageFile.h"
#include <DirectXMath.h>
#include <vector>

class MeshData
{
public:
	MeshData();
	~MeshData();

public:
	std::shared_ptr<std::vector<DirectX::XMFLOAT3>>	spVertices;
	std::shared_ptr<std::vector<DirectX::XMFLOAT2>>	spTexcoords;
	std::shared_ptr<std::vector<DirectX::XMFLOAT3>>	spNormals;
	std::shared_ptr<std::vector<DirectX::XMFLOAT3>>	spTangents;
	std::shared_ptr<std::vector<uint32_t>> spIndices;

public:
	string strModelTexture[TEXTURE_MAP_NUM];
};


class ModelFile : public IFile
{
public:
	ModelFile(
		const std::string& strFilePathIn,
		const std::string& strFileNameIn
	);
	virtual ~ModelFile();

public:
	std::vector<MeshData> vMeshData;

public:
	bool bIsGltf;

private:
	void ProcessNode(
		struct aiNode* pNode,
		const struct aiScene* pScene,
		DirectX::XMMATRIX& xmMatrix
	);

	MeshData ProcessMesh(
		struct aiNode* pNode,
		const struct aiScene* pScene
	);

private:
	void UpdateTangents();
};