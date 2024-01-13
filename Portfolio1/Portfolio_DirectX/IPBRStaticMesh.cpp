#include "IPBRStaticMesh.h"

IPBRStaticMesh::IPBRStaticMesh(
	ID3D11Device* pDeviceIn, ID3D11DeviceContext* pDeviceContextIn
)
	: IStaticMesh(pDeviceIn, pDeviceContextIn)
{
}

IPBRStaticMesh::~IPBRStaticMesh()
{
}
