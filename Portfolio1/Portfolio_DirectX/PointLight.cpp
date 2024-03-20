#include "PointLight.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "LightRenderer.h"
#include "ModelRenderer.h"
#include "LightManipulator.h"

#include "ColorBlurComputeShader.h"
#include "DepthBlurComputeShader.h"

size_t PointLight::ullPointLightCnt = 0;
ID3D11RenderTargetView* PointLight::pNullRTV = nullptr;

PointLight::PointLight(
	const float& fXPos,
	const float& fYPos,
	const float& fZPos,
	const float& fLightRColorIn,
	const float& fLightGColorIn,
	const float& fLightBColorIn,
	const float& fFallOffStartIn,
	const float& fFallOffEndIn,
	const float& fLightPowerIn
) : 
	ILight(
		fXPos, fYPos, fZPos,
		fLightRColorIn,
		fLightGColorIn,
		fLightBColorIn,
		fFallOffStartIn,
		fFallOffEndIn,
		fLightPowerIn
	),
	CubeDepthStencilView(
		fXPos, fYPos, fZPos, fFallOffStartIn, fFallOffEndIn,
		gShadowMapWidth, gShadowMapHeight, 0
	),
	IMovable(fXPos, fYPos, fZPos),
	IRectangle(gShadowMapWidth, gShadowMapHeight)
{
	ullPointLightCnt++;
	ullPointLightId = ullPointLightCnt;

	pBlurCS = DepthBlurComputeShader::GetInstance();
}

PointLight::~PointLight() {}

size_t PointLight::GetLightID() { return ullPointLightId; }

void PointLight::UpdateLight()
{
	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext,
		sBaseLightData, D3D11_MAP_WRITE_DISCARD,
		cpBaseLightBuffer.Get()
	);

	pBlurCS->ApplyShader();

	/*
	TODO: Dispatch 하나로 묶어보기
	블러 7x7 수행 관련해서 3x3으로바꿔보기
	그림자 7x7 수행 관련해서 3x3 또는 5x5로 바꿔보기
	*/ 
	for (size_t idx = 0; idx < EDirections::DirectionNum; ++idx)
	{
		cubeViewParts[idx].SetFarZ(sBaseLightData.fFallOffEnd);
		DirectX::XMVECTOR& pos = cubeViewParts[idx].GetPosition();
		memcpy(pos.m128_f32, xmvPosition.m128_f32, sizeof(float) * 4);
		cubeViewParts[idx].UpdatePosition();
		cubeViewParts[idx].UpdateViewToPerspective();

		pBlurCS->SetShader(cubeViewParts[idx].GetAddressOfSRV(), cpUAVs[idx].GetAddressOf());
		DirectXDevice::pDeviceContext->Dispatch(
			uiWidth / 200, uiHeight / 4, 1
		);

		pBlurCS->ResetShader();
	}
	pBlurCS->DisapplyShader();
}


void PointLight::SetDepthOnlyRenderTarget(const size_t& idx)
{
	cubeViewParts[idx].SetDepthOnlyRenderTarget();
}

void PointLight::ResetDepthOnlyRenderTarget()
{
	DirectXDevice::pDeviceContext->OMSetRenderTargets(1, &pNullRTV, nullptr);
	DirectXDevice::pDeviceContext->RSSetViewports(1, &Viewable::GetNullViewPort());
}

void PointLight::AcceptUpdatingLightMap(LightRenderer* pLightRenderer)
{
	pLightRenderer->SetForUpdatingLightMap(*this);
}

void PointLight::AcceptPBRDirectLighting(ModelRenderer* pModelRenderer)
{
	pModelRenderer->RenderWithLight(*this);
}

void PointLight::AcceptLightList(LightManipulator* pLightManipulator)
{
	pLightManipulator->VisitLightList(*this);
}

void PointLight::AcceptLightSetting(LightManipulator* pLightManipulator)
{
	pLightManipulator->VisitLightSetting(*this);
}

