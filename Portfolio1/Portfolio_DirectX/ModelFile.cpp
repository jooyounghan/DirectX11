#include "ModelFile.h"
#include "FileManipulator.h"

using namespace std;

ModelFile::ModelFile(
	const std::string& strFilePathIn,
	const std::string& strFileNameIn,
	const bool& bIsGltfIn
)
	: IFile(strFilePathIn, strFileNameIn), bIsGltf(bIsGltfIn)
{

}

ModelFile::~ModelFile() {}

void ModelFile::AcceptFileAsList(FileManipulator* pFileManipulator)
{
	pFileManipulator->ShowAsList(*this);
}

ID3D11ShaderResourceView* ModelFile::GetThumbNailSRV()
{
	return thumbNailFile->GetThumbNailSRV();
}

