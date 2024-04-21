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
	BoneData* pBoneParent;
	std::vector<BoneData> vBoneChildren;
};

class BoneFile : public IFile, public std::enable_shared_from_this<BoneFile>
{
public:
	BoneFile(const std::string& strFileLabelIn);
	virtual ~BoneFile();

private:
	BoneData boneRoot;
	size_t uiBoneNums;

public:
	BoneData& GetRootBone() { return boneRoot; }
	inline const size_t& GetBoneNums() { return uiBoneNums; }
	inline void SetBoneNums(const size_t uiBoneNumsIn) { uiBoneNums = uiBoneNumsIn; }

private:
	std::vector<std::string> vBoneNames;
	std::vector<DirectX::XMMATRIX> vBoneOffsetMatrix;
	static std::string strDefaultBoneName;

public:
	inline const std::vector<std::string>& GetBoneNames() { return vBoneNames; }

public:
	size_t GetBoneIdx(const std::string& strBoneNameIn);
	void SetOffsetMatrix(
		const std::string& boneName,
		const size_t boneIdIdx,
		const DirectX::XMMATRIX& boneOffsetMatrixIn
	);
	inline const DirectX::XMMATRIX& GetOffsetMatrix(const size_t& offsetMatrixIdx) { return vBoneOffsetMatrix[offsetMatrixIdx]; }

private:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator) override;
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() override { return nullptr; }
};

