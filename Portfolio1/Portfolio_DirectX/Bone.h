#pragma once
#include <vector>
#include <string>
#include <DirectXMath.h>

class Bone
{
public:
	Bone(
		const std::string& strBoneNameIn, 
		Bone* pBoneParentIn = nullptr
	);
	~Bone();

private:
	std::string strBoneName;

private:
	struct BoneWeight
	{
		unsigned int uiVertexId;
		float fWeight;
	};

private:
	unsigned int uiNumWeight;
	BoneWeight* pBoneWeights;
	DirectX::XMMATRIX	xmMatrix;

public:
	void SetWeights(const unsigned& uiNumWeightIn, void* pWeights);
	inline void SetBoneMatrix(DirectX::XMMATRIX&& xmMatirxIn) { xmMatrix = xmMatirxIn; }

private:
	Bone* pBoneParent;
	std::vector<Bone> vBoneChildren;

public:
	inline const std::string& GetBoneName() const { return strBoneName; }
	
public:
	inline bool	HasBoneChildren() const { return vBoneChildren.size() > 0; }
	inline const std::vector<Bone>& GetBoneChildren() const { return vBoneChildren; }

public:
	inline bool IsRootBone() { return pBoneParent == nullptr; }
	inline Bone* GetBoneParent() { return pBoneParent; }

public:
	inline void AddBoneChild(const std::string& strBoneNameIn) { vBoneChildren.emplace_back(strBoneNameIn, this); }
	Bone* GetLatestBoneChild() { return vBoneChildren.size() > 0 ? &vBoneChildren[vBoneChildren.size() - 1] : nullptr; }

public:
	int printtest = 0;
	int test = 0;
};

