#pragma once
#include <DirectXMesh.h>

#include "MeshFile.h"
#include "ImageFile.h"

class ModelFile : public IFile, public std::enable_shared_from_this<ModelFile>
{
public:
	ModelFile(
		const std::string&	strFileLabelIn
	);
	virtual ~ModelFile();

protected:
	bool bIsInitialized;
	std::vector<std::shared_ptr<MeshFile>>	vMeshFiles;

public:
	inline void AddMeshFile(const std::shared_ptr<MeshFile>& meshFile) { vMeshFiles.push_back(meshFile); }
	inline const std::vector<std::shared_ptr<MeshFile>>& GetMeshFileSet() { return vMeshFiles; }

public:
	void Initialize();
	inline void UpdateModel() { for (auto& meshFile : vMeshFiles) meshFile->UpdateMesh();}

public:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator) override;
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() override { return nullptr; };
};