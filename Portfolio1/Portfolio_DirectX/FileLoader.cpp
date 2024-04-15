#define __STDC_LIB_EXT1__
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "FileLoader.h"
#include "Console.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "NormalImageFile.h"
#include "DDSImageFile.h"
#include "BoneFile.h"
#include "MaterialFile.h"
#include "AnimationFile.h"
#include "MeshFile.h"

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
using namespace DirectX;

unordered_map<string, weak_ptr<class IFile>> FileLoader::mapUsingFiles;
size_t FileLoader::uiBoneId = -1;

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
    const string fullPath = strFilePath + "\\" + strFileName + strExtension;
    const string fileLabel = GetFileNameAsLabel(strFileName, strExtension);

    shared_ptr<IImageFile> result;

    IImageFile* pImg = (IImageFile*)FileLoader::GetUsingFile(fileLabel).get();
    uint8_t* ucImageRawData = nullptr;

    if (pImg == nullptr)
    {
        cout << "Loading " << fileLabel << "..." << endl;
        if (strExtension == ".exr")
        {
            ucImageRawData = FileLoader::LoadFileWithOpenEXR(fullPath.c_str(), x, y, comp);
            result = make_shared<NormalImageFile>(*x, *y, DXGI_FORMAT_R16G16B16A16_FLOAT, ucImageRawData, fileLabel);
        }
        else if (strExtension == ".dds")
        {
            result = make_shared<DDSImageFile>(fullPath, fileLabel, strFilePath.find("Brdf") == string::npos);
        }
        else
        {
            ucImageRawData = FileLoader::LoadFileWithStbi(fullPath.c_str(), x, y, comp);
            result = make_shared<NormalImageFile>(*x, *y, DXGI_FORMAT_R8G8B8A8_UNORM, ucImageRawData, fileLabel);
        }
        FileLoader::AddUsingFile(fileLabel, result);
    }
    else
    {
        result = pImg->shared_from_this();
    }
    FileLoader::FreeLoadedFileData(ucImageRawData);
    return result;
}


vector<shared_ptr<IFile>> FileLoader::LoadModelFileSet(
    const string& strFilePath, 
    const string& strFileName,
    const string& strExtension
)
{
    bool bIsGltf = false;
    strExtension == ".gltf" ? bIsGltf = true : bIsGltf = false;

    const string fullPath = strFilePath + "\\" + strFileName + strExtension;

    // ¸ðµ¨ ÀÐ±â
    Assimp::Importer importer;
    importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    const aiScene* pScene = importer.ReadFile(
        fullPath,
        aiProcess_Triangulate | aiProcess_ConvertToLeftHanded
    );

    vector<shared_ptr<IFile>> vLoadedFiles;

    if (pScene)
    {
        // Read Bone File
        shared_ptr<BoneFile> spBoneFile;
        if (HasBones(pScene))
        {
            spBoneFile = LoadBoneFile(strFileName, pScene);
            vLoadedFiles.emplace_back(spBoneFile);
        }

        // Read Material File
        vector<shared_ptr<MaterialFile>> vMaterials;
        if (pScene->mNumMaterials > 0)
        {
            vMaterials = LoadMaterialFile(strFilePath, pScene, bIsGltf);
            for (auto& material : vMaterials)
            {
                vLoadedFiles.emplace_back(material);
            }
        }

        // Read Animation File
        vector<shared_ptr<AnimationFile>> spAnimFiles;
        if (pScene->mNumAnimations > 0)
        {
            spAnimFiles = LoadAnimationFile(strFileName, pScene);
            for (auto& anim : spAnimFiles)
            {
                vLoadedFiles.emplace_back(anim);
            }
        }

        // Read Mesh File
        shared_ptr<IFile> spMeshFile;
        if (pScene->mNumMeshes > 0)
        {
            spMeshFile = LoadMeshFile(
                strFilePath, strFileName, 
                strExtension, bIsGltf, 
                pScene, spBoneFile, vMaterials
            );
            vLoadedFiles.emplace_back(spMeshFile);
        }
    }
    else
    {
        cout << "Loading " << fullPath << " Failed" << endl;
        cout << "Assimp Importer Error Code : " << importer.GetErrorString() << endl;
    }

    return vLoadedFiles;
}

