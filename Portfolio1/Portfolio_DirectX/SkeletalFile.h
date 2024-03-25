#pragma once
#include "IFile.h"
#include "Bone.h"
#include <memory>
#include <set>

class SkeletalFile : public IFile, public Bone, public std::enable_shared_from_this<SkeletalFile>
{
public:
	SkeletalFile(const std::string& strFileLabelIn);
	virtual ~SkeletalFile();

private:
	std::set<std::string> setBoneInformation;

public:
	std::set<std::string>& GetBoneInformation() { return setBoneInformation; };

private:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator) override;
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() override { return nullptr; }
};

