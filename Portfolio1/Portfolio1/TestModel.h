#pragma once
#include "IModel.h"

class TestModel : public IModel
{
public:
	TestModel(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		const float& fCenterX,
		const float& fCenterY,
		const float& fCenterZ,
		const float& fLen
	);
	~TestModel() {};

public:
	virtual void Update() override;
	virtual void Render() override;
	virtual void RenderOutline() override;

protected:
	static std::atomic_bool		bStaticTestModelInitialized;

protected:
	static Microsoft::WRL::ComPtr<ID3D11InputLayout>	cpBaseInputLayout;
	static Microsoft::WRL::ComPtr<ID3D11VertexShader>	cpBaseVertexShader;

protected:
	static Microsoft::WRL::ComPtr<ID3D11PixelShader>	cpBasePixelShader;
	static Microsoft::WRL::ComPtr<ID3D11SamplerState>	cpBaseSampler;

protected:
	static Microsoft::WRL::ComPtr<ID3D11HullShader>		cpBaseHullShader;
	static Microsoft::WRL::ComPtr<ID3D11DomainShader>	cpBaseDomainShader;

protected:
	static Microsoft::WRL::ComPtr<ID3D11PixelShader>	cpOutlinerPixelShader;
};