bool FileLoader::HasBones(const aiScene* pScene)
{
    if (pScene != nullptr)
    {
        for (size_t mesh_idx = 0; mesh_idx < pScene->mNumMeshes; ++mesh_idx)
        {
            const aiMesh* pMesh = pScene->mMeshes[mesh_idx];
            if (pMesh->HasBones())
            {
                return true;
            }
            else
            {

            }
        }
    }
    return false;
}

shared_ptr<BoneFile> FileLoader::LoadBoneFile(const string strFileName, const aiScene* pScene)
{
    shared_ptr<BoneFile> spBone;
    const string BoneLabel = GetFileNameAsBoneLabel(strFileName);
    BoneFile* pBone = (BoneFile*)FileLoader::GetUsingFile(BoneLabel).get();
    if (pBone == nullptr)
    {
        cout << "Loading " << BoneLabel << "..." << endl;
        const std::unordered_map<std::string, const aiBone*> nodeToBoneTable = GetNodeToBoneTable(pScene);

        uiBoneId = -1;
        spBone = make_shared<BoneFile>(BoneLabel, nodeToBoneTable.size());
        LoadBoneFromNode(pScene->mRootNode, spBone.get(), spBone->GetRootBone(), nodeToBoneTable);
        FileLoader::AddUsingFile(BoneLabel, spBone);
    }
    else
    {
        spBone = pBone->shared_from_this();
    }
    return spBone;
}

vector<shared_ptr<MaterialFile>> FileLoader::LoadMaterialFile(
    const string strFilePath,
    const aiScene* pScene,
    const bool& bIsGltfIn
)
{
    const aiTextureType aiTextureTypes[TEXTURE_MAP_NUM] = {
                aiTextureType_AMBIENT_OCCLUSION,
                aiTextureType_BASE_COLOR,
                aiTextureType_DIFFUSE,
                aiTextureType_SPECULAR,
                aiTextureType_METALNESS,
                aiTextureType_DIFFUSE_ROUGHNESS,
                aiTextureType_EMISSIVE,
                aiTextureType_NORMALS,
                aiTextureType_HEIGHT
    };

    vector<shared_ptr<MaterialFile>> spMaterials;
    for (size_t material_idx = 0; material_idx < pScene->mNumMaterials; ++material_idx)
    {
        aiMaterial* pMaterial = pScene->mMaterials[material_idx];
        const string materialName = pMaterial->GetName().C_Str() + string("_MATERIAL");
        spMaterials.emplace_back(make_shared<MaterialFile>(materialName, bIsGltfIn));
        for (size_t idx = 0; idx < TEXTURE_MAP_NUM; ++idx)
        {           
            MaterialFile* currentMaterial = (MaterialFile*)spMaterials[spMaterials.size() - 1].get();
            string textureFileName = ReadTextureFileName(strFilePath, pScene, pMaterial, aiTextureTypes[idx]);
            if (!textureFileName.empty())
            {
                filesystem::path texturePath(textureFileName);
                const string strFilePathIn = texturePath.parent_path().string();
                const string strFileLabel = texturePath.filename().stem().string();
                const string strExtention = texturePath.extension().string();
                UINT x, y, comp;
                currentMaterial->SetTextureImageFile((EModelTextures)idx, LoadImageFile(strFilePathIn, strFileLabel, strExtention, &x, &y, &comp));
            }
        }
    }
    return spMaterials;
}

