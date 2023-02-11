#include "Light.h"

Light::Light()
{
}

Light::~Light()
{
}

Light Light::CreateSpotLight(const Vector3 from, const Vector3 direction, const Vector3 strength)
{
    Light Light;
    LightConstantData& light_constant_data = Light.m_light_constant_data_;
    light_constant_data.position = from;
    light_constant_data.direction = direction;
    light_constant_data.strength = strength;
    return Light;
}

Light Light::CreatePointLight(const Vector3 from, const Vector3 strength, const float& fall_off_start, const float& fall_off_end)
{
    Light Light;
    LightConstantData& light_constant_data = Light.m_light_constant_data_;
    light_constant_data.position = from;
    light_constant_data.strength = strength;
    light_constant_data.fall_off_start = fall_off_start;
    light_constant_data.fall_off_end = fall_off_end;
    return Light;
}

Light Light::CreateDriectionalLight(const Vector3 from, const Vector3 strength, const float& fall_off_start, const float& fall_off_end, const float& spot_power)
{
    Light Light;
    LightConstantData& light_constant_data = Light.m_light_constant_data_;
    light_constant_data.position = from;
    light_constant_data.strength = strength;
    light_constant_data.fall_off_start = fall_off_start;
    light_constant_data.fall_off_end = fall_off_end;
    light_constant_data.spot_power = spot_power;
    return Light;
}