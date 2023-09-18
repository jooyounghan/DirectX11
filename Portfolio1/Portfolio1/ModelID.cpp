#include "ModelID.h"
#include "ID3D11Helper.h"
#include "EnumVar.h"

using namespace std;

ModelID ModelID::ullCurrentModelID;
mutex ModelID::mtxId;


ModelID::ModelID()
{
	ZeroMemory(this, sizeof(ModelID));
	IdData.ucModelIDStd = 0xFF;
}

ModelID& ModelID::operator=(const ModelID& modelIDIn)
{
	memcpy(&this->IdData, &modelIDIn.IdData, sizeof(unsigned int) * 4);
	modelIDIn.cpModelIDBuffer.CopyTo(this->cpModelIDBuffer.GetAddressOf());
	return *this;
}

void ModelID::SetPsConstantBuffers(ID3D11DeviceContext* pDeviceContext)
{
	pDeviceContext->PSSetConstantBuffers(PSConstBufferType::PS_ModelID, 1, cpModelIDBuffer.GetAddressOf());
}

ModelID ModelID::ConvertR8G8B8A8ToModelID(const unsigned int& RGBA)
{
	ModelID result;
	result.IdData.ucModelID[0] = RGBA & 0xFF;
	result.IdData.ucModelID[1] = (RGBA >> 8) & 0xFF;
	result.IdData.ucModelID[2] = (RGBA >> 16) & 0xFF;
	result.IdData.ucModelIDStd = (RGBA >> 24) & 0xFF;
	return result;
}

ModelID ModelID::IssueModelID(ID3D11Device* pDevice)
{
	lock_guard<mutex> lockGuard = lock_guard<mutex>(mtxId);
	ManageOverflow(ullCurrentModelID.IdData.ucModelID[2], ullCurrentModelID.IdData.ucModelID[1], ullCurrentModelID.IdData.ucModelID[0], ullCurrentModelID.IdData.ucModelID[2]);
	ModelID resultID = ullCurrentModelID;
	ID3D11Helper::CreateBuffer(
		pDevice,
		resultID.IdData,
		D3D11_USAGE_IMMUTABLE,
		D3D11_BIND_CONSTANT_BUFFER,
		NULL,
		NULL,
		resultID.cpModelIDBuffer.GetAddressOf()
	);
	return resultID;
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

bool operator==(const ModelID& modelID1, const ModelID& modelID2)
{
	return (modelID1.IdData.ucModelID[0] == modelID2.IdData.ucModelID[0] &&
		modelID1.IdData.ucModelID[1] == modelID2.IdData.ucModelID[1] &&
		modelID1.IdData.ucModelID[2] == modelID2.IdData.ucModelID[2]);
}
