#include "ModelID.h"
#include "ID3D11Helper.h"

using namespace std;

unsigned int ModelID::ullCurrentModelID[3] = { 0, 0, 0 };
mutex ModelID::mtxId;


ModelID::ModelID(ID3D11Device* pDevice)
{
	AutoZeroMemory(sIdData);
	sIdData.ucModelIDStd = 0xFF;
	lock_guard<mutex> lockGuard = lock_guard<mutex>(mtxId);
	ManageOverflow(ullCurrentModelID[2], ullCurrentModelID[1], ullCurrentModelID[0], ullCurrentModelID[2]);
	memcpy(&sIdData.ucModelID, &ullCurrentModelID, sizeof(decltype(sIdData.ucModelID)));

	ID3D11Helper::CreateBuffer(
		pDevice,
		sIdData,
		D3D11_USAGE_IMMUTABLE,
		D3D11_BIND_CONSTANT_BUFFER,
		NULL,
		NULL,
		cpModelIDBuffer.GetAddressOf()
	);
}

bool ModelID::IsRGBASameWithID(const unsigned int& RGBA)
{
	return ((RGBA & 0xFF) == sIdData.ucModelID[0]) &&
		(((RGBA >> 8) & 0xFF) == sIdData.ucModelID[1]) &&
		(((RGBA >> 16) & 0xFF) == sIdData.ucModelID[2]) &&
		(((RGBA >> 24) & 0xFF) == sIdData.ucModelIDStd);
}

template<typename ...Args>
void ModelID::ManageOverflow(unsigned int& IdLower, unsigned int& IdUpper, Args & ...IdUppers)
{
	if (IdLower == 0xFF)
	{
		IdLower = 0;
		ManageOverflow(IdUpper, IdUppers...);
	}
	else
	{
		IdLower++;
	}
}

template<>
void ModelID::ManageOverflow(unsigned int& IdLower, unsigned int& IdUpper)
{
	if (IdLower == 0xFF)
	{
		IdLower = 0;
		IdUpper++;
	}
	else
	{
		IdLower++;
	}
}

