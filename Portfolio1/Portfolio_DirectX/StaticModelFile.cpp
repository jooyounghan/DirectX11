#include "StaticModelFile.h"
#include "FileManipulator.h"

StaticModelFile::StaticModelFile(const std::string& strFileLabelIn)
	: ModelFile(strFileLabelIn)
{
}

StaticModelFile::~StaticModelFile()
{
}

void StaticModelFile::AcceptFileAsList(FileManipulator* pFileManipulator)
{
	pFileManipulator->ShowAsList(*this, DRAG_DROP_STATIC_MODEL_KEY);
}
