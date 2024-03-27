#pragma once
#include "ModelFile.h"

#include <memory>

class StaticModelFile : public ModelFile, public std::enable_shared_from_this<StaticModelFile>
{
public:
	StaticModelFile(const std::string& strFileLabelIn);
	virtual ~StaticModelFile();

public:
	virtual void AcceptFileAsList(FileManipulator* pFileManipulator) override;
};

