#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <vector>
#include <tuple>
#include <memory>

#include "IModel.h"
#include "ATransformerable.h"
#include "MeshFile.h"

class AStaticMesh : public IModel, public ATransformerable
{
public:
	AStaticMesh();
	virtual ~AStaticMesh();

protected:
	std::string strMeshName;

protected:
	std::shared_ptr<MeshFile> spMeshFile;

public:
	std::tuple<std::vector<UINT>, std::vector<UINT>, std::vector<ID3D11Buffer*>> GetInputInfo();

public:
	inline void SetMeshName(const std::string& strMeshNameIn) { strMeshName = strMeshNameIn; }
	inline const std::string& GetMeshName() { return strMeshName; }

public:
	virtual void Draw() override;
	virtual void UpdateModel(const float& fDelta) override;

public:
	virtual void AcceptModelManipulating(class ModelManipulator* pModelManipulator) override;
	virtual void AcceptModelRendering(class ModelRenderer* pModelRenderer) = 0;
	virtual void AcceptNormalVectorRendering(class NormalVectorRenderer* pNormalVectorRenderer) = 0;
	virtual void AcceptLightMapUpdating(class LightRenderer* pLightRnederer) = 0;

public:
	virtual void ScaleUp(const float& fXup, const float& fYUp, const float& fZUp) override final;
};

