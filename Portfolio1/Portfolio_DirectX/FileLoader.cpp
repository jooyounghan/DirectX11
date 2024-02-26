#define __STDC_LIB_EXT1__
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "FileLoader.h"
#include "Console.h"

#include "NormalImageFile.h"
#include "DDSImageFile.h"
#include "ModelFile.h"

#include "stb_image.h"
#include "stb_image_write.h"

#include <DirectXTexEXR.h>
#include <directxtk/DDSTextureLoader.h>
#include <fp16.h>
#include <atlconv.h>

#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>

#include <iostream>
#include <fstream>

using namespace std;

std::unordered_map<std::string, std::weak_ptr<class IFile>> FileLoader::mapUsingFiles;

uint8_t* FileLoader::LoadFileWithStbi(const char* sFilename, UINT* x, UINT* y, UINT* comp)
{
    FILE* f;
    fopen_s(&f, sFilename, "rb");
    uint8_t* result;
    if (!f) return stbi__errpuc("can't fopen", "Unable to open file");
    result = stbi_load_from_file(f, (int*)x, (int*)y, (int*)comp, 4);
    fclose(f);
    return result;
}

uint8_t* FileLoader::LoadFileWithStbi(const wchar_t* wFilename, UINT* x, UINT* y, UINT* comp)
{
    FILE* f;
    _wfopen_s(&f, wFilename, L"rb");
    uint8_t* result;
    if (!f) return stbi__errpuc("can't fopen", "Unable to open file");
    result = stbi_load_from_file(f, (int*)x, (int*)y, (int*)comp, 4);
    fclose(f);
    return result;
}

void FileLoader::FreeLoadedFileData(uint8_t* pRawData)
{
    if (pRawData != nullptr)
    {
        STBI_FREE(pRawData);
        pRawData = nullptr;
    }
}

uint8_t* FileLoader::LoadFileWithOpenEXR(const char* pFileName, UINT* x, UINT* y, UINT* comp)
{
    return LoadFileWithOpenEXR(FileLoader::ConvertUTF8ToUniCode(string(pFileName)).c_str(), x, y, comp);
}

uint8_t* FileLoader::LoadFileWithOpenEXR(const wchar_t* pFileName, UINT* x, UINT* y, UINT* comp)
{
    uint8_t* result = nullptr;
    DirectX::ScratchImage scratch;
    DirectX::TexMetadata metaData;
    HRESULT hResult = LoadFromEXRFile(pFileName, &metaData, scratch);
    if (FAILED(hResult))
    {
        Console::AssertPrint("EXR 파일을 불러들이는데 실패하였습니다.");
    }
    else
    {
        *x = (UINT)metaData.width;
        *y = (UINT)metaData.height;
        const DirectX::Image* pImage = scratch.GetImage(NULL, NULL, NULL);
        *comp = pImage->rowPitch / (pImage->width * 2);

        result = (uint8_t*)malloc(scratch.GetPixelsSize());
        if (result != nullptr)
        {
            memcpy(result, scratch.GetPixels(), scratch.GetPixelsSize());
        }
    }
    return result;
}

void FileLoader::AddUsingFile(const string& strFileName, std::shared_ptr<IFile> spFile)
{
    if (FileLoader::mapUsingFiles.find(strFileName) == FileLoader::mapUsingFiles.end())
    {
        FileLoader::mapUsingFiles.emplace(strFileName, spFile);
    }
    else
    {
        FileLoader::mapUsingFiles[strFileName] = spFile;
    }
}

std::shared_ptr<IFile> FileLoader::GetUsingFile(const string& strFileName) {
    std::shared_ptr<IFile> result = nullptr;
    if (FileLoader::mapUsingFiles.find(strFileName) != FileLoader::mapUsingFiles.end())
    {
        if (!FileLoader::mapUsingFiles[strFileName].expired())
        {
            result = FileLoader::mapUsingFiles[strFileName].lock();
        }
        else
        {
            FileLoader::mapUsingFiles.erase(strFileName);
        }
    }
    else
    {

    }
    return result;
}

