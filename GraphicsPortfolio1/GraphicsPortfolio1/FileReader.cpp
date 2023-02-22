#include <iostream>
#include <directxtk/SimpleMath.h>
#include <directxtk/DDSTextureLoader.h>
#include <filesystem>

#include "FileReader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace DirectX;
using DirectX::SimpleMath::Matrix;

void FileReader::GetImage(const string& filename, OUT Image& image)
{
    int width, height, num_channels;
    stbi_uc* file_data = stbi_load(filename.c_str(), &width, &height, &num_channels, 0);

    image.width = width;
    image.height = height;
    image.channels = num_channels;
    image.image_data_.resize(image.width * image.height * 4);

    const int& area = image.width * image.height;
    const int& channels = image.channels;

    for (size_t i = 0; i < area; ++i)
    {
        for (size_t c = 0; c < channels; ++c)
        {
            image.image_data_[i * 4 + c] = file_data[i * channels + c];
        }
        image.image_data_[i * 4 + 3] = 255;
    }
    return;
}

bool FileReader::CreateDDSTextureView(const wchar_t* file_name, ComPtr<ID3D11Device>& device,
    ComPtr<ID3D11ShaderResourceView>& ts_view)
{
    ComPtr<ID3D11Texture2D> temp_texture;

    HRESULT result = CreateDDSTextureFromFileEx(
        device.Get(), file_name, 0, D3D11_USAGE_DEFAULT,
        D3D11_BIND_SHADER_RESOURCE, 0,
        D3D11_RESOURCE_MISC_TEXTURECUBE, // Å¥ºê¸Ê¿ë ÅØ½ºÃç
        DirectX::DDS_LOADER_FLAGS(false), (ID3D11Resource**)temp_texture.GetAddressOf(),
        ts_view.GetAddressOf(), nullptr);

    if (FAILED(result)) {
        COUTERR("CreateDDSTextureFromFileEx Failed.");
        return false;
    }
    return true;
}

vector<MeshData> FileReader::GetMeshDataFromFile(string base_path, string file_name)
{
    vector<MeshData> mesh_data;

    Assimp::Importer importer;

    const string& full_path = base_path + file_name;

    const aiScene* pScene = importer.ReadFile(
        full_path,
        aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

    if (!pScene) {
        COUTERR("Failed to read file: " + file_name);
    }
    else {
        Matrix tr; // Initial transformation
        ProcessNode(mesh_data, pScene->mRootNode, pScene, tr, base_path);
    }

    return mesh_data;
}

void FileReader::ProcessNode(OUT vector<MeshData>& result, aiNode* node, const aiScene* scene, DirectX::SimpleMath::Matrix tr, const string& base_path)
{
    Matrix m;
    ai_real* temp = &node->mTransformation.a1;
    float* mTemp = &m._11;
    for (int t = 0; t < 16; t++) {
        mTemp[t] = float(temp[t]);
    }
    m = m.Transpose() * tr;

    for (UINT i = 0; i < node->mNumMeshes; i++) {

        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        MeshData new_mesh = ProcessMesh(mesh, scene, base_path);

        for (auto& v : new_mesh.vertices) {
            v.position = DirectX::SimpleMath::Vector3::Transform(v.position, m);
        }

        result.push_back(new_mesh);
    }

    for (UINT i = 0; i < node->mNumChildren; i++) {
        ProcessNode(result, node->mChildren[i], scene, m, base_path);
    }
}

MeshData FileReader::ProcessMesh(aiMesh* mesh, const aiScene* scene, const string& base_path)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    // Walk through each of the mesh's vertices
    for (UINT i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        vertex.position.x = mesh->mVertices[i].x * 0.01f;
        vertex.position.y = mesh->mVertices[i].y * 0.01f;
        vertex.position.z = mesh->mVertices[i].z * 0.01f;

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;
        vertex.normal.Normalize();

        if (mesh->mTextureCoords[0]) {
            vertex.uv.x = (float)mesh->mTextureCoords[0][i].x;
            vertex.uv.y = (float)mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }

    for (UINT i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (UINT j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    MeshData new_mesh;
    new_mesh.vertices = vertices;
    new_mesh.indices = indices;

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString filepath;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &filepath);

            std::string file_name = std::string(
                std::filesystem::path(filepath.C_Str()).filename().string());
            new_mesh.base_texture_name = base_path + file_name;
        }
    }

    return new_mesh;
}