vector<shared_ptr<AnimationFile>> FileLoader::LoadAnimationFile(
    const std::string strFileName,
    const aiScene* pScene
)
{
    vector<shared_ptr<AnimationFile>> result;
 
    for (size_t animIdx = 0; animIdx < pScene->mNumAnimations; ++animIdx)
    {
        aiAnimation* pAnimation = pScene->mAnimations[animIdx];
        shared_ptr<AnimationFile> spAnimation;

        const string animationLabel = GetFileNameAsAnimationLabel(strFileName);
        AnimationFile* pAnimationFile = (AnimationFile*)FileLoader::GetUsingFile(animationLabel).get();
        if (pAnimationFile == nullptr)
        {
            cout << "Loading " << animationLabel << "..." << endl;

            spAnimation = make_shared<AnimationFile>(
                animationLabel, pAnimation->mDuration, pAnimation->mTicksPerSecond
                );

            for (size_t channelIdx = 0; channelIdx < pAnimation->mNumChannels; ++channelIdx)
            {
                aiNodeAnim* pAnimNode = pAnimation->mChannels[channelIdx];
                const string animNodeName = pAnimNode->mNodeName.C_Str();
                AnimChannel* pAnimChannel = spAnimation->AddAnimChannel(animNodeName);

                pAnimChannel->ReserveTranslation(pAnimNode->mNumPositionKeys);
                for (size_t idx = 0; idx < pAnimNode->mNumPositionKeys; ++idx)
                {
                    pAnimChannel->AddTranslation(
                        pAnimNode->mPositionKeys[idx].mTime,
                        pAnimNode->mPositionKeys[idx].mValue.x,
                        pAnimNode->mPositionKeys[idx].mValue.y,
                        pAnimNode->mPositionKeys[idx].mValue.z
                    );
                }

                pAnimChannel->ReserveRotationQuat(pAnimNode->mNumRotationKeys);
                for (size_t idx = 0; idx < pAnimNode->mNumRotationKeys; ++idx)
                {
                    pAnimChannel->AddRotationQuat(
                        pAnimNode->mRotationKeys[idx].mTime,
                        pAnimNode->mRotationKeys[idx].mValue.x,
                        pAnimNode->mRotationKeys[idx].mValue.y,
                        pAnimNode->mRotationKeys[idx].mValue.z,
                        pAnimNode->mRotationKeys[idx].mValue.w
                    );
                }

                pAnimChannel->ReserveScale(pAnimNode->mNumScalingKeys);
                for (size_t idx = 0; idx < pAnimNode->mNumScalingKeys; ++idx)
                {
                    pAnimChannel->AddScale(
                        pAnimNode->mScalingKeys[idx].mTime,
                        pAnimNode->mScalingKeys[idx].mValue.x,
                        pAnimNode->mScalingKeys[idx].mValue.y,
                        pAnimNode->mScalingKeys[idx].mValue.z
                    );
                }



            }
        }
        else
        {
            spAnimation = pAnimationFile->shared_from_this();
        }
        result.push_back(spAnimation);
    }
    return result;
}

shared_ptr<MeshFile> FileLoader::LoadMeshFile(
    const string& strFilePath, 
    const string& strFileName, 
    const string& strExtension,
    const bool& bIsGltf,
    const aiScene* pScene,
    const std::shared_ptr<BoneFile>& spBoneFileIn,
    const std::vector<std::shared_ptr<MaterialFile>>& spMaterialFileIn
)
{
    shared_ptr<MeshFile> spMesh;

    const string meshLabel = GetFileNameAsLabel(strFileName, strExtension);
    MeshFile* pMesh = (MeshFile*)FileLoader::GetUsingFile(meshLabel).get();

    if (pMesh == nullptr)
    {
        cout << "Loading " << meshLabel << "..." << endl;

        size_t mesh_idx = 0;
        DirectX::XMMATRIX xmmTransform = DirectX::XMMatrixIdentity();
        spMesh = make_shared<MeshFile>(meshLabel, pScene->mNumMeshes, bIsGltf, spBoneFileIn);
        ProcessNode(strFilePath, meshLabel, mesh_idx, bIsGltf, pScene->mRootNode, pScene, xmmTransform, spMesh.get(), spMaterialFileIn);
        spMesh->Initialize();
        FileLoader::AddUsingFile(meshLabel, spMesh);
    }
    else
    {
        spMesh = pMesh->shared_from_this();
    }
    return spMesh;
}

