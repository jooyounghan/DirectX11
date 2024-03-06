#include "ModelRenderHullShader.h"

#include "ID3D11Helper.h"
#include "DirectXDevice.h"
#include "IMovable.h"

ID3D11Buffer* const ModelRenderHullShader::pNullBuffer = nullptr;

ModelRenderHullShader::ModelRenderHullShader()
{
	ID3D11Helper::CreateHS(
		DirectXDevice::pDevice, 
		L"BaseHS.hlsl", 
		cpHullShader.GetAddressOf()
	);
}

ModelRenderHullShader::~ModelRenderHullShader()
{
}

void ModelRenderHullShader::ApplyShader()
{
	DirectXDevice::pDeviceContext->HSSetShader(cpHullShader.Get(), NULL, NULL);
}

void ModelRenderHullShader::DisapplyShader()
{
	DirectXDevice::pDeviceContext->HSSetShader(nullptr, NULL, NULL);
}

void ModelRenderHullShader::SetShader(IMovable& viewPosition)
{
	DirectXDevice::pDeviceContext->HSSetConstantBuffers(0, 1, viewPosition.GetPositionBuffer());
}

void ModelRenderHullShader::ResetShader()
{
	DirectXDevice::pDeviceContext->HSSetConstantBuffers(0, 1, &pNullBuffer);
}
