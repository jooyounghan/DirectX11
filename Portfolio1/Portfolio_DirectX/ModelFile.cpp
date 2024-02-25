#include "ModelFile.h"
#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>

#include <filesystem>
#include <iostream>

using namespace std;

MeshData::MeshData()
{
}

MeshData::~MeshData()
{
}


ModelFile::ModelFile(
	const std::string& strFilePathIn,
	const std::string& strFileNameIn
)
	: IFile(strFilePathIn, strFileNameIn)
{
	string extenstion = filesystem::path(strFileName).extension().string();
	transform(extenstion.begin(), extenstion.end(), extenstion.begin(), ::tolower);
	extenstion == ".gltf" ? bIsGltf = true : bIsGltf = false;

	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(
		strFilePath + strFileName, 
		aiProcess_Triangulate | aiProcess_ConvertToLeftHanded
	);

	if (pScene)
	{
		DirectX::XMMATRIX xmmTransform;
        ProcessNode(pScene->mRootNode, pScene, xmmTransform);
        UpdateTangents();
    }
    else {
        //std::cout << "Failed to read file: " << m_basePath + filename
        //    << std::endl;
        //auto errorDescription = importer.GetErrorString();
        //std::cout << "Assimp error: " << errorDescription << endl;
    }
}

ModelFile::~ModelFile() {}

void ModelFile::ProcessNode(aiNode* pNode, const aiScene* pScene, DirectX::XMMATRIX& xmMatrix)
{
}

MeshData ModelFile::ProcessMesh(aiNode* pNode, const aiScene* pScene)
{
    return MeshData();
}

void ModelFile::UpdateTangents()
{
	for (auto& mesh : vMeshData)
	{

	}
}

