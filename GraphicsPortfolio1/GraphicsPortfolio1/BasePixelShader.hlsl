Texture2D g_texture0 : register(t0);
SamplerState g_sampler : register(s0);

struct PixelShaderInput
{
    float4 posProj : SV_POSITION;
    float3 posWorld : POSITION;
    float2 texcoord : TEXCOORD;
};

struct LightConstantData
{
	int light_type;
	float3 light_color;
	float3 position;
	float3 direction;
	float light_power;
	float fall_off_start;
	float fall_off_end;
	float spot_power;
	float dummy[2];
};

cbuffer LightConstants : register(b0)
{
	LightConstantData	light_constant_data[100];
	int					num_lights;
	int					dummy;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	int test1 = num_lights - 1;
	float4 test2 = (light_constant_data[test1].light_color, 1.f);
	
	// 빛 색이 잘 들어가는지 확인 및 HLSL 디버깅 방법 확인
	 
	if (num_lights > 0)
	{
		return test2;
	}
	else
	{
		return g_texture0.Sample(g_sampler, input.texcoord);

	}

}