std::shared_ptr<IFile> FileLoader::LoadImageFile(
    const string& strExtension, 
    const string& strFilePath,
    const string& strFileName,
    UINT* x, UINT* y, UINT* comp
)
{
    std::shared_ptr<IFile> result = FileLoader::GetUsingFile(strFileName);
    uint8_t* ucImageRawData = nullptr;
    if (result.get() == nullptr)
    {
        if (strExtension == ".exr")
        {
            ucImageRawData = FileLoader::LoadFileWithOpenEXR((strFilePath + "\\" + strFileName).c_str(), x, y, comp);
            result = make_shared<NormalImageFile>(*x, *y, DXGI_FORMAT_R16G16B16A16_FLOAT, ucImageRawData, strFilePath, strFileName);
        }
        else if (strExtension == ".dds")
        {
            result = make_shared<DDSImageFile>(strFilePath, strFileName, strFilePath.find("Brdf") == string::npos);
        }
        else
        {
            ucImageRawData = FileLoader::LoadFileWithStbi((strFilePath + "\\" + strFileName).c_str(), x, y, comp);
            result = make_shared<NormalImageFile>(*x, *y, DXGI_FORMAT_R8G8B8A8_UNORM, ucImageRawData, strFilePath, strFileName);
        }
        FileLoader::AddUsingFile(strFileName, result);
    }
    FileLoader::FreeLoadedFileData(ucImageRawData);
    return result;
}

std::shared_ptr<class IFile> FileLoader::LoadModelFile(
    const std::string& strExtension, 
    const std::string& strFilePath, 
    const std::string& strFileName
)
{
    std::shared_ptr<IFile> result = FileLoader::GetUsingFile(strFileName);
    if (result.get() == nullptr)
    {
        strModelFilePath = strFilePath;
        bool bIsGltf = false;
        strExtension == ".gltf" ? bIsGltf = true : bIsGltf = false;

        std::shared_ptr<ModelFile> modelFile = std::make_shared<ModelFile>(strFilePath, strFileName, bIsGltf);

        Assimp::Importer importer;
        std::vector<MeshData>& vTempMeshes = modelFile->GetMeshDataRef();

        const aiScene* pScene = importer.ReadFile(
            strFilePath + "\\" + strFileName,
            aiProcess_Triangulate | aiProcess_ConvertToLeftHanded
        );

        if (pScene)
        {
            DirectX::XMMATRIX xmmTransform;
            ProcessNode(bIsGltf, pScene->mRootNode, pScene, xmmTransform, vTempMeshes);
        }
        else {
            //std::cout << "Failed to read file: " << m_basePath + filename
            //    << std::endl;
            //auto errorDescription = importer.GetErrorString();
            //std::cout << "Assimp error: " << errorDescription << endl;
        }
        strModelFilePath.clear();
        result = modelFile;
        FileLoader::AddUsingFile(strFileName, result);
    }
    return result;
}

void FileLoader::ProcessNode(
    const bool& bIsGltf,
    aiNode* pNode, 
    const aiScene* pScene, 
    DirectX::XMMATRIX& xmMatrix,
    vector<MeshData>& vMeshes
)
{
    DirectX::XMMATRIX m(&pNode->mTransformation.a1);
    m = DirectX::XMMatrixTranspose(m) * xmMatrix;

    for (UINT idx = 0; idx < pNode->mNumMeshes; ++idx)
    {
        aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[idx]];
        MeshData newMesh = ProcessMesh(bIsGltf, pMesh, pScene);

        vector<DirectX::XMFLOAT3>& vertices = *newMesh.spVertices.get();
        for (DirectX::XMFLOAT3& v : vertices)
        {
            DirectX::XMVECTOR f4Vector = DirectX::XMVectorSet(v.x, v.y, v.z, 0.f);
            f4Vector = DirectX::XMVector4Transform(f4Vector, m);
            memcpy(&v, f4Vector.m128_f32, sizeof(v));
        }
        vMeshes.push_back(newMesh);
    }

    for (UINT i = 0; i < pNode->mNumChildren; i++) {
        ProcessNode(bIsGltf, pNode->mChildren[i], pScene, m, vMeshes);
    }
}

