#include "PickableModelInterface.h"
#include "ModelID.h"

#include "ID3D11Helper.h"
#include "MathematicalHelper.h"

using namespace std;
using namespace DirectX;

PickableModelInterface::PickableModelInterface(
	ID3D11Device* pDeviceIn,
	ID3D11DeviceContext* pDeviceContextIn
)
	: ModelInterface(pDeviceIn, pDeviceContextIn), upModelID(make_unique<ModelID>(pDevice))	
{
	AutoZeroMemory(sTransformation);
	sTransformation.sScales.fX = 1.f;
	sTransformation.sScales.fY = 1.f;
	sTransformation.sScales.fZ = 1.f;

	ID3D11Helper::CreateBuffer(
		pDevice,
		sTranformationData,
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		0,
		cpTransformationBuffer.GetAddressOf()
	);
}

PickableModelInterface::~PickableModelInterface()
{
}

void PickableModelInterface::Update(const float& fDelta)
{
	sTranformationData.xmmTransformationMatrix = MathematicalHelper::MakeAffineTransformation(
		sTransformation.sScales.fX, sTransformation.sScales.fY, sTransformation.sScales.fZ,
		sTransformation.sAngles.fPitch, sTransformation.sAngles.fYaw, sTransformation.sAngles.fRoll,
		sTransformation.sTranslations.fX, sTransformation.sTranslations.fY, sTransformation.sTranslations.fZ
	);
	sTranformationData.xmmInvTranformationMatrix = XMMatrixInverse(nullptr, sTranformationData.xmmTransformationMatrix);

	sTranformationData.xmmTransformationMatrix = XMMatrixTranspose(sTranformationData.xmmTransformationMatrix);

	ID3D11Helper::UpdateBuffer(
		pDeviceContext,
		sTranformationData,
		D3D11_MAP_WRITE_DISCARD,
		cpTransformationBuffer.Get()
	);
}

void PickableModelInterface::Render()
{
	pDeviceContext->DrawIndexed(ui32IndexCount, 0, 0);
}

void PickableModelInterface::ScaleUp(const float& x, const float& y, const float& z)
{
	sTransformation.sScales.fX += x;
	sTransformation.sScales.fY += y;
	sTransformation.sScales.fZ += z;
	Update(0.f);
}

void PickableModelInterface::MakePlaneVertexIndexSet(
	PickableModelInterface* pPickableModel,
	const float& fCenterX,
	const float& fCenterY,
	const float& fCenterZ,
	const float& fWidth,
	const float& fHeight,
	const float& fWrapWidth,
	const float& fWrapHeight
)
{
	if (pPickableModel != nullptr)
	{
		pPickableModel->sTransformation.sTranslations.fX = fCenterX;
		pPickableModel->sTransformation.sTranslations.fY = fCenterY;
		pPickableModel->sTransformation.sTranslations.fZ = fCenterZ;

		ModelInterface::MakePlaneVertexIndexSet(pPickableModel, fWidth, fHeight, fWrapWidth, fWrapHeight);
	}
}

void PickableModelInterface::MakeSquareVertexIndexSet(
	PickableModelInterface* pPickableModel, 
	const float& fCenterX, 
	const float& fCenterY, 
	const float& fCenterZ, 
	const float& fLen, 
	const bool& bReverse
)
{
	if (pPickableModel != nullptr)
	{
		pPickableModel->sTransformation.sTranslations.fX = fCenterX;
		pPickableModel->sTransformation.sTranslations.fY = fCenterY;
		pPickableModel->sTransformation.sTranslations.fZ = fCenterZ;

		ModelInterface::MakeSquareVertexIndexSet(pPickableModel, fLen, bReverse);
	}
}

void PickableModelInterface::MakeSphereVertexIndexSet(
	PickableModelInterface* pPickableModel,
	const float& fCenterX,
	const float& fCenterY,
	const float& fCenterZ,
	const float& fRadius,
	const unsigned short& usLevel
)
{
	if (pPickableModel != nullptr)
	{
		pPickableModel->sTransformation.sTranslations.fX = fCenterX;
		pPickableModel->sTransformation.sTranslations.fY = fCenterY;
		pPickableModel->sTransformation.sTranslations.fZ = fCenterZ;
		ModelInterface::MakeSphereVertexIndexSet(pPickableModel, fRadius, usLevel);
	}
}
