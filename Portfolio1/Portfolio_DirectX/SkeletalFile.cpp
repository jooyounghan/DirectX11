#include "SkeletalFile.h"
#include "FileManipulator.h"
#include "DefineVar.h"

SkeletalFile::SkeletalFile(
	const std::string& strFileLabelIn
)
	: IFile(strFileLabelIn), Bone("BaseRoot", nullptr)
{

}

SkeletalFile::~SkeletalFile()
{
}

void SkeletalFile::AcceptFileAsList(FileManipulator* pFileManipulator)
{
	pFileManipulator->ShowAsList(*this, DRAG_DROP_SKELETAL_KEY);
}