void FileLoader::ProcessNode(
    const string& strFilePath,
    const string& strFileLabel,
    size_t& uiElementIdx,
    const bool& bIsGltf,
    const struct aiNode* pNode,
    const struct aiScene* pScene,
    const XMMATRIX& xmMatrix,
    class MeshFile* pMeshFile,
    const std::vector<std::shared_ptr<MaterialFile>>& spMaterialFileIn
)
{
    DirectX::XMMATRIX m(&pNode->mTransformation.a1);
    m = DirectX::XMMatrixTranspose(m) * xmMatrix;
   
    for (UINT idx = 0; idx < pNode->mNumMeshes; ++idx)
    {
        aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[idx]];
        LoadMeshData(strFilePath, uiElementIdx, bIsGltf, pMesh, m, pScene, pMeshFile, spMaterialFileIn);
        uiElementIdx++;
    }

    for (UINT i = 0; i < pNode->mNumChildren; i++) 
    {
        ProcessNode(strFilePath, strFileLabel, uiElementIdx, bIsGltf, pNode->mChildren[i], pScene, m, pMeshFile, spMaterialFileIn);
    }
}

void FileLoader::LoadMeshData(
    const string& strFilePath,
    size_t& uiElementIdx,
    const bool& bIsGltf,
    const aiMesh* pMesh, 
    const DirectX::XMMATRIX& xmMatrix,
    const aiScene* pScene,
    class MeshFile* pMeshFile,
    const std::vector<std::shared_ptr<MaterialFile>>& spMaterialFileIn
)
{
    const unsigned int materialIdx = pMesh->mMaterialIndex;
    if (spMaterialFileIn.size() > pMesh->mMaterialIndex)
    {
        pMeshFile->SetMaterialFile(uiElementIdx, spMaterialFileIn[pMesh->mMaterialIndex]);
    }

    Mesh& mesh = pMeshFile->GetMeshData(uiElementIdx);
    
    for (UINT vertexIdx = 0; vertexIdx < pMesh->mNumVertices; vertexIdx++)
    {
        mesh.vVertices.emplace_back(
            pMesh->mVertices[vertexIdx].x,
            pMesh->mVertices[vertexIdx].y,
            pMesh->mVertices[vertexIdx].z
        );

        if (pMesh->mTextureCoords[0]) {
            mesh.vTexcoords.emplace_back(
                (float)pMesh->mTextureCoords[0][vertexIdx].x,
                (float)pMesh->mTextureCoords[0][vertexIdx].y
            );
        }

        bIsGltf ? mesh.vNormals.emplace_back(
            pMesh->mNormals[vertexIdx].x, -pMesh->mNormals[vertexIdx].z, pMesh->mNormals[vertexIdx].y
        ) : mesh.vNormals.emplace_back(
            pMesh->mNormals[vertexIdx].x, pMesh->mNormals[vertexIdx].y, pMesh->mNormals[vertexIdx].z
        );
    }

    for (UINT faceIdx = 0; faceIdx < pMesh->mNumFaces; faceIdx++) {
        aiFace face = pMesh->mFaces[faceIdx];
        for (UINT j = 0; j < face.mNumIndices; j++)
            mesh.vIndices.emplace_back(face.mIndices[j]);
    }

    mesh.vTangents.resize(mesh.vNormals.size());

    for (DirectX::XMFLOAT3& v : mesh.vVertices)
    {
        DirectX::XMVECTOR f4Vector = DirectX::XMVectorSet(v.x, v.y, v.z, 0.f);
        f4Vector = DirectX::XMVector3TransformCoord(f4Vector, xmMatrix);
        memcpy(&v, f4Vector.m128_f32, sizeof(v));
    }

    BoneFile* pBoneFile = pMeshFile->GetBoneFile();
    if (pMesh->HasBones() && pBoneFile != nullptr)
    {
        mesh.vBlendWeight1.resize(mesh.vVertices.size());
        mesh.vBlendWeight2.resize(mesh.vVertices.size());
        mesh.vBlendIndices1.resize(mesh.vVertices.size());
        mesh.vBlendIndices2.resize(mesh.vVertices.size());

        vector<vector<float>> tempWeights;
        vector<vector<size_t>> tempIndices;

        tempWeights.resize(mesh.vVertices.size());
        tempIndices.resize(mesh.vVertices.size());

        for (size_t boneIdx = 0; boneIdx < pMesh->mNumBones; ++boneIdx)
        {
            const aiBone* pBone = pMesh->mBones[boneIdx];
            size_t boneId = pBoneFile->GetBoneIdx(pBone->mName.C_Str());

            for (size_t weightIdx = 0; weightIdx < pBone->mNumWeights; ++weightIdx)
            {
                const aiVertexWeight& pWeight = pBone->mWeights[weightIdx];
                tempWeights[pWeight.mVertexId].push_back(pWeight.mWeight);
                tempIndices[pWeight.mVertexId].push_back(boneId);
            }            
        }

        for (size_t vertexIdx = 0; vertexIdx < mesh.vVertices.size(); ++vertexIdx)
        {
            vector<float>& weights = tempWeights[vertexIdx];
            vector<size_t>& indices = tempIndices[vertexIdx];

            const size_t weightNums = weights.size();

            for (size_t weightIdx = 0; weightIdx < weightNums; ++weightIdx)
            {
                weightIdx < 4 ? *(&mesh.vBlendWeight1[vertexIdx].x + weightIdx) = weights[weightIdx] :
                    *(&mesh.vBlendWeight2[vertexIdx].x + weightIdx - 4) = weights[weightIdx];

                weightIdx < 4 ? *(&mesh.vBlendIndices1[vertexIdx].x + weightIdx) = indices[weightIdx] :
                    *(&mesh.vBlendIndices2[vertexIdx].x + weightIdx - 4) = indices[weightIdx];
            }
        }
    }
}

