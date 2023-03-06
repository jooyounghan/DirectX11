#pragma once
#include <directxtk/SimpleMath.h>
#include <windows.h>

#include "Shader.h"
#include "IRenderable.h"

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;

#define MAX_LIGHT_NUM		10

enum LightType
{
	Directional,
	Point,
	Spot
};

struct LightConstantData
{
	Vector4 light_color = Vector4(0.0f);			// 12
	Vector3 position = Vector3(0.0f);				// 12
	float	dummy = 0.f;							// 4
	Vector3 direction = Vector3(0.0f);				// 12
	float light_power = 0.f;						// 4
	float spot_power = 100.0f;						// 4
	float fall_off_start = 0.0f;					// 4
	float fall_off_end = 10.0f;						// 4
	int light_type = -1;							// 4
};

struct LigthBufferData
{
	LightConstantData	light_constant_data[MAX_LIGHT_NUM];
	int					num_lights = 0;
	Vector3				dummy;
};

static_assert((sizeof(LigthBufferData) % 16) == 0,
	"Constant Buffer size must be 16-byte aligned");

class Light : public IRenderable
{
public:
	Light(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& device_context);
	~Light();

public:
	static LightConstantData CreateDriectionalLightData(const Vector4 color, const Vector3 from, const Vector3 direction, const float& power);
	static LightConstantData CreatePointLightData(const Vector4 color, const Vector3 from, const float& power, const float& fall_off_start, const float& fall_off_end);
	static LightConstantData CreateSpotLightData(const Vector4 color, const Vector3 from, const float& power, const float& fall_off_start, const float& fall_off_end, const float& spot_power);

private:
	LigthBufferData			m_light_buffers_data_;

public:
	void	AddLightConstantData(const LightConstantData& light_constant_data, OUT LightConstantData*& light_ptr);
	void	DeleteLightConstantData(const size_t& light_index);

public:
	virtual void Render() override;
	virtual void Update() override;
};

