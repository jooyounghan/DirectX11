#ifndef __COMMON_HLSLI__
#define __COMMON_HLSLI__


#define DIRECT_LIGHT	0
#define POINT_LIGHT		1
#define SPOT_LIGHT		2

#define MAX_LIGHT_NUM 10

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

float4 GetDirectionalSpecular(float3 eye_pos, float3 obj_pos, float3 obj_normal, float3 light_direction, float4 light_color, float light_power)
{
    float3 to_eyes = normalize(eye_pos - obj_pos);
    float3 to_light = -normalize(light_direction);

    float3 halfway = normalize(to_eyes + to_light) * light_power;
    return light_color * dot(halfway, obj_normal);
}

float4 GetPointSpecular(float3 eye_pos, float3 obj_pos, float3 obj_normal, float3 light_pos, float4 light_color, float light_power, float fall_off_start, float fall_off_end)
{
    float3 to_eyes = normalize(eye_pos - obj_pos);
    float3 to_light = normalize(light_pos - obj_pos);

	float distance = length(light_pos - obj_pos);
	
	float atten = GetAtten(1.f, 0.f, distance, fall_off_start, fall_off_end);
    float3 halfway = normalize(to_eyes + to_light) * light_power * atten;
	
    return light_color * max(dot(halfway, obj_normal), 0.f);
}

float4 GetSpotSpecular(float3 eye_pos, float3 obj_pos, float3 obj_normal, float3 light_direction, float3 light_pos, float4 light_color, float light_power, float fall_off_start, float fall_off_end, float spot_power)
{
    float3 to_eyes = normalize(eye_pos - obj_pos);

    float3 to_light = normalize(light_pos - obj_pos);
    float distance = length(light_pos - obj_pos);
	
	float atten = GetAtten(1.f, 0.f, distance, fall_off_start, fall_off_end);
    atten *= pow(dot(normalize(light_direction), -to_light), spot_power);

    float3 halfway = normalize(to_eyes + to_light) * light_power * atten;	
    return light_color * dot(halfway, obj_normal);
}
#endif // __COMMON_HLSLI__