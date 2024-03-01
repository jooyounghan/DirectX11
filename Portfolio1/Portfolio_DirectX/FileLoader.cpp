#define __STDC_LIB_EXT1__
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "FileLoader.h"
#include "Console.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "NormalImageFile.h"
#include "DDSImageFile.h"
#include "ModelFile.h"

#include "DefineVar.h"

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

unordered_map<string, weak_ptr<class IFile>> FileLoader::mapUsingFiles;

uint8_t* FileLoader::LoadFileWithStbi(const char* sFilename, UINT* x, UINT* y, UINT* comp)
{
    FILE* f;
    fopen_s(&f, sFilename, "rb");
    uint8_t* result = nullptr;
    if (!f)
    {
        result = stbi__errpuc("can't fopen", "Unable to open file");
    }
    else
    {
        result = stbi_load_from_file(f, (int*)x, (int*)y, (int*)comp, 4);
        fclose(f);
    }
    return result;
}

uint8_t* FileLoader::LoadFileWithStbi(const wchar_t* wFilename, UINT* x, UINT* y, UINT* comp)
{
    FILE* f;
    _wfopen_s(&f, wFilename, L"rb");
    uint8_t* result;
    if (!f) 
    {
        result = stbi__errpuc("can't fopen", "Unable to open file");
    }
    else
    {
        result = stbi_load_from_file(f, (int*)x, (int*)y, (int*)comp, 4);
        fclose(f);
    }
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
        Console::AssertPrint("Loading EXR File Failed.");
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

void FileLoader::AddUsingFile(const string& strFileNameWithExt, shared_ptr<IFile> spFile)
{
    if (FileLoader::mapUsingFiles.find(strFileNameWithExt) == FileLoader::mapUsingFiles.end())
    {
        FileLoader::mapUsingFiles.emplace(strFileNameWithExt, spFile);
    }
    else
    {
        FileLoader::mapUsingFiles[strFileNameWithExt] = spFile;
    }
}

shared_ptr<IFile> FileLoader::GetUsingFile(const string& strFileNameWithExt) {
    shared_ptr<IFile> result = nullptr;
    if (FileLoader::mapUsingFiles.find(strFileNameWithExt) != FileLoader::mapUsingFiles.end())
    {
        if (!FileLoader::mapUsingFiles[strFileNameWithExt].expired())
        {
            result = FileLoader::mapUsingFiles[strFileNameWithExt].lock();
        }
        else
        {
            FileLoader::mapUsingFiles.erase(strFileNameWithExt);
        }
    }
    else
    {

    }
    return result;
}

shared_ptr<IImageFile> FileLoader::LoadImageFile(
    const string& strFilePath,
    const string& strFileName,
    const string& strExtension, 
    UINT* x, UINT* y, UINT* comp
)
{
    const string fileNameWithExt = strFileName + strExtension;
    const string fullPath = strFilePath + "\\" + fileNameWithExt;

    shared_ptr<IImageFile> result;

    IImageFile* pImg = (IImageFile*)FileLoader::GetUsingFile(fileNameWithExt).get();
    uint8_t* ucImageRawData = nullptr;

    if (pImg == nullptr)
    {
        cout << "Loading " << fileNameWithExt << "..." << endl;
        if (strExtension == ".exr")
        {
            ucImageRawData = FileLoader::LoadFileWithOpenEXR(fullPath.c_str(), x, y, comp);
            result = make_shared<NormalImageFile>(*x, *y, DXGI_FORMAT_R16G16B16A16_FLOAT, ucImageRawData, strFilePath, fileNameWithExt);
        }
        else if (strExtension == ".dds")
        {
            result = make_shared<DDSImageFile>(strFilePath, fileNameWithExt, strFilePath.find("Brdf") == string::npos);
        }
        else
        {
            ucImageRawData = FileLoader::LoadFileWithStbi(fullPath.c_str(), x, y, comp);
            result = make_shared<NormalImageFile>(*x, *y, DXGI_FORMAT_R8G8B8A8_UNORM, ucImageRawData, strFilePath, fileNameWithExt);
        }
        FileLoader::AddUsingFile(fileNameWithExt, result);
    }
    else
    {
        result = pImg->shared_from_this();
    }
    FileLoader::FreeLoadedFileData(ucImageRawData);
    return result;
}

shared_ptr<ModelFile> FileLoader::LoadModelFile(
    const string& strFilePath, 
    const string& strFileName,
    const string& strExtension
)
{
    const string fileNameWithExt = strFileName + strExtension;
    const string fullPath = strFilePath + "\\" + fileNameWithExt;

    shared_ptr<ModelFile> result;
    
    ModelFile* pModel = (ModelFile*)FileLoader::GetUsingFile(fileNameWithExt).get();

    if (pModel == nullptr)
    {
        cout << "Loading " << fileNameWithExt << " Set..." << endl;
        bool bIsGltf = false;
        strExtension == ".gltf" ? bIsGltf = true : bIsGltf = false;

        result = make_shared<ModelFile>(strFilePath, fileNameWithExt, bIsGltf);

        const string thumbNailPath = strFilePath + "\\" + strFileName + ".png";

        UINT thumbNailX, thumbNailY, thumbNailComp;
        result->SetThumbNailFile(LoadImageFile(strFilePath, strFileName, ".png", &thumbNailX, &thumbNailY, &thumbNailComp));

        // ¸ðµ¨ ÀÐ±â
        Assimp::Importer importer;
        NodeData& rootNode = result->GetRootNode();

        const aiScene* pScene = importer.ReadFile(
            fullPath,
            aiProcess_Triangulate | aiProcess_ConvertToLeftHanded
        );

        if (pScene)
        {
            DirectX::XMMATRIX xmmTransform;
            ProcessNode(strFilePath, strFileName, strExtension, bIsGltf, pScene->mRootNode, pScene, xmmTransform, rootNode);
        }
        else 
        {
            cout << "Loading " << fullPath << " Failed" << endl;
            cout << "Assimp Importer Error Code : " << importer.GetErrorString() << endl;
        }
        FileLoader::AddUsingFile(fileNameWithExt, result);
    }
    else
    {
        result = pModel->shared_from_this();
    }
    return result;
}

void FileLoader::ProcessNode(
    const std::string& strFilePath,
    const std::string& strFileName,
    const std::string& strExtension,
    const bool& bIsGltf,
    aiNode* pNode, 
    const aiScene* pScene, 
    DirectX::XMMATRIX& xmMatrix,
    NodeData& parentNode
)
{
    DirectX::XMMATRIX m(&pNode->mTransformation.a1);
    m = DirectX::XMMatrixTranspose(m) * xmMatrix;

    for (UINT idx = 0; idx < pNode->mNumMeshes; ++idx)
    {
        aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[idx]];
        shared_ptr<MeshFile> newMesh = ProcessMesh(strFilePath, strFileName, strExtension, bIsGltf, pMesh, pScene, parentNode);

        vector<DirectX::XMFLOAT3>& vertices = newMesh->vVertices;
        for (DirectX::XMFLOAT3& v : vertices)
        {
            DirectX::XMVECTOR f4Vector = DirectX::XMVectorSet(v.x, v.y, v.z, 0.f);
            f4Vector = DirectX::XMVector4Transform(f4Vector, m);
            memcpy(&v, f4Vector.m128_f32, sizeof(v));
        }
        newMesh->CreateBuffers();
        parentNode.vChildrenMeshes.push_back(newMesh);
    }

    for (UINT i = 0; i < pNode->mNumChildren; i++) 
    {
        parentNode.vChildrenNodes.emplace_back();
        NodeData& nextNode = parentNode.vChildrenNodes[parentNode.vChildrenNodes.size() - 1];
        ProcessNode(strFilePath, strFileName, strExtension, bIsGltf, pNode->mChildren[i], pScene, m, nextNode);
    }
}