std::unordered_map<string, const aiBone*> FileLoader::GetNodeToBoneTable(const aiScene* pScene)
{
    std::unordered_map<string, const aiBone*> result;

    for (size_t mesh_idx = 0; mesh_idx < pScene->mNumMeshes; ++mesh_idx)
    {
        const aiMesh* pMesh = pScene->mMeshes[mesh_idx];
        for (size_t bone_idx = 0; bone_idx < pMesh->mNumBones; ++bone_idx)
        {
            const aiBone* pBone = pMesh->mBones[bone_idx];
            result.emplace(pBone->mName.C_Str(), pBone);
        }
    }
    return result;
}

void FileLoader::LoadBoneFromNode(
    const aiNode* pNode,
    BoneFile* pBoneFile,
    BoneData& boneData,
    const unordered_map<string, const aiBone*>& nodeToBoneTable
)
{
    if (pNode != nullptr)
    {
        boneData.strBoneName = pNode->mName.C_Str();

        if (nodeToBoneTable.find(boneData.strBoneName) != nodeToBoneTable.end())
        {
            uiBoneId++;
            const aiBone* pBone = nodeToBoneTable.at(boneData.strBoneName);
            pBoneFile->SetOffsetMatrix(boneData.strBoneName, uiBoneId, XMMatrixTranspose(XMMATRIX((float*)&pBone->mOffsetMatrix)));
        }

        for (size_t node_idx = 0; node_idx < pNode->mNumChildren; ++node_idx)
        {
            const aiNode* pChildNode = pNode->mChildren[node_idx];
            if (pChildNode->mNumMeshes == 0)
            {
                boneData.vBoneChildren.emplace_back();
                BoneData& currentBoneData = boneData.vBoneChildren[boneData.vBoneChildren.size() - 1];
                currentBoneData.pBoneParent = &boneData;
                LoadBoneFromNode(pChildNode, pBoneFile, currentBoneData, nodeToBoneTable);
            }
        }
    }
}

