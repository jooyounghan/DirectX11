#pragma once
#include <DirectXMesh.h>

#include "MeshFile.h"
#include "ImageFile.h"

class ModelFile : public IFile
{
public:
	ModelFile(const std::string& strFileLabelIn);
	virtual ~ModelFile();

protected:
	bool bIsInitialized;
	std::vector<std::shared_ptr<MeshFile>>	vMeshFiles;

public:
	inline void AddMeshFile(const std::shared_ptr<MeshFile>& meshFile) { vMeshFiles.emplace_back(meshFile); }
	inline const std::vector<std::shared_ptr<MeshFile>>& GetMeshFileSet() { return vMeshFiles; }

public:
	void Initialize();

public:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator) = 0;
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() override { return nullptr; };
};