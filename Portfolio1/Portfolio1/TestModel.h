#pragma once
#include "IModel.h"

namespace std {
	template<typename T>
	struct atomic;

	using atomic_bool = atomic<bool>;
}

class TestModel : public IModel
{
public:
	TestModel(
		ComPtr<ID3D11Device>& cpDeviceIn,
		ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		const float& fCenterX,
		const float& fCenterY,
		const float& fCenterZ,
		const float& fLen
	);
	~TestModel() {};

public:
	virtual void Update() override;
	virtual void Render() override;

public:
	ComPtr<ID3D11Texture2D>				cpTexture;
	ComPtr<ID3D11ShaderResourceView>	cpShaderResourceViewTexture;

protected:
	static std::atomic_bool		bBaseInitialized;

protected:
	static ComPtr<ID3D11InputLayout>	cpBaseInputLayout;
	static ComPtr<ID3D11VertexShader>	cpBaseVertexShader;

protected:
	static ComPtr<ID3D11PixelShader>	cpBasePixelShader;
	static ComPtr<ID3D11SamplerState>	cpBaseSampler;
};

