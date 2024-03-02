#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>


#include <string>

class IModel
{
public:
	IModel();
	~IModel();

public:
	static uint32_t uiGlobalModelID;

public:
	struct
	{
		uint32_t uiModelID;
		uint32_t uiDummy[3];
	} sModelData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpIdBuffer;

public:
	virtual void Draw() = 0;
	virtual void UpdateModel(const float& fDelta) = 0;

public:
	virtual void AcceptModelManipulating(class ModelManipulator* pModelManipulator) = 0;
};

