#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <vector>
#include <tuple>
#include "IModel.h"
#include "ATransformerable.h"

class AStaticMesh : public IModel, public ATransformerable
{
public:
	AStaticMesh();
	virtual ~AStaticMesh();

protected:
	std::vector<DirectX::XMFLOAT3>	spVertices;
	std::vector<DirectX::XMFLOAT2>	spTexcoords;
	std::vector<DirectX::XMFLOAT3>	spNormals;
	std::vector<DirectX::XMFLOAT3>	spTangents;
	std::vector<uint32_t>			spIndicesData;

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpVerticesBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpTexcoordsBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpNormalsBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpTangentsBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpIndexBuffer;

public:
	std::tuple<std::vector<UINT>, std::vector<UINT>, std::vector<ID3D11Buffer*>> GetInputInfo();

public:
	virtual void Load(const std::string& path) = 0;
	virtual void Draw() override;
	virtual void UpdateModel(const float& fDelta) override;

public:
	virtual void AcceptModelManipulating(class ModelManipulator* pModelManipulator) override;
	virtual void AcceptModelRendering(class ModelRenderer* pModelRenderer) = 0;
	virtual void AcceptNormalVectorRendering(class NormalVectorRenderer* pNormalVectorRenderer) = 0;
	virtual void AcceptLightMapUpdating(class LightRenderer* pLightRnederer) = 0;

public:
	virtual void ScaleUp(const float& fXup, const float& fYUp, const float& fZUp) override final;

public:
	static void CreateCubeModel(
		std::vector<DirectX::XMFLOAT3>&	inputVerticesIn,
		std::vector<DirectX::XMFLOAT2>&	inputTexcoordsIn,
		std::vector<DirectX::XMFLOAT3>&	inputNormalsIn,
		std::vector<DirectX::XMFLOAT3>&	inputTangentsIn,
		std::vector<uint32_t>& indexData, 
		const float& fRadius,
		const bool& bReverse,
		const uint32_t& usLevel
	);
};

