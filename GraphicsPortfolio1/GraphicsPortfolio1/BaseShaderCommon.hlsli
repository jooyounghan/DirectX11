#ifndef __COMMON_HLSLI__
#define __COMMON_HLSLI__


#define MAX_LIGHT_NUM 10

struct VertexShaderInput
{
	float3 pos_model	: POSITION;
	float3 normal_model	: NORMAL;
	float2 tex_coord	: TEXCOORD;
};

struct PixelShaderInput
{
	float4 pos_proj			: SV_POSITION;
	float3 pos_world		: POSITION0;
	float3 eye_world_pos	: POSITION1;
	float3 normal_model		: NORMAL;
	float2 tex_coord		: TEXCOORD;
};

struct LightConstantData
{
	float3 light_color;
	float dummy1;
	float3 position;
	float dummy2;
	float3 direction;
	float light_power;
	float spot_power;
	float fall_off_start;
	float fall_off_end;
	int light_type;
};

#define DIRECT_LIGHT	0
#define POINT_LIGHT		1
#define SPOT_LIGHT		2

#endif // __COMMON_HLSLI__