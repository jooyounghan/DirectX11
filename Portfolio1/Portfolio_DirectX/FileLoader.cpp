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
#include "SkeletalModelFile.h"
#include "StaticModelFile.h"
#include "AnimationFile.h"

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

    // 모델 읽기
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
            // When Loading Model Files, Set Loaded Bone And Material as Argument to decide the type of the File Class between StaticModelFile and BoneMdel File
            // Static Model File Has Mesh and Material File and, Bone Model File Has Bone File and All of Static Mesh
            spMeshFile = LoadModelFile(strFilePath, strFileName, strExtension, bIsGltf, pScene, spBoneFile, vMaterials);
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
        spBone = make_shared<BoneFile>(BoneLabel);

        UpdateBoneNameSet(pScene, spBone->GetBoneInformation());
        LoadBoneFromNode(pScene->mRootNode, spBone.get(), spBone->GetBoneInformation());
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

vector<shared_ptr<AnimationFile>> FileLoader::LoadAnimationFile(const std::string strFileName, const aiScene* pScene)
{
    vector<shared_ptr<AnimationFile>> result;
    for (size_t animIdx = 0; animIdx < pScene->mNumAnimations; ++animIdx)
    {
        aiAnimation* pAnimation = pScene->mAnimations[animIdx];
        const string animationLabel = GetFileNameAsAnimationLabel(strFileName);
        shared_ptr<AnimationFile> spAnimation = make_shared<AnimationFile>(
            animationLabel, pAnimation->mDuration, pAnimation->mTicksPerSecond
        );

        for (size_t channelIdx = 0; channelIdx < pAnimation->mNumChannels; ++channelIdx)
        {
            aiNodeAnim* pAnimNode = pAnimation->mChannels[channelIdx];
            const string animNodeName = pAnimNode->mNodeName.C_Str();

            spAnimation->AddAnimChannel(
                AnimChannelFile(
                    animNodeName,
                    pAnimNode->mNumPositionKeys,
                    pAnimNode->mPositionKeys,
                    pAnimNode->mNumRotationKeys,
                    pAnimNode->mRotationKeys,
                    pAnimNode->mNumScalingKeys,
                    pAnimNode->mScalingKeys
                )
            );
        }
        result.push_back(spAnimation);
    }
    return result;
}

shared_ptr<IFile> FileLoader::LoadModelFile(
    const string& strFilePath, 
    const string& strFileName, 
    const string& strExtension,
    const bool& bIsGltf,
    const aiScene* pScene,
    const shared_ptr<BoneFile>& spBone,
    const vector<shared_ptr<MaterialFile>>& spMaterials
)
{
    const bool isSkeletal = spBone.get() != nullptr;

    shared_ptr<IFile> spModel;
    const string modelLabel = GetFileNameAsLabel(strFileName, strExtension);
    ModelFile* pModel = (ModelFile*)FileLoader::GetUsingFile(modelLabel).get();
    if (pModel == nullptr)
    {
        cout << "Start Loading " << modelLabel << "====================================" << endl;

        shared_ptr<ModelFile> spTempModel;

        if (isSkeletal)
        {
            shared_ptr<SkeletalModelFile> spTempSkeletalModel = make_shared<SkeletalModelFile>(modelLabel);
            spTempSkeletalModel->SetBoneFile(spBone);
            spTempModel = spTempSkeletalModel;
        }
        else
        {
            spTempModel = make_shared<StaticModelFile>(modelLabel);
        }

        uint8_t mesh_idx = 0;
        DirectX::XMMATRIX xmmTransform = DirectX::XMMatrixIdentity();
        ProcessNode(strFilePath, modelLabel, mesh_idx, bIsGltf, pScene->mRootNode, pScene, xmmTransform, spTempModel.get(), spMaterials);
        spTempModel->Initialize();
        cout << "Finish Loading " << modelLabel << "===================================" << endl;

        spModel = spTempModel;
        FileLoader::AddUsingFile(modelLabel, spModel);
    }
    else
    {
        if (isSkeletal)
        {
            spModel = ((SkeletalModelFile*)pModel)->shared_from_this();
        }
        else
        {
            spModel = ((StaticModelFile*)pModel)->shared_from_this();
        }
    }
    return spModel;
}

