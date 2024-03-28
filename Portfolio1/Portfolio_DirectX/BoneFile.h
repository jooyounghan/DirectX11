#pragma once
#include "IFile.h"
#include "Bone.h"
#include <memory>
#include <unordered_map>

class BoneFile : public IFile, public Bone, public std::enable_shared_from_this<BoneFile>
{
public:
	BoneFile(const std::string& strFileLabelIn);
	virtual ~BoneFile();

private:
	std::unordered_map<std::string, const void*> unmapBoneInformation;

public:
	std::unordered_map<std::string, const void*>& GetBoneInformation() { return unmapBoneInformation; };

private:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator) override;
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() override { return nullptr; }
};

