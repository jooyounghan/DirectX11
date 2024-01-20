#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <stdint.h>

class APickable
{
public:
	APickable();
	~APickable();

public:
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				cpPickedIDTexture;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView>	cpPickedIDUAV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	cpPickedIDSRV;

protected:
	struct
	{
		float fMouseXNdc;
		float fMouseYNdc;
		float fDummy[2];
	} sMousePosNdc;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpMousePosNdc;

public:
	virtual void SetMousePos(const int& iMouseX, const int& iMouseY) = 0;

public:
	uint32_t GetPickedID();
};

