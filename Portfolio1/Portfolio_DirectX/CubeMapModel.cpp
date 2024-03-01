#include "CubeMapModel.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"
#include "FileLoader.h"

#include <string>
using namespace std;

CubeMapModel::CubeMapModel(
	const float& fRadius
)
	: AIBLModel()
{
	xmvPosition.m128_f32[0] = 0.f;
	xmvPosition.m128_f32[1] = 0.f;
	xmvPosition.m128_f32[2] = 0.f;
	ScaleUp(fRadius - 1.f, fRadius - 1.f, fRadius - 1.f);

	spMeshFile = FileLoader::LoadDefaultCubeMesh(true);

	SetMeshName("IBL Cube Map Model" + to_string(sModelData.uiModelID));
}

CubeMapModel::~CubeMapModel()
{
}