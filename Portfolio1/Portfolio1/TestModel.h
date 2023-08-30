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

protected:
	static std::atomic_bool		bBaseInitialized;

protected:
	static Microsoft::WRL::ComPtr<ID3D11InputLayout>	cpBaseInputLayout;
	static Microsoft::WRL::ComPtr<ID3D11VertexShader>	cpBaseVertexShader;

protected:
	static Microsoft::WRL::ComPtr<ID3D11PixelShader>	cpBasePixelShader;
	static Microsoft::WRL::ComPtr<ID3D11SamplerState>	cpBaseSampler;
};

