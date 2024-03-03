#include "CubeMapModel.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"
#include "FileLoader.h"

using namespace std;

uint32_t CubeMapModel::uiCubeMapModelIdx = 1;

CubeMapModel::CubeMapModel(
	const float& fRadius
)
	: AIBLMesh()
{
	spMeshFile = FileLoader::LoadDefaultCubeMesh(true);
	SetPosition(0.f, 0.f, 0.f);
	ScaleUp(fRadius - 1.f, fRadius - 1.f, fRadius - 1.f);
	UpdateTranformationMatrix();
	
	SetMeshName("IBL Cube Map Model" + to_string(uiCubeMapModelIdx));
	uiCubeMapModelIdx++;
}

CubeMapModel::~CubeMapModel()
{
}