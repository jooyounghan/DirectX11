#pragma once
#include "IFile.h"
#include <DirectXMath.h>
#include <memory>
#include <vector>
#include <unordered_map>

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

public:
	BoneData& GetRootBone() { return boneRoot; }
	inline const size_t& GetBoneNums() { return uiBoneNums; }

private:
	std::unordered_map<std::string, size_t> unmapBoneNameToIdx;
	std::vector<DirectX::XMMATRIX> vBoneOffsetMatrix;

public:
	inline size_t GetBoneIdxByName(const std::string& strBoneName) 
	{
		return unmapBoneNameToIdx.find(strBoneName) != unmapBoneNameToIdx.end() ?
			unmapBoneNameToIdx[strBoneName] : -1;
	}

	void SetOffsetMatrix(
		const std::string& boneName,
		const size_t boneIdIdx,
		const DirectX::XMMATRIX& boneOffsetMatrixIn
	);

private:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator) override;
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() override { return nullptr; }
};

