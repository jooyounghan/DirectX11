#include "BoneFile.h"
#include "FileManipulator.h"
#include "DefineVar.h"
#include "DirectXDevice.h"
#include "ID3D11Helper.h"

BoneFile::BoneFile(
	const std::string& strFileLabelIn, const size_t& uiBoneNumsIn
)
	: IFile(strFileLabelIn), uiBoneNums(uiBoneNumsIn)
{
	vBoneOffsetMatrix.resize(uiBoneNums);
}

BoneFile::~BoneFile()
{
}


void BoneFile::AcceptFileAsList(FileManipulator* pFileManipulator)
{
	pFileManipulator->ShowAsList(*this, DRAG_DROP_BONE_KEY);
}