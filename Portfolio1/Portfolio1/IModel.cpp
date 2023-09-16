#include "IModel.h"
#include "ID3D11Helper.h"
#include "MathematicalHelper.h"

using namespace std;
using namespace DirectX;

ModelID ModelID::ullCurrentModelID;
std::mutex ModelID::mtxId;

void TextureSet::CreateTextureAndSRV(
	IN TextureType eTextureType,
	IN ID3D11Device* pDevice,
	IN ImageContainer* pImageContainer
)
{
	ID3D11Texture2D** ppTexture2D = nullptr;
	ID3D11ShaderResourceView** ppSRV = nullptr;
	switch (eTextureType)
	{
	case TEXTURE_AO:
		ppTexture2D = AOTexture2D.GetAddressOf();
		ppSRV = AOSRV.GetAddressOf();
		break;
	case TEXTURE_DIFFUSE:
		ppTexture2D = DiffuseTexture2D.GetAddressOf();
		ppSRV = DiffuseSRV.GetAddressOf();
		break;
	case TEXTURE_HEIGHT:
		ppTexture2D = HeightTexture2D.GetAddressOf();
		ppSRV = HeightSRV.GetAddressOf();
		break;
	case TEXTURE_NORMAL:
		ppTexture2D = NormalTexture2D.GetAddressOf();
		ppSRV = NormalSRV.GetAddressOf();
		break;
	case TEXTURE_REFLECT:
		ppTexture2D = ReflectTexture2D.GetAddressOf();
		ppSRV = ReflectSRV.GetAddressOf();
		break;
	}
	ID3D11Helper::CreateTexture2D(pDevice, pImageContainer, ppTexture2D);
	ID3D11Helper::CreateShaderResoureView(pDevice, *ppTexture2D, ppSRV);
}

IModel::IModel(ComPtr<ID3D11Device>& cpDeviceIn, ComPtr<ID3D11DeviceContext>& cpDeviceContextIn)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn)
{
	ModelTransform::Init(&sModelTransformation);

	ID3D11Helper::CreateBuffer(
		cpDevice.Get(),
		TransformedMatrix::CreateTransfomredMatrix(ModelTransform::GetAffineTransformMatrix(sModelTransformation)),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		0,
		cpModelMatrixBuffer.GetAddressOf()
	);

	SetModelID();
}

void IModel::SetModelID()
{
	{
		lock_guard<mutex> lockGuard = lock_guard<mutex>(ModelID::mtxId);
		ullModelID = ModelID::IssueModelID();
	}

	ID3D11Helper::CreateBuffer(
		cpDevice.Get(),
		ullModelID,
		D3D11_USAGE_IMMUTABLE,
		D3D11_BIND_CONSTANT_BUFFER,
		NULL,
		NULL,
		cpModelIDBuffer.GetAddressOf()
	);
}

ModelID::ModelID()
{
	ZeroMemory(this, sizeof(ModelID));
	ucModelIDStd = 0xFF;
}

ModelID& ModelID::operator=(const ModelID& modelIDIn)
{
	memcpy(this, &modelIDIn, sizeof(ModelID));
	return *this;
}

 ModelID ModelID::ConvertR8G8B8A8ToModelID(const unsigned int& RGBA)
{
	ModelID result;
	result.ucModelID[0] = RGBA & 0xFF;
	result.ucModelID[1] = (RGBA >> 8) & 0xFF;
	result.ucModelID[2] = (RGBA >> 16) & 0xFF;
	result.ucModelIDStd = (RGBA >> 24) & 0xFF;
	return result;
}

ModelID ModelID::IssueModelID()
{
	ManageOverflow(ullCurrentModelID.ucModelID[2], ullCurrentModelID.ucModelID[1], ullCurrentModelID.ucModelID[0], ullCurrentModelID.ucModelID[2]);
	ModelID resultID = ullCurrentModelID;
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
	return (modelID1.ucModelID[0] == modelID2.ucModelID[0] && modelID1.ucModelID[1] == modelID2.ucModelID[1] && modelID1.ucModelID[2] == modelID2.ucModelID[2]);
}
