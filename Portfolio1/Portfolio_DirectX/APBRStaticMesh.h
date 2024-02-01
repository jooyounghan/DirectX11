#pragma once
#include <memory>
#include "AStaticMesh.h"
#include <unordered_map>

class APBRStaticMesh : public AStaticMesh
{
public:
	enum EModelTextures : WORD
	{
		AO_TEXUTRE_MAP,
		COLOR_TEXTURE_MAP,
		METALNESS_TEXTURE_MAP,
		ROUGHNESS_TEXTURE_MAP,
		EMISSION_TEXTURE_MAP,
		NORMAL_TEXTURE_MAP,
		HEIGHT_TEXTURE_MAP,
		TEXTURE_MAP_NUM
	};

public:
	static std::unordered_map<WORD, std::string> unmapTextureNames;

public:
	APBRStaticMesh();
	virtual ~APBRStaticMesh();

public:
	std::shared_ptr<class NormalImageFile> pModelTexture[TEXTURE_MAP_NUM];

public:
	struct
	{
		float fFrenelConstant[3];
		float fHeightFactor;
	} sPBRConstant;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpPBRConstantBuffer;


public:
	virtual void Load(const std::string& path) = 0;
	virtual void UpdateModel(const float& fDelta);

public:
	virtual void AcceptModelManipulator(class ModelManipulator* pModelManipulator);
};

