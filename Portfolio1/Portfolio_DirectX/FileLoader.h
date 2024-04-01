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
	static std::vector<std::shared_ptr<class IFile>> LoadModelFileSet(
		const std::string& strFilePath,
		const std::string& strFileName,
		const std::string& strExtension
	);

private:
	static bool HasBones(const struct aiScene*);

private:
	inline static const std::string GetFileNameAsBoneLabel(const std::string& strFileName) { return strFileName + "_Bone"; }
	inline static const std::string GetFileNameAsAnimationLabel(const std::string& strFileName) { return strFileName + "_ANIM"; }
	inline static const std::string GetFileNameAsAnimationChannelLabel(const std::string& strFileName) { return strFileName + "_ANIM_CHANNEL"; }

private:
	static std::shared_ptr<class BoneFile> LoadBoneFile(
		const std::string strFileName, 
		const struct aiScene* pScene
	);
	static std::vector<std::shared_ptr<class MaterialFile>> LoadMaterialFile(
		const std::string strFilePath, 
		const struct aiScene* pScene,
		const bool& bIsGltfIn
	);
	static std::vector<std::shared_ptr<class AnimationFile>> LoadAnimationFile(
		const std::string strFileName,
		const struct aiScene* pScene
	);

	static std::shared_ptr<class MeshFile> LoadMeshFile(
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
		size_t& uiElementIdx,
		const bool& bIsGltf,
		const struct aiNode* pNode,
		const struct aiScene* pScene,
		const DirectX::XMMATRIX& xmMatrix,
		class MeshFile* pMeshFile
	);

	static void LoadMeshData(
		const std::string& strFilePath,
		size_t& uiElementIdx,
		const bool& bIsGltf,
		const struct aiMesh* pMesh,
		const DirectX::XMMATRIX& xmMatrix,
		const struct aiScene* pScene,
		class MeshFile* pMeshFile
	);

private:
	static size_t uiBoneId;
	static std::unordered_map<std::string, struct aiBone*> GetNodeToBoneTable(
		const struct aiScene* pScene
	);
	static void LoadBoneFromNode(
		const struct aiNode* pNode,
		class BoneFile* pBoneFile,
		struct BoneData& boneData,
		const std::unordered_map<std::string, struct aiBone*>& nodeToBoneTable
	);

public:
	static std::string ReadTextureFileName(
		const std::string& strFilePath,
		const struct aiScene* pScene,
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

