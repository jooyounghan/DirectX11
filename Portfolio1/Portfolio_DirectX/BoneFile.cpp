#include "BoneFile.h"
#include "FileManipulator.h"
#include "DefineVar.h"

BoneFile::BoneFile(
	const std::string& strFileLabelIn
)
	: IFile(strFileLabelIn), Bone("BaseRoot", nullptr)
{

}

BoneFile::~BoneFile()
{
}

void BoneFile::AcceptFileAsList(FileManipulator* pFileManipulator)
{
	pFileManipulator->ShowAsList(*this, DRAG_DROP_BONE_KEY);
}