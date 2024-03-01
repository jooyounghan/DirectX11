#pragma once
#include <DirectXMesh.h>
#include <vector>

#include "MeshFile.h"
#include "ImageFile.h"

struct PBRModelTextureSet
{
	std::shared_ptr<IImageFile>			spModelTexture[TEXTURE_MAP_NUM];
};

struct NodeData
{
	std::vector<NodeData>							vChildrenNodes;
	std::vector<std::shared_ptr<class MeshFile>>	vChildrenMeshes;
	std::vector<PBRModelTextureSet>					vChildrenTextureSets;
};

class ModelFile : public IFile, public std::enable_shared_from_this<ModelFile>
{
public:
	ModelFile(
		const std::string& strFilePathIn,
		const std::string& strFileNameIn,
		const bool&			bIsGltfIn
	);
	virtual ~ModelFile();

protected:
	std::shared_ptr<IImageFile> thumbNailFile;

protected:
	NodeData rootNode;
	bool bIsGltf;

public:
	inline NodeData& GetRootNode() { return rootNode; }

public:
	inline const bool& IsGLTF() { return bIsGltf; }
	inline void SetIsGLTF(const bool& bIsGltfIn) { bIsGltf = bIsGltfIn; }

public:
	inline void SetThumbNailFile(const std::shared_ptr<IImageFile>& thumbNailFileIn) { thumbNailFile = thumbNailFileIn; }

public:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator) override;
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() override;
};