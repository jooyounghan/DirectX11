#pragma once
#include <filesystem>
#include <Windows.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <DirectXMath.h>
#include <set>

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

private:
	inline static const std::string GetFileNameAsLabel(
		const std::string& strFileName,
		const std::string& strExtension
	)
	{
		std::string labelType = strExtension;
		replace(labelType.begin(), labelType.end(), '.', '_');
		transform(labelType.begin(), labelType.end(), labelType.begin(), ::toupper);
		return strFileName + labelType;
	}

public:
	static std::vector<std::shared_ptr<class IFile>> LoadModelFile(
		const std::string& strFilePath,
		const std::string& strFileName,
		const std::string& strExtension
	);

private:
	static bool HasBones(const struct aiScene*);
	inline static const std::string GetFileNameAsSkeletalLabel(const std::string& strFileName) { return strFileName + "_SKELETAL"; }

private:
	static std::shared_ptr<IFile> LoadSkeletalFile(
		const std::string strFileName, 
		const struct aiScene* pScene
	);
	static std::vector<std::shared_ptr<IFile>> LoadMaterialFile(
		const std::string strFilePath, 
		const struct aiScene* pScene,
		const bool& bIsGltfIn
	);
	static std::shared_ptr<IFile> LoadModelFile(
		const std::string& strFilePath, 
		const std::string& strFileName, 
		const std::string& strExtension,
		const bool& bIsGltf,
		const struct aiScene* pScene
	);

private:
	static void ProcessNode(
		const std::string& strFilePath,
		const std::string& strFileLabel,
		uint8_t& ucElementIdx,
		const bool& bIsGltf,
		const struct aiNode* pNode,
		const struct aiScene* pScene,
		const DirectX::XMMATRIX& xmMatrix,
		class ModelFile* pModelFile
	);

	static std::shared_ptr<class MeshFile> LoadMeshFile(
		const std::string& strFilePath,
		uint8_t& ucElementIdx,
		const bool& bIsGltf,
		const struct aiMesh* pMesh,
		const DirectX::XMMATRIX& xmMatrix,
		const struct aiScene* pScene
	);

private:
	static void UpdateBoneNameSet(
		const struct aiScene* pScene, 
		std::set<std::string>& setBoneInformation
	);
	static void LoadBoneFromNode(
		const struct aiNode* pNode,
		class Bone* pBoneParent,
		const std::set<std::string>& setBoneInformation
	);

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
	static std::shared_ptr<class ModelFile> LoadDefaultCubeModel(
		const bool& bReverse
	);
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

