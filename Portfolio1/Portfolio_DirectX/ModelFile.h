#pragma once

#include "IFile.h"
#include <DirectXMesh.h>
#include <vector>

struct NodeData
{
	std::vector<NodeData> vChildrenNodes;
	std::vector<class MeshData> vChildrenMeshes;
};

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
	void UpdateTangents();

public:
	std::shared_ptr<IFile> spModelTexture[TEXTURE_MAP_NUM];

public:
	std::string				strMeshName;
};


class ModelFile : public IFile
{
public:
	ModelFile(
		const std::string& strFilePathIn,
		const std::string& strFileNameIn,
		const bool&			bIsGltfIn
	);
	virtual ~ModelFile();

protected:
	std::shared_ptr<IFile> thumbNailFile;

protected:
	NodeData rootNode;
	bool bIsGltf;

public:
	inline NodeData& GetRootNode() { return rootNode; }

public:
	inline const bool& IsGLTF() { return bIsGltf; }
	inline void SetIsGLTF(const bool& bIsGltfIn) { bIsGltf = bIsGltfIn; }

public:
	inline void SetThumbNailFile(const std::shared_ptr<IFile>& thumbNailFileIn) { thumbNailFile = thumbNailFileIn; }

public:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator, std::shared_ptr<IFile>& spFile) override;
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() override;
};