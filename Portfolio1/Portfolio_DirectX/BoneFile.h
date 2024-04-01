#pragma once
#include "IFile.h"
#include <DirectXMath.h>
#include <memory>
#include <vector>

struct BoneWeight
{
	unsigned int uiVertexId;
	float fWeight;
};

struct BoneData
{
	std::string strBoneName;
	unsigned int uiNumWeight;
	BoneWeight* pBoneWeights;
	BoneData* pBoneParent;
	std::vector<BoneData> vBoneChildren;
};

class BoneFile : public IFile, public std::enable_shared_from_this<BoneFile>
{
public:
	BoneFile(const std::string& strFileLabelIn, const size_t& uiBoneNumsIn);
	virtual ~BoneFile();

private:
	BoneData boneRoot;
	size_t uiBoneNums;

private:
	std::vector<DirectX::XMMATRIX> vBoneOffsetMatrix;

public:
	inline void SetOffsetMatrix(const size_t boneIdIdx, const DirectX::XMMATRIX& boneOffsetMatrixIn) { vBoneOffsetMatrix[boneIdIdx] = boneOffsetMatrixIn; }

public:
	BoneData& GetRootBone() { return boneRoot; }
	inline const size_t& GetBoneNums() { return uiBoneNums; }

private:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator) override;
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() override { return nullptr; }
};

