#include "Bone.h"

using namespace std;

Bone::Bone(const std::string& strBoneNameIn, Bone* pBoneParentIn)
	: strBoneName(strBoneNameIn), pBoneParent(pBoneParentIn)
{

}

Bone::~Bone()
{
}
