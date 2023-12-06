#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <mutex>

class ModelID
{
public:	
	ModelID(ID3D11Device* pDevice);

public:
	struct {
		unsigned int ucModelID[3];
		unsigned int ucModelIDStd;
	} sIdData;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpModelIDBuffer;

public:
	inline ID3D11Buffer** GetAddressOfTextureIDBuffer() { return cpModelIDBuffer.GetAddressOf(); }

public:
	bool IsRGBASameWithID(const unsigned int& RGBA);

protected:
	static unsigned int ullCurrentModelID[3];
	static std::mutex mtxId;

protected:
	template<typename ...Args>
	static void ManageOverflow(unsigned int& IdLower, unsigned int& IdUpper, Args & ...IdUppers);
	template<>
	static void ManageOverflow(unsigned int& IdLower, unsigned int& IdUpper);
};