#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <string>
#include <memory>

class MeshFile;
class ModelManipulator;
class ModelManipulator;
class ModelRenderer;
class NormalVectorRenderer;
class LightRenderer;

class IMesh
{
public:
	IMesh();
	~IMesh();

private:
	static uint32_t uiGlobalModelID;

protected:
	std::string					strMeshName;
	std::shared_ptr<MeshFile>		spMeshFile;

public:
	inline std::shared_ptr<MeshFile>& GetMeshFileRef() { return spMeshFile; };

public:
	inline void SetMeshName(const std::string& strMeshNameIn) { strMeshName = strMeshNameIn; }
	inline const std::string& GetMeshName() const { return strMeshName; }

protected:
	struct
	{
		uint32_t uIMeshID;
		uint32_t uiDummy[3];
	} sModelData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpIdBuffer;

public:
	inline const uint32_t& GetMeshID() const { return sModelData.uIMeshID; }
	inline ID3D11Buffer* const* GetIDBuffer() const { return cpIdBuffer.GetAddressOf(); }

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