MeshData FileLoader::ProcessMesh(
    const bool& bIsGltf,
    aiMesh* pMesh, 
    const aiScene* pScene
)
{
    MeshData meshData;
    meshData.spVertices = make_shared<vector<DirectX::XMFLOAT3>>();
    meshData.spTexcoords = make_shared<vector<DirectX::XMFLOAT2>>();
    meshData.spNormals = make_shared<vector<DirectX::XMFLOAT3>>();
    meshData.spTangents = make_shared<vector<DirectX::XMFLOAT3>>();
    meshData.spIndices = make_shared<vector<uint32_t>>();

    // Walk through each of the mesh's vertices
    for (UINT i = 0; i < pMesh->mNumVertices; i++) 
    {   
        meshData.spVertices->emplace_back(
            pMesh->mVertices[i].x,
            pMesh->mVertices[i].y,
            pMesh->mVertices[i].z
        );

        if (pMesh->mTextureCoords[0]) {
            meshData.spTexcoords->emplace_back(
                (float)pMesh->mTextureCoords[0][i].x,
                (float)pMesh->mTextureCoords[0][i].y
            );
        }

        bIsGltf ? meshData.spNormals->emplace_back(
            pMesh->mNormals[i].x, pMesh->mNormals[i].z, -pMesh->mNormals[i].y
        ) : meshData.spNormals->emplace_back(
            pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z
        );
    }

    for (UINT i = 0; i < pMesh->mNumFaces; i++) {
        aiFace face = pMesh->mFaces[i];
        for (UINT j = 0; j < face.mNumIndices; j++)
            meshData.spIndices->emplace_back(face.mIndices[j]);
    }

    if (pMesh->mMaterialIndex >= 0) {

        aiMaterial* material = pScene->mMaterials[pMesh->mMaterialIndex];
        string test = ReadTextureFileName(pScene, material, aiTextureType_BASE_COLOR);

        //newMesh.albedoTextureFilename =
        //    ReadTextureFilename(scene, material, aiTextureType_BASE_COLOR);
        //if (newMesh.albedoTextureFilename.empty()) {
        //    newMesh.albedoTextureFilename =
        //        ReadTextureFilename(scene, material, aiTextureType_DIFFUSE);
        //}
        //newMesh.emissiveTextureFilename =
        //    ReadTextureFilename(scene, material, aiTextureType_EMISSIVE);
        //newMesh.heightTextureFilename =
        //    ReadTextureFilename(scene, material, aiTextureType_HEIGHT);
        //newMesh.normalTextureFilename =
        //    ReadTextureFilename(scene, material, aiTextureType_NORMALS);
        //newMesh.metallicTextureFilename =
        //    ReadTextureFilename(scene, material, aiTextureType_METALNESS);
        //newMesh.roughnessTextureFilename = ReadTextureFilename(
        //    scene, material, aiTextureType_DIFFUSE_ROUGHNESS);
        //newMesh.aoTextureFilename = ReadTextureFilename(
        //    scene, material, aiTextureType_AMBIENT_OCCLUSION);
    }

    return meshData;

}

std::string FileLoader::ReadTextureFileName(
    const aiScene* pScene,
    aiMaterial* pMaterial,
    aiTextureType eType
)
{
    string fullPath;
    if (pMaterial->GetTextureCount(eType) > 0) {
        aiString filepath;
        pMaterial->GetTexture(eType, 0, &filepath);
        fullPath = strModelFilePath + "\\" + filepath.C_Str();

        if (!filesystem::exists(fullPath)) {
            const aiTexture* texture =
                pScene->GetEmbeddedTexture(filepath.C_Str());
            if (texture) {
                if (string(texture->achFormatHint).find("png") !=
                    string::npos) {
                    ofstream fs(fullPath.c_str(), ios::binary | ios::out);
                    fs.write((char*)texture->pcData, texture->mWidth);
                    fs.close();
                }
            }
        }
    }
    return fullPath;
}

wstring FileLoader::GetLastDirectoryName(const filesystem::path& filePath)
{
    const wstring& wstrPathName = filePath.native();
    wstring::size_type pathNameStart = wstrPathName.find_last_of(L"\\");
    return wstrPathName.substr(pathNameStart + 1);
}

std::wstring FileLoader::ConvertUTF8ToUniCode(const std::string& str)
{
    int iUnicodeSize = ::MultiByteToWideChar(
        CP_UTF8,
        MB_ERR_INVALID_CHARS,
        str.c_str(), static_cast<int>(str.size()),
        nullptr, 0
    );

    wstring result;
    result.resize(iUnicodeSize);

    ::MultiByteToWideChar(
        CP_UTF8,
        MB_ERR_INVALID_CHARS,
        str.c_str(), static_cast<int>(str.size()),
        const_cast<wchar_t*>(result.c_str()), static_cast<int>(result.size())
    );

    return result;
}

std::string FileLoader::ConvertUniCodeToUTF8(const std::wstring& wStr)
{
    int iUTF8Size = ::WideCharToMultiByte(
        CP_UTF8,
        MB_ERR_INVALID_CHARS,
        wStr.c_str(), lstrlenW(wStr.c_str()),
        NULL, 0, NULL, NULL
    );

    string result;
    result.resize(iUTF8Size);

    ::WideCharToMultiByte(
        CP_UTF8,
        MB_ERR_INVALID_CHARS,
        wStr.c_str(), lstrlenW(wStr.c_str()),
        const_cast<char*>(result.c_str()), static_cast<int>(result.size()),
        NULL, NULL
    );

    return result;
}
