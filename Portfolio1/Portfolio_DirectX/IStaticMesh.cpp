#include "IStaticMesh.h"

IStaticMesh::IStaticMesh(
	ID3D11Device* pDeviceIn, 
	ID3D11DeviceContext* pDeviceContextIn
) 
	: IModel(), IMovable(pDeviceIn, pDeviceContextIn), IScalable()
{

}

IStaticMesh::~IStaticMesh() {}
