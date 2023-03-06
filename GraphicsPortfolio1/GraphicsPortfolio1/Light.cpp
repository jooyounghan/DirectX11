#include "Light.h"
#include "D3D11Utilizer.h"
#include "LightShader.h"

using namespace DirectX;
using namespace std;

Light::Light(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& device_context)
    : IRenderable(device, device_context), m_light_buffers_data_()
{
    D3D11Utilizer::CreateConstantBuffer(m_device_, m_light_buffers_data_, m_pixel_cbuffer_);
}

Light::~Light()
{
}

LightConstantData Light::CreateDriectionalLightData(const Vector4 color, const Vector3 from, const Vector3 direction, const float& power)
{
    LightConstantData light_constant_data;
    light_constant_data.light_type = LightType::Directional;
    light_constant_data.light_color = color;
    light_constant_data.position = from;
    light_constant_data.direction = direction;
    light_constant_data.light_power = power;
    return light_constant_data;
}

LightConstantData Light::CreatePointLightData(const Vector4 color, const Vector3 from, const float& power, const float& fall_off_start, const float& fall_off_end)
{
    LightConstantData light_constant_data;
    light_constant_data.light_type = LightType::Point;
    light_constant_data.light_color = color;
    light_constant_data.position = from;
    light_constant_data.light_power = power;
    light_constant_data.fall_off_start = fall_off_start;
    light_constant_data.fall_off_end = fall_off_end;

    return light_constant_data;
}

LightConstantData Light::CreateSpotLightData(const Vector4 color, const Vector3 from, const float& power, const float& fall_off_start, const float& fall_off_end, const float& spot_power)
{
    LightConstantData light_constant_data;
    light_constant_data.light_type = LightType::Spot;
    light_constant_data.light_color = color;
    light_constant_data.position = from;
    light_constant_data.light_power = power;
    light_constant_data.fall_off_start = fall_off_start;
    light_constant_data.fall_off_end = fall_off_end;
    light_constant_data.spot_power = spot_power;
    return light_constant_data;
}

void Light::AddLightConstantData(const LightConstantData& light_constant_data, OUT LightConstantData*& light_ptr)
{
    light_ptr = &m_light_buffers_data_.light_constant_data[0];
    m_light_buffers_data_.light_constant_data[m_light_buffers_data_.num_lights] = light_constant_data;
    m_light_buffers_data_.num_lights++;
}

void Light::DeleteLightConstantData(const size_t& light_index)
{
    if (m_light_buffers_data_.num_lights > 0)
    {
        if (light_index == (MAX_LIGHT_NUM - 1))
        {

        }
        else if (light_index >= 0)
        {
            memcpy(&m_light_buffers_data_.light_constant_data[light_index],
                &m_light_buffers_data_.light_constant_data[light_index + 1],
                sizeof(LightConstantData) * (MAX_LIGHT_NUM - light_index - 1));
        }
        else;

        m_light_buffers_data_.num_lights--;
    }
}

void Light::Render()
{
}

void Light::Update()
{
    D3D11Utilizer::UpdateBuffer(m_device_, m_device_context_, m_light_buffers_data_, m_pixel_cbuffer_);
}