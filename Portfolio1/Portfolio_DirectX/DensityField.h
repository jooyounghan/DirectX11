#pragma once
#include "AFilter.h"
#include "StructuredBuffer.h"

#include "IVertexShader.h"
#include "IComputeShader.h"
#include "IPixelShader.h"

template<typename T>
class DensityField : public AFilter
{
public:
	DensityField(
		UINT uiWidthIn,
		UINT uiHeightIn,
		UINT uiThreadGroupCntXIn,
		UINT uiThreadGroupCntYIn,
		UINT uiThreadGroupCntZIn
	);
	~DensityField();

protected:
	StructuredBuffer<T> mDensityInput;

protected:
	IVertexShader* pVS;
	IPixelShader* pPS;
	IComputeShader* pSourcingCS;
	IComputeShader* pDissipationCS;

public:
	void Excute(void* pSourceCSBinding, void* pVSBinding, void* pPSBinding, void* pDissiCSBinding);
};

template<typename T>
inline DensityField<T>::DensityField(
	UINT uiWidthIn,
	UINT uiHeightIn,
	UINT uiThreadGroupCntXIn,
	UINT uiThreadGroupCntYIn,
	UINT uiThreadGroupCntZIn
)
	: AFilter(
		uiWidthIn,
		uiHeightIn,
		1, 0,
		NULL, NULL, NULL,
		D3D11_USAGE_DEFAULT,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		uiThreadGroupCntXIn,
		uiThreadGroupCntYIn,
		uiThreadGroupCntZIn
	)
{
}

template<typename T>
inline DensityField<T>::~DensityField()
{
}

template<typename T>
inline void DensityField<T>::Excute(void* pSourceCSBinding, void* pVSBinding, void* pPSBinding, void* pDissiCSBinding)
{
	pDissipationCS->ApplyShader();
	pDissipationCS->SetShader(pDissiCSBinding);
	pDissipationCS->ResetShader();
	pDissipationCS->DisapplyShader();

	pSourcingCS->ApplyShader();
	pSourcingCS->SetShader(pSourceCSBinding);
	pSourcingCS->ResetShader();
	pSourcingCS->DisapplyShader();

	pVS->ApplyShader();
	pVS->SetShader(pVSBinding);
	pVS->ResetShader();
	pVS->DisapplyShader();

	pPS->ApplyShader();
	pPS->SetShader(pPSBinding);
	pPS->ResetShader();
	pPS->DisapplyShader();
}
