#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <mutex>

struct ModelIDData
{
	friend bool operator == (const ModelIDData& modelId1, const ModelIDData& modelId2);

	unsigned int ucModelID[3];
	unsigned int ucModelIDStd = 0xFF;
};

class ModelID
{
public:	
	ModelID(ID3D11Device* pDevice);
	//ModelID& operator= (const ModelID& modelIDRight);
	//friend bool operator==(const ModelID& modelID1, const ModelID& modelID2);

public:
	ModelIDData								sIdData;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpModelIDBuffer;

public:
	void SetPsConstantBuffers(ID3D11DeviceContext* pDeviceContext);

public:
	static ModelIDData ConvertR8G8B8A8ToModelID(const unsigned int& RGBA);

protected:
	static ModelIDData ullCurrentModelID;
	static std::mutex mtxId;

protected:
	template<typename ...Args>
	static void ManageOverflow(unsigned int& IdLower, unsigned int& IdUpper, Args & ...IdUppers);
	template<>
	static void ManageOverflow(unsigned int& IdLower, unsigned int& IdUpper);
};