string FileLoader::ReadTextureFileName(
    const std::string& strFilePath, 
    const aiScene* pScene, 
    aiMaterial* pMaterial, 
    aiTextureType eType
)
{
    filesystem::path fullPath;
    if (pMaterial->GetTextureCount(eType) > 0) {
        aiString fileName;
        pMaterial->GetTexture(eType, 0, &fileName);
        fullPath = fileName.C_Str();
        fullPath = strFilePath + "\\textures\\" + fullPath.filename().string();
        if (!filesystem::exists(fullPath))
        {
            const aiTexture* texture =
                pScene->GetEmbeddedTexture(fileName.C_Str());
            if (texture)
            {
                if (string(texture->achFormatHint).find("png") != string::npos)
                {
                    filesystem::create_directories(fullPath.parent_path());

                    ofstream fs(fullPath.string(), ios::binary | ios::out);
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
    return fullPath.string();
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

shared_ptr<MeshFile> FileLoader::LoadDefaultCubeMesh(
    const bool& bReverse
)
{
    shared_ptr<MeshFile> spMeshFile;
    string strMeshFileName = bReverse ? "DefaultReverseCube_MESH" : "DefaultCube_MESH";

    const float normalFactor = bReverse ? -1.f : 1.f;

    MeshFile* pMeshFile = (MeshFile*)GetUsingFile(strMeshFileName).get();
    if (pMeshFile == nullptr)
    {
        cout << "Loading " << strMeshFileName << "..." << endl;

        spMeshFile = make_shared<MeshFile>(strMeshFileName, 1, false);
        Mesh& meshData = spMeshFile->GetMeshData(0);

        for (uint32_t latitudeIdx = 0; latitudeIdx < DEFAULT_CUBE_LEVEL; ++latitudeIdx)
        {
            const float& fLatitudeLow = DirectX::XM_PIDIV2 / DEFAULT_CUBE_LEVEL * latitudeIdx;
            const float& fLatitudeHigh = DirectX::XM_PIDIV2 / DEFAULT_CUBE_LEVEL * (latitudeIdx + 1);
            const float& fLatitudeLowTextureCord = (latitudeIdx / DEFAULT_CUBE_LEVEL) / 2.f;
            const float& fLatitudeHighTextureCord = ((latitudeIdx + 1) / DEFAULT_CUBE_LEVEL) / 2.f;

            const uint32_t& usLatitudeOffset = (uint32_t)meshData.vVertices.size();

            for (uint32_t longitudeIdx = 0; longitudeIdx <= (uint32_t)DEFAULT_CUBE_LEVEL * 2; ++longitudeIdx)
            {
                const float& fLongitudeLow = DirectX::XM_2PI / (DEFAULT_CUBE_LEVEL * 2.f) * longitudeIdx;
                const float& fLongitudeTextureCord = longitudeIdx / (DEFAULT_CUBE_LEVEL * 2.f);

                meshData.vVertices.emplace_back(cosf(fLongitudeLow) * cosf(fLatitudeLow), sinf(fLatitudeLow), cosf(fLatitudeLow) * sinf(fLongitudeLow));
                meshData.vVertices.emplace_back(cosf(fLongitudeLow) * cosf(fLatitudeHigh), sinf(fLatitudeHigh), cosf(fLatitudeHigh) * sinf(fLongitudeLow));
                meshData.vVertices.emplace_back(cosf(fLongitudeLow) * cosf(-fLatitudeLow), sinf(-fLatitudeLow), cosf(-fLatitudeLow) * sinf(fLongitudeLow));
                meshData.vVertices.emplace_back(cosf(fLongitudeLow) * cosf(-fLatitudeHigh), sinf(-fLatitudeHigh), cosf(-fLatitudeHigh) * sinf(fLongitudeLow));

                meshData.vTexcoords.emplace_back(fLongitudeTextureCord, 0.5f + fLatitudeLowTextureCord);
                meshData.vTexcoords.emplace_back(fLongitudeTextureCord, 0.5f + fLatitudeHighTextureCord);
                meshData.vTexcoords.emplace_back(fLongitudeTextureCord, 0.5f - fLatitudeLowTextureCord);
                meshData.vTexcoords.emplace_back(fLongitudeTextureCord, 0.5f - fLatitudeHighTextureCord);

                meshData.vNormals.emplace_back(normalFactor * cosf(fLongitudeLow) * cosf(fLatitudeLow), normalFactor * sinf(fLatitudeLow), normalFactor * cosf(fLatitudeLow) * sinf(fLongitudeLow));
                meshData.vNormals.emplace_back(normalFactor * cosf(fLongitudeLow) * cosf(fLatitudeHigh), normalFactor * sinf(fLatitudeHigh), normalFactor * cosf(fLatitudeHigh) * sinf(fLongitudeLow));
                meshData.vNormals.emplace_back(normalFactor * cosf(fLongitudeLow) * cosf(-fLatitudeLow), normalFactor * sinf(-fLatitudeLow), normalFactor * cosf(-fLatitudeLow) * sinf(fLongitudeLow));
                meshData.vNormals.emplace_back(normalFactor * cosf(fLongitudeLow) * cosf(-fLatitudeHigh), normalFactor * sinf(-fLatitudeHigh), normalFactor * cosf(-fLatitudeHigh) * sinf(fLongitudeLow));
            }

            for (uint32_t longitudeIdx = 0; longitudeIdx < (uint32_t)DEFAULT_CUBE_LEVEL * 2; ++longitudeIdx)
            {
                const uint32_t& usLongitudeOffset = 4 * longitudeIdx + usLatitudeOffset;
                meshData.vIndices.push_back(usLongitudeOffset + 0);
                meshData.vIndices.push_back(usLongitudeOffset + 1);
                meshData.vIndices.push_back(usLongitudeOffset + 4);
                meshData.vIndices.push_back(usLongitudeOffset + 4);
                meshData.vIndices.push_back(usLongitudeOffset + 1);
                meshData.vIndices.push_back(usLongitudeOffset + 5);

                meshData.vIndices.push_back(usLongitudeOffset + 3);
                meshData.vIndices.push_back(usLongitudeOffset + 2);
                meshData.vIndices.push_back(usLongitudeOffset + 7);
                meshData.vIndices.push_back(usLongitudeOffset + 7);
                meshData.vIndices.push_back(usLongitudeOffset + 2);
                meshData.vIndices.push_back(usLongitudeOffset + 6);
            }
        }

        meshData.vTangents.resize(meshData.vNormals.size());
        if (bReverse)
        {
            reverse(meshData.vIndices.begin(), meshData.vIndices.end());
        }
        spMeshFile->Initialize();
        FileLoader::AddUsingFile(strMeshFileName, spMeshFile);
    }
    else
    {
        spMeshFile = pMeshFile->shared_from_this();
    }
    return spMeshFile;
}

shared_ptr<MeshFile> FileLoader::LoadPlaneMesh(
    const float& fMirrorWidthIn,
    const float& fMirrorHeightIn
)
{
    shared_ptr<MeshFile> spMeshFile = make_shared<MeshFile>("Plane_MESH", 1, false);
    Mesh& meshData = spMeshFile->GetMeshData(0);
    meshData.vVertices = vector<XMFLOAT3> {
        XMFLOAT3(-0.5f * fMirrorWidthIn, -0.5f * fMirrorHeightIn, 0.f),
        XMFLOAT3(-0.5f * fMirrorWidthIn, +0.5f * fMirrorHeightIn, 0.f),
        XMFLOAT3(+0.5f * fMirrorWidthIn, +0.5f * fMirrorHeightIn, 0.f),
        XMFLOAT3(+0.5f * fMirrorWidthIn, -0.5f * fMirrorHeightIn, 0.f)
    };

    meshData.vTexcoords = vector<XMFLOAT2>{
        XMFLOAT2(1.f, 1.f),
        XMFLOAT2(1.f, 0.f),
        XMFLOAT2(0.f, 0.f),
        XMFLOAT2(0.f, 1.f)
    };

    meshData.vNormals = vector<XMFLOAT3>{
        XMFLOAT3(0.0f, 0.0f, 1.0f),
        XMFLOAT3(0.0f, 0.0f, 1.0f),
        XMFLOAT3(0.0f, 0.0f, 1.0f),
        XMFLOAT3(0.0f, 0.0f, 1.0f),
    };

    meshData.vIndices = vector<uint32_t>{
        0, 2, 1, 0, 3, 2,
    };

    meshData.vTangents.resize(meshData.vNormals.size());
    spMeshFile->Initialize();

    return spMeshFile;
}