shared_ptr<MeshFile> FileLoader::ProcessMesh(
    const std::string& strFilePath,
    const std::string& strFileName,
    const std::string& strExtension,
    const bool& bIsGltf,
    aiMesh* pMesh, 
    const aiScene* pScene,
    NodeData& parentNode
)
{
    shared_ptr<MeshFile> meshData;
    string strFileWithExtMesh = strFileName + "_" + +pMesh->mName.C_Str() + strExtension;

    MeshFile* pMeshFile = (MeshFile*)GetUsingFile(strFileWithExtMesh).get();

    if (pMeshFile == nullptr)
    {
        cout << "Loading " << strFileWithExtMesh << "..." << endl;

        meshData = make_shared<MeshFile>(strFilePath, strFileWithExtMesh);
        for (UINT i = 0; i < pMesh->mNumVertices; i++)
        {
            meshData->vVertices.emplace_back(
                pMesh->mVertices[i].x,
                pMesh->mVertices[i].y,
                pMesh->mVertices[i].z
            );

            if (pMesh->mTextureCoords[0]) {
                meshData->vTexcoords.emplace_back(
                    (float)pMesh->mTextureCoords[0][i].x,
                    (float)pMesh->mTextureCoords[0][i].y
                );
            }

            bIsGltf ? meshData->vNormals.emplace_back(
                pMesh->mNormals[i].x, pMesh->mNormals[i].z, -pMesh->mNormals[i].y
            ) : meshData->vNormals.emplace_back(
                pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z
            );
        }

        for (UINT i = 0; i < pMesh->mNumFaces; i++) {
            aiFace face = pMesh->mFaces[i];
            for (UINT j = 0; j < face.mNumIndices; j++)
                meshData->vIndices.emplace_back(face.mIndices[j]);
        }

        meshData->vTangents.resize(meshData->vNormals.size());
        meshData->UpdateTangents();

        if (pMesh->mMaterialIndex >= 0)
        {
            vector<PBRModelTextureSet>& vTextureSets = parentNode.vChildrenTextureSets;
            vTextureSets.emplace_back();

            const aiTextureType aiTextureTypes[TEXTURE_MAP_NUM] = {
                aiTextureType_AMBIENT_OCCLUSION,
                aiTextureType_BASE_COLOR,
                aiTextureType_METALNESS,
                aiTextureType_DIFFUSE_ROUGHNESS,
                aiTextureType_EMISSIVE,
                aiTextureType_NORMALS,
                aiTextureType_HEIGHT
            };

            aiMaterial* material = pScene->mMaterials[pMesh->mMaterialIndex];

            for (size_t idx = 0; idx < TEXTURE_MAP_NUM; ++idx)
            {
                string textureFileName = ReadTextureFileName(strFilePath, pScene, material, aiTextureTypes[idx]);
                if (!textureFileName.empty())
                {
                    filesystem::path texturePath(textureFileName);
                    const string strFilePathIn = texturePath.parent_path().string();
                    const string strFileName = texturePath.filename().stem().string();
                    const string strExtention = texturePath.extension().string();
                    UINT x, y, comp;
                    vTextureSets[vTextureSets.size() - 1].spModelTexture[idx] = LoadImageFile(strFilePathIn, strFileName, strExtention, &x, &y, &comp);
                }
            }
        }

        AddUsingFile(strFileWithExtMesh, meshData);
    }
    else
    {
        meshData = pMeshFile->shared_from_this();
    }

    return meshData;

}

