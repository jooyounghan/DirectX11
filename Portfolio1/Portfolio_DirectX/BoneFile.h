#pragma once
#include "IFile.h"
#include "Bone.h"
#include <memory>
#include <set>

class BoneFile : public IFile, public Bone, public std::enable_shared_from_this<BoneFile>
{
public:
	BoneFile(const std::string& strFileLabelIn);
	virtual ~BoneFile();

private:
	std::set<std::string> setBoneInformation;

public:
	std::set<std::string>& GetBoneInformation() { return setBoneInformation; };

private:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator) override;
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() override { return nullptr; }
};

