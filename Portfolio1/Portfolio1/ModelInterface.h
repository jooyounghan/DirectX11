#pragma once
#include <memory>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <directxmath/DirectXMath.h>

#include "ManualDataType.h"

struct Vertex
{
	PositionVector	sPosVec;
	TextureCoord	sTexCoord;
	Vector			sNorVec;
	Vector			sTangVec;
};

class ModelInterface
{
public:
	ModelInterface(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn
	);
	virtual ~ModelInterface();

#pragma region Virtual
// Virtual Function ==============================================
public:
	virtual void Update(const float& fDelta) = 0;
public:
	virtual void Render() = 0;
// ==============================================================
#pragma endregion

#pragma region Interface
// Interface Function ============================================
public:
	virtual void SetIAProperties() = 0;

public:
	virtual void SetConstantBuffers() = 0;
	virtual void ResetConstantBuffers() = 0;

public:
	virtual void SetShaderResources() = 0;
	virtual void ResetShaderResources() = 0;
// ==============================================================
#pragma endregion

protected:
	ID3D11Device*			pDevice;
	ID3D11DeviceContext*	pDeviceContext;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cpIndexBuffer;
	uint32_t								ui32IndexCount;

protected:
	static void MakePlaneVertexIndexSet(
		ModelInterface* pModelInterface,
		const float& fWidth,
		const float& fHeight,
		const float& fWrapWidth,
		const float& fWrapHeight
	);

	static void MakeSquareVertexIndexSet(
		ModelInterface* pModelInterface,
		const float& fLen,
		const bool& bReverse = false
	);
	static void MakeSphereVertexIndexSet(
		ModelInterface* pModelInterface,
		const float& fRadius,
		const unsigned short& usLevel
	);
};

