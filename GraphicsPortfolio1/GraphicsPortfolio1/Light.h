#pragma once
#include <d3d11.h>
#include <directxtk/SimpleMath.h>
#include <windows.h>
#include <wrl.h>
#include <memory>

#include "Shader.h"

using Microsoft::WRL::ComPtr;
using DirectX::SimpleMath::Matrix;

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;

using std::shared_ptr;

struct LightConstantData
{
	Vector3 strength = Vector3(0.0f);				// 12
	Vector3 position = Vector3(0.0f);				// 12
	Vector3 direction = Vector3(0.0f);				// 12
	float fall_off_start = 0.0f;					// 4
	float fall_off_end = 10.0f;						// 4
	float spot_power = 100.0f;						// 4
};

class Light
{
public:
	Light();
	~Light();

public:
	static Light CreateSpotLight(const Vector3 from, const Vector3 direction, const Vector3 strength);
	static Light CreatePointLight(const Vector3 from, const Vector3 strength, const float& fall_off_start, const float& fall_off_end);
	static Light CreateDriectionalLight(const Vector3 from, const Vector3 strength, const float& fall_off_start, const float& fall_off_end, const float& spot_power);

private:
	LightConstantData		m_light_constant_data_;

public:
	ComPtr<ID3D11Buffer> m_light_cbuffer;

public:
	shared_ptr<Shader>		m_light_shader_;
};

