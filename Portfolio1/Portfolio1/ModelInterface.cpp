#include "ModelInterface.h"
#include "EnumVar.h"
#include "FileLoader.h"
#include "ID3D11Helper.h"

#include <atomic>

using namespace std;

ModelInterface::ModelInterface(
	ComPtr<ID3D11Device>& cpDeviceIn,
	ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
	const float& fCenterX,
	const float& fCenterY,
	const float& fCenterZ,
	const float& fLen
)
	: cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn), modelID(cpDevice.Get())
{
	vector<uint32_t> vIndex{
		0, 1, 2,
		3, 4, 5,

		6, 7, 8,
		9, 10, 11,
		
		12, 13, 14,
		15, 16, 17,

		18, 19, 20,
		21, 22, 23,

		24, 25, 26,
		27, 28, 29,

		30, 31, 32,
		33, 34, 35,
	};
	ui32IndexCount = UINT(vIndex.size());

	sTransformationProperties.xmvTranslation.m128_f32[0] = fCenterX;
	sTransformationProperties.xmvTranslation.m128_f32[1] = fCenterY;
	sTransformationProperties.xmvTranslation.m128_f32[2] = fCenterZ;

	vector<Vertex> vVertex{
		{{-fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {0.f, 1.f}, {0.f, 0.f, -1.f, 0.f}},
		{{fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {1.f, 0.f}, {0.f, 0.f, -1.f, 0.f}},
		{{fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {1.f, 1.f}, {0.f, 0.f, -1.f, 0.f}},
		{{-fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {0.f, 1.f}, {0.f, 0.f, -1.f, 0.f}},
		{{-fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {0.f, 0.f}, {0.f, 0.f, -1.f, 0.f}},
		{{fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {1.f, 0.f}, {0.f, 0.f, -1.f, 0.f}},


		{{fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {0.f, 1.f}, {1.f, 0.f, 0.f, 0.f}},
		{{fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {0.f, 0.f}, {1.f, 0.f, 0.f, 0.f}},
		{{fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {1.f, 1.f}, {1.f, 0.f, 0.f, 0.f}},
		{{fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {1.f, 1.f}, {1.f, 0.f, 0.f, 0.f}},
		{{fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {0.f, 0.f}, {1.f, 0.f, 0.f, 0.f}},
		{{fLen / 2.f, fLen / 2.f, fLen / 2.f}, {1.f, 0.f}, {1.f, 0.f, 0.f, 0.f}},


		{{-fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {1.f, 1.f}, {-1.f, 0.f, 0.f, 0.f}},
		{{-fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {0.f, 1.f}, {-1.f, 0.f, 0.f, 0.f}},
		{{-fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {1.f, 0.f}, {-1.f, 0.f, 0.f, 0.f}},
		{{-fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {0.f, 1.f}, {-1.f, 0.f, 0.f, 0.f}},
		{{-fLen / 2.f, fLen / 2.f, fLen / 2.f}, {0.f, 0.f}, {-1.f, 0.f, 0.f, 0.f}},
		{{-fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {1.f, 0.f}, {-1.f, 0.f, 0.f, 0.f}},


		{{-fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {0.f, 0.f}, {0.f, -1.f, 0.f, 0.f}},
		{{fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {1.f, 0.f}, {0.f, -1.f, 0.f, 0.f}},
		{{fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {1.f, 1.f}, {0.f, -1.f, 0.f, 0.f}},
		{{-fLen / 2.f, -fLen / 2.f, -fLen / 2.f}, {0.f, 0.f}, {0.f, -1.f, 0.f, 0.f}},
		{{fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {1.f, 1.f}, {0.f, -1.f, 0.f, 0.f}},
		{{-fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {0.f, 1.f}, {0.f, -1.f, 0.f, 0.f}},


		{{-fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {0.f, 1.f}, {0.f, 1.f, 0.f, 0.f}},
		{{-fLen / 2.f, fLen / 2.f, fLen / 2.f}, {0.f, 0.f}, {0.f, 1.f, 0.f, 0.f}},
		{{fLen / 2.f, fLen / 2.f, fLen / 2.f}, {1.f, 0.f}, {0.f, 1.f, 0.f, 0.f}},
		{{-fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {0.f, 1.f}, {0.f, 1.f, 0.f, 0.f}},
		{{fLen / 2.f, fLen / 2.f, fLen / 2.f}, {1.f, 0.f}, {0.f, 1.f, 0.f, 0.f}},
		{{fLen / 2.f, fLen / 2.f, -fLen / 2.f}, {1.f, 1.f}, {0.f, 1.f, 0.f, 0.f}},


		{{-fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {1.f, 1.f}, {0.f, 0.f, 1.f, 0.f}},
		{{fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {0.f, 1.f}, {0.f, 0.f, 1.f, 0.f}},
		{{-fLen / 2.f, fLen / 2.f, fLen / 2.f}, {1.f, 0.f}, {0.f, 0.f, 1.f, 0.f}},
		{{fLen / 2.f, -fLen / 2.f, fLen / 2.f}, {0.f, 1.f}, {0.f, 0.f, 1.f, 0.f}},
		{{fLen / 2.f, fLen / 2.f, fLen / 2.f}, {0.f, 0.f}, {0.f, 0.f, 1.f, 0.f}},
		{{-fLen / 2.f, fLen / 2.f, fLen / 2.f}, {1.f, 0.f}, {0.f, 0.f, 1.f, 0.f}}
	};

	ID3D11Helper::CreateBuffer(
		cpDevice.Get(),
		TransformationBufferData::CreateTransfomredMatrix(TransformProperties::GetAffineTransformMatrix(sTransformationProperties)),
		D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER,
		D3D11_CPU_ACCESS_WRITE,
		0,
		cpTransformationDataBuffer.GetAddressOf()
	);

	ID3D11Helper::CreateBuffer(cpDevice.Get(), vIndex, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0, 0, cpIndexBuffer.GetAddressOf());
	ID3D11Helper::CreateBuffer(cpDevice.Get(), vVertex, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0, 0, cpVertexBuffer.GetAddressOf());

	sTextures.CreateTextureAndSRV(TextureType::TEXTURE_AO, cpDevice.Get(), cpDeviceContext.Get(), &ImageContainer::ExampleTextureAO);
	sTextures.CreateTextureAndSRV(TextureType::TEXTURE_DIFFUSE, cpDevice.Get(), cpDeviceContext.Get(), &ImageContainer::ExampleTextureDiffuse);
	sTextures.CreateTextureAndSRV(TextureType::TEXTURE_HEIGHT, cpDevice.Get(), cpDeviceContext.Get(), &ImageContainer::ExampleTextureHeight);
	sTextures.CreateTextureAndSRV(TextureType::TEXTURE_NORMAL, cpDevice.Get(), cpDeviceContext.Get(), &ImageContainer::ExampleTextureNormal);
	sTextures.CreateTextureAndSRV(TextureType::TEXTURE_REFLECT, cpDevice.Get(), cpDeviceContext.Get(), &ImageContainer::ExampleTextureReflection);
}

void ModelInterface::Update()
{
	ID3D11Helper::UpdateBuffer(
		cpDeviceContext.Get(),
		TransformationBufferData::CreateTransfomredMatrix(TransformProperties::GetAffineTransformMatrix(sTransformationProperties)),
		D3D11_MAP_WRITE_DISCARD,
		cpTransformationDataBuffer.Get()
	);
}

void ModelInterface::Render()
{
	cpDeviceContext->DrawIndexed(ui32IndexCount, 0, 0);
}

void ModelInterface::SetIAProperties()
{
	cpDeviceContext->IASetIndexBuffer(cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	cpDeviceContext->IASetVertexBuffers(0, 1, cpVertexBuffer.GetAddressOf(), &stride, &offset);
}

void ModelInterface::SetVSConstantBuffer()
{
	cpDeviceContext->VSSetConstantBuffers(VSConstBufferType::VS_ModelMatrix, 1, cpTransformationDataBuffer.GetAddressOf());
}

void ModelInterface::SetHSConstantBuffer()
{
}

void ModelInterface::SetDSConstantBuffer()
{
}

void ModelInterface::SetPSConstantBuffer()
{
	modelID.SetPsConstantBuffers(cpDeviceContext.Get());
}

void ModelInterface::SetVSShaderResources()
{
}

void ModelInterface::SetHSShaderResources()
{
}

void ModelInterface::SetDSShaderResources()
{
	cpDeviceContext->DSSetShaderResources(DSSRVType::DS_HEIGHT, 1, sTextures.HeightSRV.GetAddressOf());
}

void ModelInterface::SetPSShaderResources()
{
	cpDeviceContext->PSSetShaderResources(PSSRVType::PS_AO, 1, sTextures.AOSRV.GetAddressOf());
	cpDeviceContext->PSSetShaderResources(PSSRVType::PS_DIFFUSE, 1, sTextures.DiffuseSRV.GetAddressOf());
	cpDeviceContext->PSSetShaderResources(PSSRVType::PS_REFLECT, 1, sTextures.ReflectSRV.GetAddressOf());
	cpDeviceContext->PSSetShaderResources(PSSRVType::PS_NORMAL, 1, sTextures.NormalSRV.GetAddressOf());
}

void ModelInterface::ScaleUp(const float& x, const float& y, const float& z)
{
	sTransformationProperties.xmvScale.m128_f32[0] += x;
	sTransformationProperties.xmvScale.m128_f32[1] += y;
	sTransformationProperties.xmvScale.m128_f32[2] += z;

	ID3D11Helper::UpdateBuffer(
		cpDeviceContext.Get(),
		TransformationBufferData::CreateTransfomredMatrix(TransformProperties::GetAffineTransformMatrix(sTransformationProperties)),
		D3D11_MAP_WRITE_DISCARD,
		cpTransformationDataBuffer.Get()
	);
}
