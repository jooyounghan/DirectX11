#include "IModel.h"
#include "ID3D11Helper.h"
#include "MathematicalHelper.h"

using namespace std;
using namespace DirectX;

unsigned int IModel::ullCurrentModelID = 0;
std::mutex IModel::mtxId;

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
		lock_guard<mutex> lockGuard = lock_guard<mutex>(mtxId);
		AutoZeroMemory(ullModelID);
		ullModelID.ullModelID = IModel::ullCurrentModelID;
		IModel::ullCurrentModelID += 0x00FF;
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
