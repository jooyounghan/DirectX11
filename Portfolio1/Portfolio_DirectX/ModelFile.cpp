#include "ModelFile.h"
#include "FileManipulator.h"
#include "DefineVar.h"

using namespace std;

ModelFile::ModelFile(
    const std::string& strFileLabelIn
)
	: IFile(strFileLabelIn)
{

}

ModelFile::~ModelFile() {}

void ModelFile::Initialize()
{
    DirectX::XMFLOAT3 vmin(1E9, 1E9, 1E9);
    DirectX::XMFLOAT3 vmax(-1E9, -1E9, -1E9);
    if (!bIsInitialized)
    {
        for (auto meshFile : vMeshFiles)
        {
            for (auto& v : meshFile->GetVerticesRef())
            {
                vmin.x = DirectX::XMMin(vmin.x, v.x);
                vmin.y = DirectX::XMMin(vmin.y, v.y);
                vmin.z = DirectX::XMMin(vmin.z, v.z);

                vmax.x = DirectX::XMMax(vmax.x, v.x);
                vmax.y = DirectX::XMMax(vmax.y, v.y);
                vmax.z = DirectX::XMMax(vmax.z, v.z);
            }

            float dx = vmax.x - vmin.x, dy = vmax.y - vmin.y, dz = vmax.z - vmin.z;
            float scale = 1.f / DirectX::XMMax(DirectX::XMMax(dx, dy), dz);

            DirectX::XMFLOAT3 translation;
            translation.x = -(vmin.x + vmax.x) * 0.5f;
            translation.y = -(vmin.y + vmax.y) * 0.5f;
            translation.z = -(vmin.z + vmax.z) * 0.5f;

            for (auto& v : meshFile->GetVerticesRef())
            {
                v.x = (v.x + translation.x) * scale;
                v.y = (v.y + translation.y) * scale;
                v.z = (v.z + translation.z) * scale;
            }
        }
        bIsInitialized = true;
    }
}

void ModelFile::AcceptFileAsList(FileManipulator* pFileManipulator)
{
	pFileManipulator->ShowAsList(*this, DRAG_DROP_MODEL_KEY);
}