void FileLoader::ProcessNode(
    const string& strFilePath,
    const string& strFileLabel,
    uint8_t& ucElementIdx,
    const bool& bIsGltf,
    const aiNode* pNode, 
    const aiScene* pScene, 
    const DirectX::XMMATRIX& xmMatrix,
    ModelFile* pModelFile,
    const std::vector<std::shared_ptr<MaterialFile>>& spMaterials
)
{
    DirectX::XMMATRIX m(&pNode->mTransformation.a1);
    m = DirectX::XMMatrixTranspose(m) * xmMatrix;

   
    for (UINT idx = 0; idx < pNode->mNumMeshes; ++idx)
    {
        ucElementIdx++;
        aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[idx]];
        shared_ptr<MeshFile> meshFile = LoadMeshFile(strFilePath, ucElementIdx, bIsGltf, pMesh, m, pScene);
        
        const size_t materialIdx = pMesh->mMaterialIndex;
        if (spMaterials.size() > materialIdx)
        {
            meshFile->SetMaterial(spMaterials[materialIdx]);
        }

        pModelFile->AddMeshFile(meshFile);
    }

    for (UINT i = 0; i < pNode->mNumChildren; i++) 
    {
        ProcessNode(strFilePath, strFileLabel, ucElementIdx, bIsGltf, pNode->mChildren[i], pScene, m, pModelFile, spMaterials);
    }
}

shared_ptr<MeshFile> FileLoader::LoadMeshFile(
    const string& strFilePath,
    uint8_t& ucElementIdx,
    const bool& bIsGltf,
    const aiMesh* pMesh, 
    const DirectX::XMMATRIX& xmMatrix,
    const aiScene* pScene
)
{
    shared_ptr<MeshFile> result;

    const string strMeshLabelExtMesh = string(pMesh->mName.C_Str()) + to_string(ucElementIdx) + "_Mesh";
    MeshFile* pMeshFile = (MeshFile*)GetUsingFile(strMeshLabelExtMesh).get();
    if (pMeshFile == nullptr)
    {
        cout << "Loading " << strMeshLabelExtMesh << "..." << endl;

        result = make_shared<MeshFile>(strMeshLabelExtMesh);

        for (UINT i = 0; i < pMesh->mNumVertices; i++)
        {
            result->vVertices.emplace_back(
                pMesh->mVertices[i].x,
                pMesh->mVertices[i].y,
                pMesh->mVertices[i].z
            );

            if (pMesh->mTextureCoords[0]) {
                result->vTexcoords.emplace_back(
                    (float)pMesh->mTextureCoords[0][i].x,
                    (float)pMesh->mTextureCoords[0][i].y
                );
            }

            bIsGltf ? result->vNormals.emplace_back(
                pMesh->mNormals[i].x, -pMesh->mNormals[i].z, pMesh->mNormals[i].y
            ) : result->vNormals.emplace_back(
                pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z
            );
        }

        for (UINT i = 0; i < pMesh->mNumFaces; i++) {
            aiFace face = pMesh->mFaces[i];
            for (UINT j = 0; j < face.mNumIndices; j++)
                result->vIndices.emplace_back(face.mIndices[j]);
        }

        result->vTangents.resize(result->vNormals.size());
        result->UpdateTangents();

        vector<DirectX::XMFLOAT3>& vertices = result->vVertices;

        for (DirectX::XMFLOAT3& v : vertices)
        {
            DirectX::XMVECTOR f4Vector = DirectX::XMVectorSet(v.x, v.y, v.z, 0.f);
            f4Vector = DirectX::XMVector3TransformCoord(f4Vector, xmMatrix);
            memcpy(&v, f4Vector.m128_f32, sizeof(v));
        }
        AddUsingFile(strMeshLabelExtMesh, result);
    }
    else
    {
        result = pMeshFile->shared_from_this();
    }
    return result;
}


