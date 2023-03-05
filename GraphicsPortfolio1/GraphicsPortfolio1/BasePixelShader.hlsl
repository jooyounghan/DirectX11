#include "BaseShaderCommon.hlsli"

Texture2D g_texture0 : register(t0);
SamplerState g_sampler : register(s0);

cbuffer LigthBufferData : register(b0)
{
	LightConstantData	light_constant_data[MAX_LIGHT_NUM];
	int					num_lights;
	float3				dummy;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	if (num_lights > 0)
	{
		[loop]
		for (int light_idx = 0; light_idx < num_lights; ++light_idx)
		{
            float3 to_eyes = normalize(input.eye_world_pos - input.pos_world);
            float3 to_light = normalize(light_constant_data[light_idx].position - input.pos_world);

			float3 halfway = to_eyes + to_light;
            halfway = normalize(halfway);
            float specular = dot(halfway, input.normal_model);
			
			
			[branch]
			switch (light_constant_data[light_idx].light_type)
			{
			case DIRECT_LIGHT:
				break;
			case POINT_LIGHT:
				break;
			case SPOT_LIGHT:
				break;
			}
            float4 new_color = float4(light_constant_data[light_idx].light_color, 1.f) * specular;
			return g_texture0.Sample(g_sampler, input.tex_coord) * new_color;
		}

	}
	else
	{
		return g_texture0.Sample(g_sampler, input.tex_coord);
	}
}
