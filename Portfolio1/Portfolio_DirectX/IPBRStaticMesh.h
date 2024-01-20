#pragma once
#include <memory>
#include "AStaticMesh.h"

enum ModelTextures : WORD
{
	AO_TEXUTRE_MAP,
	COLOR_TEXTURE_MAP,
	METALNESS_TEXTURE_MAP,
	ROUGHNESS_TEXTURE_MAP,
	EMISSION_TEXTURE_MAP,
	NORMAL_TEXTURE_MAP,
	TEXTURE_MAP_NUM
};

class IPBRStaticMesh : public AStaticMesh
{
public:
	IPBRStaticMesh();
	virtual ~IPBRStaticMesh();

public:
	std::shared_ptr<class NormalImageFile> pModelTexture[TEXTURE_MAP_NUM];
	std::shared_ptr<class NormalImageFile> pHeightTexture;

public:
	virtual void Load(const std::string& path) = 0;
	virtual void Draw() = 0;
	virtual void UpdateModel(const float& fDelta) = 0;
};

