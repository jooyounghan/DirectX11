#include "ModelFile.h"
#include "FileManipulator.h"

#include <filesystem>
#include <iostream>

using namespace std;

MeshData::MeshData()
{
}

MeshData::~MeshData()
{
}

void MeshData::UpdateTangents()
{
	if (
		spIndices != nullptr &&
		spVertices != nullptr &&
		spNormals != nullptr &&
		spTangents != nullptr
		)
	{
		HRESULT hResult = DirectX::ComputeTangentFrame(
			spIndices->data(),
			spIndices->size() / 3,
			spVertices->data(),
			spNormals->data(),
			spTexcoords->data(),
			spVertices->size(),
			spTangents->data(),
			nullptr
		);

		if (FAILED(hResult)) { cout << "Computing Tangent Frame Failed" << endl; }
	};
}


ModelFile::ModelFile(
	const std::string& strFilePathIn,
	const std::string& strFileNameIn,
	const bool& bIsGltfIn
)
	: IFile(strFilePathIn, strFileNameIn), bIsGltf(bIsGltfIn)
{

}

ModelFile::~ModelFile() {}

void ModelFile::AcceptFileAsList(FileManipulator* pFileManipulator, std::shared_ptr<IFile>& spFile)
{
	pFileManipulator->ShowAsList(*this, spFile);
}

ID3D11ShaderResourceView* ModelFile::GetThumbNailSRV()
{
	return thumbNailFile->GetThumbNailSRV();
}
