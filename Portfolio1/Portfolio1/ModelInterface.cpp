#include "ModelInterface.h"
#include "EnumVar.h"
#include "FileLoader.h"
#include "ID3D11Helper.h"
#include "DepthStencilState.h"

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
	//cpDeviceContext->IASetInputLayout(cpBaseInputLayout.Get());
	//cpDeviceContext->IASetIndexBuffer(cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	//UINT stride = sizeof(Vertex);
	//UINT offset = 0;
	//cpDeviceContext->IASetVertexBuffers(0, 1, cpVertexBuffer.GetAddressOf(), &stride, &offset);
	//cpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	//cpDeviceContext->VSSetShader(cpBaseVertexShader.Get(), 0, 0);
	cpDeviceContext->VSSetConstantBuffers(VSConstBufferType::VS_ModelMatrix, 1, cpModelMatrixBuffer.GetAddressOf());

	//cpDeviceContext->HSSetShader(cpBaseHullShader.Get(), 0, 0);
	//cpDeviceContext->DSSetShader(cpBaseDomainShader.Get(), 0, 0);

	//cpDeviceContext->DSSetSamplers(0, 1, cpBaseSampler.GetAddressOf());
	//cpDeviceContext->DSSetShaderResources(DSSRVType::DS_HEIGHT, 1, sTextures.HeightSRV.GetAddressOf());

	//cpDeviceContext->PSSetShader(cpBasePixelShader.Get(), 0, 0);
	//cpDeviceContext->PSSetSamplers(0, 1, cpBaseSampler.GetAddressOf());

	modelID.SetPsConstantBuffers(cpDeviceContext.Get());

	cpDeviceContext->PSSetShaderResources(PSSRVType::PS_AO, 1, sTextures.AOSRV.GetAddressOf());
	cpDeviceContext->PSSetShaderResources(PSSRVType::PS_DIFFUSE, 1, sTextures.DiffuseSRV.GetAddressOf());
	cpDeviceContext->PSSetShaderResources(PSSRVType::PS_REFLECT, 1, sTextures.ReflectSRV.GetAddressOf());
	cpDeviceContext->PSSetShaderResources(PSSRVType::PS_NORMAL, 1, sTextures.NormalSRV.GetAddressOf());

	cpDeviceContext->OMSetDepthStencilState(DepthStencilState::pGetDSS(DepthStencilState::MaskOption), 1);

	cpDeviceContext->DrawIndexed(ui32IndexCount, 0, 0);

	cpDeviceContext->OMSetDepthStencilState(DepthStencilState::pGetDSS(DepthStencilState::DefaultOption), 0);
}

void ModelInterface::RenderOutline()
{
	sModelTransformation.xmvScale.m128_f32[0] += 0.05;
	sModelTransformation.xmvScale.m128_f32[1] += 0.05;
	sModelTransformation.xmvScale.m128_f32[2] += 0.05;
	sModelTransformation.xmvScale.m128_f32[3] += 0.05;

	ID3D11Helper::UpdateBuffer(
		cpDeviceContext.Get(),
		TransformedMatrix::CreateTransfomredMatrix(ModelTransform::GetAffineTransformMatrix(sModelTransformation)),
		D3D11_MAP_WRITE_DISCARD,
		cpModelMatrixBuffer.Get()
	);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	cpDeviceContext->IASetInputLayout(cpBaseInputLayout.Get());
	cpDeviceContext->IASetIndexBuffer(cpIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	cpDeviceContext->IASetVertexBuffers(0, 1, cpVertexBuffer.GetAddressOf(), &stride, &offset);
	cpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cpDeviceContext->VSSetShader(cpBaseVertexShader.Get(), 0, 0);
	cpDeviceContext->VSSetConstantBuffers(VSConstBufferType::VS_ModelMatrix, 1, cpModelMatrixBuffer.GetAddressOf());

	cpDeviceContext->PSSetShader(cpOutlinerPixelShader.Get(), 0, 0);
	
	modelID.SetPsConstantBuffers(cpDeviceContext.Get());

	cpDeviceContext->OMSetDepthStencilState(DepthStencilState::pGetDSS(DepthStencilState::DrawNotEqualOption), 1);

	cpDeviceContext->DrawIndexed(ui32IndexCount, 0, 0);

	cpDeviceContext->OMSetDepthStencilState(DepthStencilState::pGetDSS(DepthStencilState::DefaultOption), 0);

	sModelTransformation.xmvScale.m128_f32[0] -= 0.05;
	sModelTransformation.xmvScale.m128_f32[1] -= 0.05;
	sModelTransformation.xmvScale.m128_f32[2] -= 0.05;
	sModelTransformation.xmvScale.m128_f32[3] -= 0.05;

	ID3D11Helper::UpdateBuffer(
		cpDeviceContext.Get(),
		TransformedMatrix::CreateTransfomredMatrix(ModelTransform::GetAffineTransformMatrix(sModelTransformation)),
		D3D11_MAP_WRITE_DISCARD,
		cpModelMatrixBuffer.Get()
	);
}
