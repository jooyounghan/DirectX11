#pragma once
#include "ModelFile.h"
#include "BoneFile.h"

#include <memory>

class SkeletalModelFile : public ModelFile, public std::enable_shared_from_this<SkeletalModelFile>
{
public:
	SkeletalModelFile(const std::string& strFileLabelIn);
	virtual ~SkeletalModelFile();

protected:
	std::shared_ptr<BoneFile> spBoneFile;

public:
	inline void SetBoneFile(const std::shared_ptr<BoneFile> spBoneFileIn) { spBoneFile = spBoneFileIn; }
	inline std::shared_ptr<BoneFile>& GetBoneFile() { return spBoneFile; }

public:
	virtual void AcceptFileAsList(FileManipulator* pFileManipulator) override;
};

