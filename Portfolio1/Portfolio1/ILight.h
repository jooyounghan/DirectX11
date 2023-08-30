#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <directxmath/DirectXMath.h>

#include <map>
#include <vector>

constexpr size_t ullMaxLightNum = 10;

enum LightType : unsigned int
{
	Directional,
	Point,
	Spot,
};

struct LightSet
{
public:
	LightType			eLightType;
	DirectX::XMVECTOR	xmvLocation;
	DirectX::XMVECTOR	xmvLightColor;
	DirectX::XMVECTOR	xmvDirection;

private:
	uint8_t				uiPadding[12];
};

class ILight
{
public:
	ILight(Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
	);
	~ILight();


protected:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContext;

protected:
	size_t ullLightId;

protected:
	static size_t ullNextLightId;
	static std::map<size_t, LightSet*> mIdToLightSet;
	static std::vector<LightSet>	vLightSets;
	static Microsoft::WRL::ComPtr<ID3D11Buffer> cpLightConstantBuffer;

public:
	static void InitLights(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	static void UpdateLights(ID3D11DeviceContext* pDeviceContext);

protected:
	void AddLightSet()
	{
		vLightSets.emplace_back();
		LightSet* pLightSet = &vLightSets.back();
		mIdToLightSet.emplace(ullLightId, pLightSet);
	}

	void RemoveLightSet()
	{
		// ªË¡¶
		LightSet* pRemoveLightSet = mIdToLightSet.at(ullLightId);

		mIdToLightSet.erase(ullLightId);
		vLightSets.erase(remove_if(vLightSets.begin(), vLightSets.end(),
			[&](LightSet& pTempLightSet) { return &pTempLightSet == pRemoveLightSet; }), vLightSets.end());
	}
};

