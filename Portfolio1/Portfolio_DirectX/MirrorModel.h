#pragma once
#include "IMesh.h"
#include "ViewableRenderTarget.h"
#include "ViewableDepthStencil.h"
#include "ATransformerable.h"

constexpr float gMirrorFovDeg = 90.f;
constexpr float gMirrorNearZ = 0.001f;
constexpr float gMirrorFarZ = 1000.f;
constexpr UINT gMirrorTextureWidth = 1000;
constexpr UINT gMirrorTextureHeight = 1000;

class MirrorModel : 
	public IMesh, 
	public ViewableRenderTarget,
	public ViewableDepthStencil, 
	public ATransformerable
{
public:
	MirrorModel(
		const float& fMirrorWidth,
		const float& fMirrorHeight,
		const float& fXPos,
		const float& fYPos,
		const float& fZPos,
		const float& fPitchDegIn,
		const float& fYawDegIn,
		const float& fRollDegIn,
		const float& fWidthIn = gMirrorTextureWidth,
		const float& fHeightIn = gMirrorTextureHeight,
		const float& fFovDegreeIn = gMirrorFovDeg,
		const float& fNearZIn = gMirrorNearZ,
		const float& fFarZIn =gMirrorFarZ
	);
	virtual ~MirrorModel();

protected:
	static uint32_t uiMirrorModelIdx;

protected:
	struct
	{
		float Alpha;
		float fDummy[3];
	} sMirrorProperties;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpMirrorProperties;

public:
	inline float* GetAlphaAddress() { return &sMirrorProperties.Alpha; }
	inline ID3D11Buffer* const* GetMirrorPropertiesBuffer() { return cpMirrorProperties.GetAddressOf(); }

public:
	virtual void Draw() override;

public:
	void SetAsRenderTarget();
	void ResetAsRenderTarget();

public:
	virtual void Resize(const UINT& uiWidthIn, const UINT& uiHeightIn) override;
	virtual void UpdateModel(const float& fDelta) override;

public:
	virtual void AcceptModelAsList(ModelManipulator* pModelManipulator) override;
	virtual void AcceptModelManipulating(class ModelManipulator* pModelManipulator) override;
	virtual void AcceptModelRendering(class ModelRenderer* pModelRenderer) override;
	virtual void AcceptNormalVectorRendering(class NormalVectorRenderer* pNormalVectorRenderer) override;
	virtual void AcceptRenderingLightMap(class LightRenderer* pLightRnederer) override;
};

