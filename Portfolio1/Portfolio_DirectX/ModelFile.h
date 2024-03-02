#pragma once
#include <DirectXMesh.h>
#include <vector>

#include "MeshFile.h"
#include "ImageFile.h"

struct MeshFileSet
{
	std::shared_ptr<class MeshFile>		spMeshFile;
	std::shared_ptr<IImageFile>			spModelTexture[TEXTURE_MAP_NUM];
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
	std::vector<MeshFileSet>	vMeshFileSets;

protected:
	bool bIsGltf;

public:
	inline void AddMeshFileSet(const MeshFileSet& meshFileSet) { vMeshFileSets.push_back(meshFileSet); }
	inline const std::vector<MeshFileSet>& GetMeshFileSet() { return vMeshFileSets; }

public:
	inline const bool& IsGLTF() { return bIsGltf; }
	inline void SetIsGLTF(const bool& bIsGltfIn) { bIsGltf = bIsGltfIn; }

public:
	inline void SetThumbNailFile(const std::shared_ptr<IImageFile>& thumbNailFileIn) { thumbNailFile = thumbNailFileIn; }

public:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator) override;
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() override;
};