string FileLoader::ReadTextureFileName(
    const std::string& strFilePath,
    const aiScene* pScene,
    aiMaterial* pMaterial,
    aiTextureType eType
)
{
    string fullPath;
    if (pMaterial->GetTextureCount(eType) > 0) {
        aiString fileName;
        pMaterial->GetTexture(eType, 0, &fileName);
        fullPath = strFilePath + "\\" + fileName.C_Str();

        if (!filesystem::exists(fullPath)) 
        {
            const aiTexture* texture =
                pScene->GetEmbeddedTexture(fileName.C_Str());
            if (texture) 
            {
                if (string(texture->achFormatHint).find("png") !=
                    string::npos) {
                    ofstream fs(fullPath.c_str(), ios::binary | ios::out);
                    fs.write((char*)texture->pcData, texture->mWidth);
                    fs.close();
                }
            }
            else
            {
                cout << "No Embedded Texture. (" << fileName.C_Str() << ")" << endl;
            }
        }
        else 
        {

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

wstring FileLoader::ConvertUTF8ToUniCode(const string& str)
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

string FileLoader::ConvertUniCodeToUTF8(const wstring& wStr)
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

std::shared_ptr<MeshFile> FileLoader::LoadDefaultCubeMesh(const bool& bReverse)
{
    shared_ptr<MeshFile> meshData;
    string strMeshFileName = bReverse ? "DefaultReverseCube" : "DefaultCube";

    MeshFile* pMeshFile = (MeshFile*)GetUsingFile(strMeshFileName).get();
    if (pMeshFile == nullptr)
    {
        cout << "Loading " << strMeshFileName << "..." << endl;

        meshData = make_shared<MeshFile>("", strMeshFileName);
        for (uint32_t latitudeIdx = 0; latitudeIdx < DEFAULT_CUBE_LEVEL; ++latitudeIdx)
        {
            const float& fLatitudeLow = DirectX::XM_PIDIV2 / DEFAULT_CUBE_LEVEL * latitudeIdx;
            const float& fLatitudeHigh = DirectX::XM_PIDIV2 / DEFAULT_CUBE_LEVEL * (latitudeIdx + 1);
            const float& fLatitudeLowTextureCord = (latitudeIdx / DEFAULT_CUBE_LEVEL) / 2.f;
            const float& fLatitudeHighTextureCord = ((latitudeIdx + 1) / DEFAULT_CUBE_LEVEL) / 2.f;

            const uint32_t& usLatitudeOffset = (uint32_t)meshData->vVertices.size();

            for (uint32_t longitudeIdx = 0; longitudeIdx <= (uint32_t)DEFAULT_CUBE_LEVEL * 2; ++longitudeIdx)
            {
                const float& fLongitudeLow = DirectX::XM_2PI / (DEFAULT_CUBE_LEVEL * 2.f) * longitudeIdx;
                const float& fLongitudeTextureCord = longitudeIdx / (DEFAULT_CUBE_LEVEL * 2.f);

                meshData->vVertices.emplace_back(cosf(fLongitudeLow) * cosf(fLatitudeLow), sinf(fLatitudeLow), cosf(fLatitudeLow) * sinf(fLongitudeLow));
                meshData->vTexcoords.emplace_back(fLongitudeTextureCord, 0.5f + fLatitudeLowTextureCord);
                meshData->vNormals.emplace_back(cosf(fLongitudeLow) * cosf(fLatitudeLow), sinf(fLatitudeLow), cosf(fLatitudeLow) * sinf(fLongitudeLow));

                meshData->vVertices.emplace_back(cosf(fLongitudeLow) * cosf(fLatitudeHigh), sinf(fLatitudeHigh), cosf(fLatitudeHigh) * sinf(fLongitudeLow));
                meshData->vTexcoords.emplace_back(fLongitudeTextureCord, 0.5f + fLatitudeHighTextureCord);
                meshData->vNormals.emplace_back(cosf(fLongitudeLow) * cosf(fLatitudeHigh), sinf(fLatitudeHigh), cosf(fLatitudeHigh) * sinf(fLongitudeLow));

                meshData->vVertices.emplace_back(cosf(fLongitudeLow) * cosf(-fLatitudeLow), sinf(-fLatitudeLow), cosf(-fLatitudeLow) * sinf(fLongitudeLow));
                meshData->vTexcoords.emplace_back(fLongitudeTextureCord, 0.5f - fLatitudeLowTextureCord);
                meshData->vNormals.emplace_back(cosf(fLongitudeLow) * cosf(-fLatitudeLow), sinf(-fLatitudeLow), cosf(-fLatitudeLow) * sinf(fLongitudeLow));

                meshData->vVertices.emplace_back(cosf(fLongitudeLow) * cosf(-fLatitudeHigh), sinf(-fLatitudeHigh), cosf(-fLatitudeHigh) * sinf(fLongitudeLow));
                meshData->vTexcoords.emplace_back(fLongitudeTextureCord, 0.5f - fLatitudeHighTextureCord);
                meshData->vNormals.emplace_back(cosf(fLongitudeLow) * cosf(-fLatitudeHigh), sinf(-fLatitudeHigh), cosf(-fLatitudeHigh) * sinf(fLongitudeLow));
            }

            for (uint32_t longitudeIdx = 0; longitudeIdx < (uint32_t)DEFAULT_CUBE_LEVEL * 2; ++longitudeIdx)
            {
                const uint32_t& usLongitudeOffset = 4 * longitudeIdx + usLatitudeOffset;
                meshData->vIndices.push_back(usLongitudeOffset + 0);
                meshData->vIndices.push_back(usLongitudeOffset + 1);
                meshData->vIndices.push_back(usLongitudeOffset + 4);
                meshData->vIndices.push_back(usLongitudeOffset + 4);
                meshData->vIndices.push_back(usLongitudeOffset + 1);
                meshData->vIndices.push_back(usLongitudeOffset + 5);

                meshData->vIndices.push_back(usLongitudeOffset + 3);
                meshData->vIndices.push_back(usLongitudeOffset + 2);
                meshData->vIndices.push_back(usLongitudeOffset + 7);
                meshData->vIndices.push_back(usLongitudeOffset + 7);
                meshData->vIndices.push_back(usLongitudeOffset + 2);
                meshData->vIndices.push_back(usLongitudeOffset + 6);
            }
        }

        meshData->vTangents.resize(meshData->vNormals.size());
        meshData->UpdateTangents();

        if (bReverse)
        {
            std::reverse(meshData->vIndices.begin(), meshData->vIndices.end());
        }
        meshData->CreateBuffers();
        FileLoader::AddUsingFile(strMeshFileName, meshData);
    }
    else
    {
        meshData = pMeshFile->shared_from_this();
    }
    return meshData;
}