void FileLoader::UpdateBoneNameSet(
    const aiScene* pScene, 
    unordered_map<string, const void*>& unmapBoneInformation
)
{
    if (pScene != nullptr)
    {
        for (size_t mesh_idx = 0; mesh_idx < pScene->mNumMeshes; ++mesh_idx)
        {
            const aiMesh* pMesh = pScene->mMeshes[mesh_idx];
            for (size_t bone_idx = 0; bone_idx < pMesh->mNumBones; ++bone_idx)
            {
                const aiBone* pBone = pMesh->mBones[bone_idx];
                unmapBoneInformation.emplace(pBone->mName.C_Str(), pBone);
            }
        }
    }
}

void FileLoader::LoadBoneFromNode(
    const aiNode* pNode,
    Bone* pBoneParent,
    const unordered_map<string, const void*>& unmapBoneInformation
)
{
    if (pNode != nullptr && pBoneParent != nullptr)
    {
        for (size_t node_idx = 0; node_idx < pNode->mNumChildren; ++node_idx)
        {
            aiNode* pChild = pNode->mChildren[node_idx];
            const string boneName = pChild->mName.C_Str();
            if (unmapBoneInformation.find(boneName) != unmapBoneInformation.end())
            {
                const aiBone* pBone = (aiBone*)unmapBoneInformation.at(boneName);
                pBoneParent->AddBoneChild(boneName);
                Bone* pLastBone = pBoneParent->GetLatestBoneChild();
                pLastBone->SetWeights(pBone->mNumWeights, pBone->mWeights);
                pLastBone->SetBoneMatrix(DirectX::XMMATRIX(&pBone->mOffsetMatrix.a1));
                LoadBoneFromNode(pChild, pLastBone, unmapBoneInformation);
            }
        }
    }
}

