#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <string>

class MeshFile;
class ModelManipulator;
class ModelManipulator;
class ModelRenderer;
class NormalVectorRenderer;
class LightRenderer;

class IObject
{
public:
	IObject();
	~IObject();

protected:
	struct
	{
		uint32_t uIObjectID;
		uint32_t uiDummy[3];
	} sObjectData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpObjectBuffer;

public:
	inline const uint32_t& GetMeshID() const { return sObjectData.uIObjectID; }
	inline ID3D11Buffer* const* GetObjectBuffer() const { return cpObjectBuffer.GetAddressOf(); }

protected:
	static uint32_t uiGlobalObjectID;

protected:
	std::string strObjectName;

public:
	inline void SetObjectName(const std::string& strMeshNameIn) { strObjectName = strMeshNameIn; }
	inline const std::string& GetObjectName() const { return strObjectName; }

public:
	virtual void Draw() = 0;
	virtual void UpdateModel(const float& fDelta) = 0;

public:
	virtual void AcceptModelAsList(ModelManipulator* pModelManipulator) = 0;
	virtual void AcceptModelManipulating(ModelManipulator* pModelManipulator) = 0;
	virtual void AcceptModelRendering(ModelRenderer* pModelRenderer) = 0;
	virtual void AcceptNormalVectorRendering(NormalVectorRenderer* pNormalVectorRenderer) = 0;
	virtual void AcceptRenderingLightMap(LightRenderer* pLightRnederer) = 0;
};

