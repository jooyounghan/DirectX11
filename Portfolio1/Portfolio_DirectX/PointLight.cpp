#include "PointLight.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

#include "LightRenderer.h"
#include "ModelRenderer.h"
#include "LightManipulator.h"

#include "ColorBlurComputeShader.h"
#include "PointLightDepthBlurComputeShader.h"

using namespace std;

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

	pBlurCS = PointLightDepthBlurComputeShader::GetInstance();
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

	vector<ID3D11ShaderResourceView*> srvs;
	vector<ID3D11UnorderedAccessView*> uavs;

	for (size_t idx = 0; idx < EDirections::DirectionNum; ++idx)
	{
		cubeViewParts[idx].SetFarZ(sBaseLightData.fFallOffEnd);
		DirectX::XMVECTOR& pos = cubeViewParts[idx].GetPosition();
		memcpy(pos.m128_f32, xmvPosition.m128_f32, sizeof(float) * 4);
		cubeViewParts[idx].UpdatePosition();
		cubeViewParts[idx].UpdateViewToPerspective();

		srvs.push_back(cubeViewParts[idx].GetSRV());
		uavs.push_back(cpUAVs[idx].Get());
	}

	PointDepthBlurCSBindingSet pBinding;
	pBinding.ppInputSRV = srvs.data();
	pBinding.ppOutputUAV = uavs.data();
	pBinding.uiIndex = EDirections::DirectionNum;

	pBlurCS->SetShader(&pBinding);

	const UINT xNumThread = 26;
	const UINT yNumThread = 26;

	DirectXDevice::pDeviceContext->Dispatch(
		uiWidth % xNumThread ? uiWidth / xNumThread + 1 : uiWidth / xNumThread,
		uiHeight % yNumThread ? uiHeight / yNumThread + 1 : uiHeight / yNumThread,
		1
	);

	pBlurCS->ResetShader();
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