string FileLoader::ReadTextureFileName(
    const string& strFilePath,
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

shared_ptr<ModelFile> FileLoader::LoadDefaultCubeModel(const bool& bReverse)
{
    shared_ptr<ModelFile> modelData;
    string strModelFileName = bReverse ? "DefaultReverseCube_MODEL" : "DefaultCube_MODEL";

    StaticModelFile* pModelFile = (StaticModelFile*)GetUsingFile(strModelFileName).get();

    if (pModelFile == nullptr)
    {
        modelData = make_shared<StaticModelFile>(strModelFileName);
        modelData->AddMeshFile(LoadDefaultCubeMesh(bReverse));
    }
    else
    {
        modelData = pModelFile->shared_from_this();
    }

    return modelData;
}

shared_ptr<class MeshFile> FileLoader::LoadDefaultCubeMesh(const bool& bReverse)
{
    shared_ptr<MeshFile> meshData;
    string strMeshFileName = bReverse ? "DefaultReverseCube_MESH" : "DefaultCube_MESH";

    const float normalFactor = bReverse ? -1.f : 1.f;

    MeshFile* pMeshFile = (MeshFile*)GetUsingFile(strMeshFileName).get();
    if (pMeshFile == nullptr)
    {
        cout << "Loading " << strMeshFileName << "..." << endl;

        meshData = make_shared<MeshFile>(strMeshFileName);
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
                meshData->vVertices.emplace_back(cosf(fLongitudeLow) * cosf(fLatitudeHigh), sinf(fLatitudeHigh), cosf(fLatitudeHigh) * sinf(fLongitudeLow));
                meshData->vVertices.emplace_back(cosf(fLongitudeLow) * cosf(-fLatitudeLow), sinf(-fLatitudeLow), cosf(-fLatitudeLow) * sinf(fLongitudeLow));
                meshData->vVertices.emplace_back(cosf(fLongitudeLow) * cosf(-fLatitudeHigh), sinf(-fLatitudeHigh), cosf(-fLatitudeHigh) * sinf(fLongitudeLow));

                meshData->vTexcoords.emplace_back(fLongitudeTextureCord, 0.5f + fLatitudeLowTextureCord);
                meshData->vTexcoords.emplace_back(fLongitudeTextureCord, 0.5f + fLatitudeHighTextureCord);
                meshData->vTexcoords.emplace_back(fLongitudeTextureCord, 0.5f - fLatitudeLowTextureCord);
                meshData->vTexcoords.emplace_back(fLongitudeTextureCord, 0.5f - fLatitudeHighTextureCord);

                meshData->vNormals.emplace_back(normalFactor * cosf(fLongitudeLow) * cosf(fLatitudeLow), normalFactor * sinf(fLatitudeLow), normalFactor * cosf(fLatitudeLow) * sinf(fLongitudeLow));
                meshData->vNormals.emplace_back(normalFactor * cosf(fLongitudeLow) * cosf(fLatitudeHigh), normalFactor * sinf(fLatitudeHigh), normalFactor * cosf(fLatitudeHigh) * sinf(fLongitudeLow));
                meshData->vNormals.emplace_back(normalFactor * cosf(fLongitudeLow) * cosf(-fLatitudeLow), normalFactor * sinf(-fLatitudeLow), normalFactor * cosf(-fLatitudeLow) * sinf(fLongitudeLow));
                meshData->vNormals.emplace_back(normalFactor * cosf(fLongitudeLow) * cosf(-fLatitudeHigh), normalFactor * sinf(-fLatitudeHigh), normalFactor * cosf(-fLatitudeHigh) * sinf(fLongitudeLow));
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
            reverse(meshData->vIndices.begin(), meshData->vIndices.end());
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

shared_ptr<MeshFile> FileLoader::LoadPlaneMesh(
    const float& fMirrorWidthIn,
    const float& fMirrorHeightIn
)
{
    shared_ptr<MeshFile> meshData = make_shared<MeshFile>("Plane_MESH");

    meshData->vVertices = vector<XMFLOAT3> { 
        XMFLOAT3(-0.5f * fMirrorWidthIn, -0.5f * fMirrorHeightIn, 0.f),
        XMFLOAT3(-0.5f * fMirrorWidthIn, +0.5f * fMirrorHeightIn, 0.f),
        XMFLOAT3(+0.5f * fMirrorWidthIn, +0.5f * fMirrorHeightIn, 0.f),
        XMFLOAT3(+0.5f * fMirrorWidthIn, -0.5f * fMirrorHeightIn, 0.f)
    };

    meshData->vTexcoords = vector<XMFLOAT2>{
        XMFLOAT2(1.f, 1.f),
        XMFLOAT2(1.f, 0.f),
        XMFLOAT2(0.f, 0.f),
        XMFLOAT2(0.f, 1.f)
    };

    meshData->vNormals = vector<XMFLOAT3>{
        XMFLOAT3(0.0f, 0.0f, 1.0f),
        XMFLOAT3(0.0f, 0.0f, 1.0f),
        XMFLOAT3(0.0f, 0.0f, 1.0f),
        XMFLOAT3(0.0f, 0.0f, 1.0f),
    };

    meshData->vIndices = vector<uint32_t>{
        0, 2, 1, 0, 3, 2,
    };

    meshData->vTangents.resize(meshData->vNormals.size());
    meshData->UpdateTangents();
    meshData->CreateBuffers();

    return meshData;
}

/*
File은 변하지 않는 객체로 두고, 이를 생성자에서 받아서 인스턴스를 조작하는 방식으로!
ModelFile -> GroupModel
BoneFile -> Bone
MaterialFile -> Material
AnimationFile -> Animation


*/