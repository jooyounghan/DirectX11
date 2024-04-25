#pragma once
#include <unordered_map>
#include "ImageFile.h"

class MaterialFile : public IFile, public std::enable_shared_from_this<MaterialFile>
{
public:
	MaterialFile(
		const std::string& strFileLabelIn,
		const bool& bIsGLTFIn
	);
	virtual ~MaterialFile();

protected:
	std::shared_ptr<IImageFile>			spModelTexture[TEXTURE_MAP_NUM];

private:
	struct
	{
		BOOL bIsTextureOn[TEXTURE_MAP_NUM];
		BOOL bIsGLTF;
		BOOL bDummy[2];
	} sModelTextureFlag;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpModelTextureFlagBuffer;

public:
	inline ID3D11Buffer* const* GetPBRTextureFlagBuffer() { return cpModelTextureFlagBuffer.GetAddressOf(); }

public:
	void SetTextureImageFile(const EModelTextures& eModelTexture, std::shared_ptr<IImageFile> spImageFileIn);
	inline std::shared_ptr<IImageFile>& GetTextureImageFileRef(const EModelTextures& eModelTexture) { return spModelTexture[eModelTexture]; }

protected:
	static std::string								strDefaultTextureName;
	static std::unordered_map<size_t, std::string>	unmapTextureNames;

public:
	static const std::string& GetTextureName(const size_t& iTextureID);

public:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator);
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() { return nullptr; };
};

