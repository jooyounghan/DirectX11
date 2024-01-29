#include "AStaticMesh.h"
#include "ID3D11Helper.h"
#include "DirectXDevice.h"

AStaticMesh::AStaticMesh() 
	: IModel(), ATransformerable()
{

}

AStaticMesh::~AStaticMesh() {}

void AStaticMesh::ScaleUp(const float& fXup, const float& fYUp, const float& fZUp)
{
	sScales.fScaleX += fXup;
	sScales.fScaleY += fYUp;
	sScales.fScaleZ += fZUp;

	UpdateTranformationMatrix();
	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext, 
		sTransformation, 
		D3D11_MAP_WRITE_DISCARD, 
		cpTransformationBuffer.Get()
	);
}
