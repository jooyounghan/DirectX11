#include "Light.h"

Light::Light()
    : m_light_buffers_data_()
{
}

Light::~Light()
{
}

LightConstantData Light::CreateDriectionalLightData(const Vector3 color, const Vector3 from, const Vector3 direction, const float& power)
{
    LightConstantData light_constant_data;
    light_constant_data.light_type = LightType::Directional;
    light_constant_data.light_color = color;
    light_constant_data.position = from;
    light_constant_data.direction = direction;
    light_constant_data.light_power = power;
    return light_constant_data;
}

LightConstantData Light::CreatePointLightData(const Vector3 color, const Vector3 from, const float& power, const float& fall_off_start, const float& fall_off_end)
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

LightConstantData Light::CreateSpotLightData(const Vector3 color, const Vector3 from, const float& power, const float& fall_off_start, const float& fall_off_end, const float& spot_power)
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

void Light::AddLightConstantData(const LightConstantData& light_constant_data)
{
    m_light_buffers_data_.light_constant_data[m_light_buffers_data_.num_lights] = light_constant_data;
    m_light_buffers_data_.num_lights += 1;
}

void Light::RemoveLightConstantData(const size_t& light_constant_data)
{

}
