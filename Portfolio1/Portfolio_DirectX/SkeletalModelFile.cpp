#include "SkeletalModelFile.h"
#include "FileManipulator.h"

using namespace std;

SkeletalModelFile::SkeletalModelFile(const std::string& strFileLabelIn)
	: ModelFile(strFileLabelIn)
{
}

SkeletalModelFile::~SkeletalModelFile()
{
}

void SkeletalModelFile::AcceptFileAsList(FileManipulator* pFileManipulator)
{
	pFileManipulator->ShowAsList(*this, DRAG_DROP_SKELETAL_MODEL_KEY);
}
