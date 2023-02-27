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

#define MAX_LIGHT_NUM		100

enum LightType
{
	Directional,
	Point,
	Spot
};

struct LightConstantData
{
	int light_type = -1;							// 4
	Vector3 light_color = Vector3(0.0f);			// 12
	Vector3 position = Vector3(0.0f);				// 12
	Vector3 direction = Vector3(0.0f);				// 12
	float light_power = 0.f;						// 4
	float fall_off_start = 0.0f;					// 4
	float fall_off_end = 10.0f;						// 4
	float spot_power = 100.0f;						// 4
	float dummy[2];									// 8
};

struct LigthBufferData
{
	LightConstantData	light_constant_data[MAX_LIGHT_NUM];
	int					num_lights = 0;
	int					dummy[3]{ 0, 0, 0 };
};

class Light
{
public:
	Light();
	~Light();

public:
	static LightConstantData CreateDriectionalLightData(const Vector3 color, const Vector3 from, const Vector3 direction, const float& power);
	static LightConstantData CreatePointLightData(const Vector3 color, const Vector3 from, const float& power, const float& fall_off_start, const float& fall_off_end);
	static LightConstantData CreateSpotLightData(const Vector3 color, const Vector3 from, const float& power, const float& fall_off_start, const float& fall_off_end, const float& spot_power);

private:
	LigthBufferData			m_light_buffers_data_;

public:
	void AddLightConstantData(const LightConstantData& light_constant_data);
	void RemoveLightConstantData(const size_t& light_constant_data);

public:
	ComPtr<ID3D11Buffer>	m_light_cbuffer;

public:
	shared_ptr<Shader>		m_light_shader_;
};

