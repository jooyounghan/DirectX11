#include "BoneFile.h"
#include "FileManipulator.h"
#include "DefineVar.h"
#include "DirectXDevice.h"
#include "ID3D11Helper.h"

using namespace std;
using namespace DirectX;

string BoneFile::strDefaultBoneName = "";

BoneFile::BoneFile(const string& strFileLabelIn)
	: IFile(strFileLabelIn)
{

}

BoneFile::~BoneFile()
{
}


size_t BoneFile::GetBoneIdx(const std::string& strBoneNameIn)
{
	for (size_t idx = 0; idx < vBoneNames.size(); ++idx)
	{
		if (vBoneNames[idx] == strBoneNameIn)
			return idx;
	}
	return -1;
}

void BoneFile::SetOffsetMatrix(
	const string& boneName,
	const size_t boneIdIdx,
	const XMMATRIX& boneOffsetMatrixIn
)
{
	if (vBoneNames.size() > boneIdIdx)
	{
		vBoneNames.insert(vBoneNames.begin() + boneIdIdx, boneName);
	}
	else
	{
		vBoneNames.emplace_back(boneName);
	}

	if (vBoneOffsetMatrix.size() > boneIdIdx)
	{
		vBoneOffsetMatrix.insert(vBoneOffsetMatrix.begin() + boneIdIdx, boneOffsetMatrixIn);
	}
	else
	{
		vBoneOffsetMatrix.emplace_back(boneOffsetMatrixIn);
	}
}

void BoneFile::AdjustOffsetMatrix(const XMMATRIX& xmmAdjust)
{
	for (XMMATRIX& boneOffsetMatrix : vBoneOffsetMatrix)
	{
		boneOffsetMatrix = xmmAdjust * boneOffsetMatrix;
	}
}

void BoneFile::AcceptFileAsList(FileManipulator* pFileManipulator)
{
	pFileManipulator->ShowAsList(*this, DRAG_DROP_BONE_KEY);
}