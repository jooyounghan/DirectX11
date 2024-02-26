#pragma once
#include <filesystem>
#include <Windows.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <DirectXMath.h>

class FileLoader
{
private:
	static std::unordered_map<std::string, std::weak_ptr<class IFile>> mapUsingFiles;

public:
	uint8_t* LoadFileWithStbi(const char* pFilename, UINT* x, UINT* y, UINT* comp);
	uint8_t* LoadFileWithStbi(const wchar_t* wFilename, UINT* x, UINT* y, UINT* comp);
	void	FreeLoadedFileData(uint8_t* pRawData);

public:
	uint8_t* LoadFileWithOpenEXR(const char* pFileName, UINT* x, UINT* y, UINT* comp);
	uint8_t* LoadFileWithOpenEXR(const wchar_t* pFileName, UINT* x, UINT* y, UINT* comp);

private:
	static void AddUsingFile(const std::string& strFileName, std::shared_ptr<IFile> spFile);
	static std::shared_ptr<IFile> GetUsingFile(const std::string& strFileName);

public:
	std::shared_ptr<class IFile> LoadImageFile(
		const std::string& strExtension, 
		const std::string& strFilePath,
		const std::string& strFileName, 
		UINT* x, UINT* y, UINT* comp
	);
	std::shared_ptr<class IFile> LoadModelFile(
		const std::string& strExtension,
		const std::string& strFilePath,
		const std::string& strFileName
	);

private:
	std::string strModelFilePath;

private:
	void ProcessNode(
		const bool& bIsGltf,
		struct aiNode* pNode,
		const struct aiScene* pScene,
		DirectX::XMMATRIX& xmMatrix,
		std::vector<class MeshData>& vMeshes 
	);

	class MeshData ProcessMesh(
		const bool& bIsGltf,
		struct aiMesh* pMesh,
		const struct aiScene* pScene
	);

public:
	std::string ReadTextureFileName(
		const aiScene* pScene,
		struct aiMaterial* pMaterial,
		enum aiTextureType eType
	);

public:
	std::wstring GetLastDirectoryName(const std::filesystem::path& filePath);

public:
	static std::wstring ConvertUTF8ToUniCode(const std::string& str);
	static std::string ConvertUniCodeToUTF8(const std::wstring& wStr);

public:
	template<typename ...Args>
	static void IsStrSame(bool* pResult, const std::string& ref, const std::string& s, Args... args)
	{
		if (ref == s) *pResult = true;
		return IsStrSame(pResult, ref, args...);
	}

	static void IsStrSame(bool* pResult, const std::string& ref, const std::string& s)
	{
		if (ref == s) *pResult = true;
	}
};

