#ifndef __COMMON_HLSLI__
#define __COMMON_HLSLI__


#define DIRECT_LIGHT	0
#define POINT_LIGHT		1
#define SPOT_LIGHT		2

#define MAX_LIGHT_NUM 10

struct Material
{
    float diffuse;
    float specular;
    float shininess;
    float dummy1;
};

struct BaseVertexShaderInput
{
	float3 pos_model	: POSITION;
	float3 normal_model	: NORMAL;
	float2 tex_coord	: TEXCOORD;
};

struct BasePixelShaderInput
{
	float4 pos_proj			: SV_POSITION;
	float3 pos_world		: POSITION0;
	float3 eye_world_pos	: POSITION1;
	float3 normal_model		: NORMAL;
	float2 tex_coord		: TEXCOORD;
};

struct LightConstantData
{
	float4 light_color;
	float3 position;
	float dummy2;
	float3 direction;
	float light_power;
	float spot_power;
	float fall_off_start;
	float fall_off_end;
	int light_type;
};

float GetAtten(float start, float end, float distance, float fall_off_start, float fall_off_end)
{
	return lerp(start, end, (clamp(distance, fall_off_start, fall_off_end) - fall_off_start) / (fall_off_end - fall_off_start));
}

float4 BlinnPhong(float4 ambient, Material material, float3 to_eyes, float3 to_light, float3 obj_normal, float4 light_color, float light_power)
{
    float3 halfway = normalize(to_eyes + to_light);
    float hdotn = max(dot(halfway, obj_normal), 0.f);
    float4 calc_specular = material.specular * pow(hdotn, material.shininess) * light_color;
    float4 calc_diffuse = material.diffuse * light_color;
    return (ambient * hdotn + (calc_diffuse + calc_specular)) * light_power;
}

float4 GetDirectionalSpecular(float4 ambient, Material material, float3 eye_pos, float3 obj_pos, float3 obj_normal, float3 light_direction, float4 light_color, float light_power)
{
    float3 to_eyes = normalize(eye_pos - obj_pos);
    float3 to_light = -light_direction;
    float ndotl = max(dot(to_light, obj_normal), 0.0f);
    to_light = normalize(to_light);
	
    return BlinnPhong(ambient, material, to_eyes, to_light, obj_normal, light_color, light_power * ndotl);
}

float4 GetPointSpecular(float4 ambient, Material material, float3 eye_pos, float3 obj_pos, float3 obj_normal, float3 light_pos, float4 light_color, float light_power, float fall_off_start, float fall_off_end)
{
    float3 to_eyes = normalize(eye_pos - obj_pos);
    float3 to_light = light_pos - obj_pos;
    float ndotl = max(dot(to_light, obj_normal), 0.0f);

    float distance = length(to_light);
    to_light = normalize(to_light);

	float atten = GetAtten(1.f, 0.f, distance, fall_off_start, fall_off_end);

    return BlinnPhong(ambient, material, to_eyes, to_light, obj_normal, light_color, light_power * ndotl * atten);
}

float4 GetSpotSpecular(float4 ambient, Material material, float3 eye_pos, float3 obj_pos, float3 obj_normal, float3 light_direction, float3 light_pos, float4 light_color, float light_power, float fall_off_start, float fall_off_end, float spot_power)
{
    float3 to_eyes = normalize(eye_pos - obj_pos);
    float3 to_light = light_pos - obj_pos;
    float ndotl = max(dot(to_light, obj_normal), 0.0f);
    
    float distance = length(to_light);
    to_light = normalize(to_light);
    
	float atten = GetAtten(1.f, 0.f, distance, fall_off_start, fall_off_end);
    atten *= pow(dot(normalize(light_direction), -to_light), spot_power);

    return BlinnPhong(ambient, material, to_eyes, to_light, obj_normal, light_color, light_power * atten);
}
#endif // __COMMON_HLSLI__