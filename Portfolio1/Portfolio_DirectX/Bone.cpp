#include "Bone.h"

using namespace std;

Bone::Bone(const std::string& strBoneNameIn, Bone* pBoneParentIn)
	: strBoneName(strBoneNameIn), pBoneParent(pBoneParentIn)
{

}

Bone::~Bone()
{
}

void Bone::SetWeights(const unsigned& uiNumWeightIn, void* pWeights)
{
	uiNumWeight = uiNumWeightIn;
	pBoneWeights = (BoneWeight*)malloc(sizeof(BoneWeight) * uiNumWeight);
	memcpy(pBoneWeights, pWeights, sizeof(BoneWeight) * uiNumWeight);
}
