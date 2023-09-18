#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <mutex>

class ModelID
{
	friend class IModel;

public:	
	ModelID();
	ModelID& operator= (const ModelID& modelIDRight);
	friend bool operator==(const ModelID& modelID1, const ModelID& modelID2);

public:
	struct
	{
		unsigned int ucModelID[3];
		unsigned int ucModelIDStd = 0xFF;
	} IdData;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpModelIDBuffer;

public:
	void SetPsConstantBuffers(ID3D11DeviceContext* pDeviceContext);

public:
	static ModelID ConvertR8G8B8A8ToModelID(const unsigned int& RGBA);


protected:
	static ModelID ullCurrentModelID;
	static ModelID IssueModelID(ID3D11Device* pDevice);
	static std::mutex mtxId;

protected:
	template<typename ...Args>
	static void ManageOverflow(unsigned int& IdLower, unsigned int& IdUpper, Args & ...IdUppers);
	template<>
	static void ManageOverflow(unsigned int& IdLower, unsigned int& IdUpper);
};