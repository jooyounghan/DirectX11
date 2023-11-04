#include "ModelID.h"
#include "ID3D11Helper.h"
#include "EnumVar.h"

using namespace std;

ModelIDData ModelID::ullCurrentModelID;
mutex ModelID::mtxId;


ModelID::ModelID(ID3D11Device* pDevice)
{
	ZeroMemory(this, sizeof(ModelID));

	lock_guard<mutex> lockGuard = lock_guard<mutex>(mtxId);
	ManageOverflow(ullCurrentModelID.ucModelID[2], ullCurrentModelID.ucModelID[1], ullCurrentModelID.ucModelID[0], ullCurrentModelID.ucModelID[2]);
	memcpy(&sIdData, &ullCurrentModelID, sizeof(decltype(sIdData)));

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

bool operator==(const ModelIDData& modelID1, const ModelIDData& modelID2)
{
	return (modelID1.ucModelID[0] == modelID2.ucModelID[0] &&
		modelID1.ucModelID[1] == modelID2.ucModelID[1] &&
		modelID1.ucModelID[2] == modelID2.ucModelID[2]);
}

ModelIDData ModelID::ConvertR8G8B8A8ToModelID(const unsigned int& RGBA)
{
	ModelIDData result;
	result.ucModelID[0] = (RGBA & 0xFF);
	result.ucModelID[1] = ((RGBA >> 8) & 0xFF);
	result.ucModelID[2] = ((RGBA >> 16) & 0xFF);
	result.ucModelIDStd = ((RGBA >> 24) & 0xFF);
	int a;
	a = 5;
	return result;
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