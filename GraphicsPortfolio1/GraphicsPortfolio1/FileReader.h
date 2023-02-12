#pragma once

#include <d3d11.h>
#include <wrl.h>

#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>

#include "MeshData.h"

#define COUTERR(String) std::cout << String << "/" << "Error Code : " << GetLastError() << std::endl

using Microsoft::WRL::ComPtr;
using std::vector;
using std::string;

struct Image
{
	vector<uint8_t> image_data_;
	int width;
	int height;
	int channels;
};

class FileReader
{
public:
	static void GetImage(const string& file_path, OUT Image& image);

public:
	static bool CreateDDSTextureView(const wchar_t* file_name, ComPtr<ID3D11Device>& device,
		ComPtr<ID3D11ShaderResourceView>& ts_view);

public:
	static vector<MeshData>	GetMeshDataFromFile(string base_path, string file_name);

private:
	static void ProcessNode(OUT vector<MeshData>& result, aiNode* node, const aiScene* scene,
		DirectX::SimpleMath::Matrix tr, const string& base_path);
	static MeshData ProcessMesh(aiMesh* mesh, const aiScene* scene, const string& base_path);

};

