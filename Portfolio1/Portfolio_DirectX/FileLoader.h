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
	static uint8_t* LoadFileWithStbi(const char* pFilename, UINT* x, UINT* y, UINT* comp);
	static uint8_t* LoadFileWithStbi(const wchar_t* wFilename, UINT* x, UINT* y, UINT* comp);
	static void	FreeLoadedFileData(uint8_t* pRawData);

public:
	static uint8_t* LoadFileWithOpenEXR(const char* pFileName, UINT* x, UINT* y, UINT* comp);
	static uint8_t* LoadFileWithOpenEXR(const wchar_t* pFileName, UINT* x, UINT* y, UINT* comp);

private:
	static void AddUsingFile(const std::string& strFileNameWithExt, std::shared_ptr<IFile> spFile);
	static std::shared_ptr<IFile> GetUsingFile(const std::string& strFileNameWithExt);

public:
	static std::shared_ptr<class IImageFile> LoadImageFile(
		const std::string& strFilePath,
		const std::string& strFileName, 
		const std::string& strExtension, 
		UINT* x, UINT* y, UINT* comp
	);
	static std::vector<std::shared_ptr<class IFile>> LoadModelFile(
		const std::string& strFilePath,
		const std::string& strFileName,
		const std::string& strExtension
	);

private:
	static void ProcessNode(
		const std::string& strFilePath,
		const std::string& strFileName,
		const std::string& strExtension,
		const bool& bIsGltf,
		struct aiNode* pNode,
		const struct aiScene* pScene,
		DirectX::XMMATRIX& xmMatrix,
		class ModelFile* pModelFile
	);

	static struct MeshFileSet ProcessMesh(
		const std::string& strFilePath,
		const std::string& strFileName,
		const std::string& strExtension,
		const bool& bIsGltf,
		struct aiMesh* pMesh,
		DirectX::XMMATRIX& xmMatrix,
		const struct aiScene* pScene
	);

private:
	static std::unordered_map<std::string, int> GetBoneInformation(const struct aiScene* pScene);


public:
	static std::string ReadTextureFileName(
		const std::string& strFilePath,
		const aiScene* pScene,
		struct aiMaterial* pMaterial,
		enum aiTextureType eType
	);

public:
	static std::wstring GetLastDirectoryName(const std::filesystem::path& filePath);

public:
	static std::wstring ConvertUTF8ToUniCode(const std::string& str);
	static std::string ConvertUniCodeToUTF8(const std::wstring& wStr);

public:
	static std::shared_ptr<class MeshFile> LoadDefaultCubeMesh(
		const bool& bReverse
	);
	static std::shared_ptr<class MeshFile> LoadPlaneMesh(
		const float& fMirrorWidthIn,
		const float& fMirrorHeightIn
	);

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

