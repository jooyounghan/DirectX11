#pragma once
#include <unordered_map>

#include "ImageFile.h"

class MaterialFile : public IFile
{
public:
	MaterialFile(
		const std::string& strFileLabelIn,
		const bool& bIsGLTFIn
	);
	virtual ~MaterialFile();

protected:
	struct
	{
		BOOL bIsTextureOn[TEXTURE_MAP_NUM];
		BOOL bIsGLTF;
		BOOL bDummy[2];
	} sModelTextureFlag;

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpModelTextureFlagBuffer;

public:
	inline ID3D11Buffer* const* GetPBRTextureFlagBuffer() { return cpModelTextureFlagBuffer.GetAddressOf(); }

protected:
	std::shared_ptr<IImageFile>			spModelTexture[TEXTURE_MAP_NUM];

public:
	inline void SetTextureImageFile(const EModelTextures& eModelTexture, std::shared_ptr<IImageFile> spImageFileIn) { spModelTexture[eModelTexture] = spImageFileIn; }
	inline std::shared_ptr<IImageFile>& GetTextureImageFileRef(const EModelTextures& eModelTexture) { return spModelTexture[eModelTexture]; }

public:
	void UpdateMaterial();

protected:
	static std::string								strDefaultTextureName;
	static std::unordered_map<WORD, std::string>	unmapTextureNames;

public:
	static const std::string& GetTextureName(const WORD& iTextureID);

public:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator);
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() { return nullptr; };
};

