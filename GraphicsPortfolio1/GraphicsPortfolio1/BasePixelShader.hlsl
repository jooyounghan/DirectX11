#include "BaseShaderCommon.hlsli"

Texture2D g_texture0 : register(t0);
SamplerState g_sampler : register(s0);

cbuffer MeshPixelConstantData : register(b0)
{
    Material material;
};

cbuffer LigthBufferData : register(b1)
{
	LightConstantData	light_constant_data[MAX_LIGHT_NUM];
	int					num_lights;
	float3				dummy2;
};

float4 main(BasePixelShaderInput input) : SV_TARGET
{
    float4 ambient = g_texture0.Sample(g_sampler, input.tex_coord);
    
	float4 color = float4(0.f, 0.f, 0.f, 0.f);

	[loop]
	for (int light_idx = 0; light_idx < num_lights; ++light_idx)
	{			
		[branch]
		switch (light_constant_data[light_idx].light_type)
		{
		case DIRECT_LIGHT:
                color += GetDirectionalSpecular(ambient, material,
			input.eye_world_pos,
			input.pos_world, input.normal_model,
			light_constant_data[light_idx].direction,
			light_constant_data[light_idx].light_color,
			light_constant_data[light_idx].light_power);
			break;
		case POINT_LIGHT:
                color += GetPointSpecular(ambient, material, 
			input.eye_world_pos,
			input.pos_world, input.normal_model,
			light_constant_data[light_idx].position,
			light_constant_data[light_idx].light_color,
			light_constant_data[light_idx].light_power,
			light_constant_data[light_idx].fall_off_start,
			light_constant_data[light_idx].fall_off_end);
			break;
		case SPOT_LIGHT:
                color += GetSpotSpecular(ambient, material, 
			input.eye_world_pos,
			input.pos_world, input.normal_model,
			light_constant_data[light_idx].direction,
			light_constant_data[light_idx].position,
			light_constant_data[light_idx].light_color,
			light_constant_data[light_idx].light_power,
			light_constant_data[light_idx].fall_off_start,
			light_constant_data[light_idx].fall_off_end,
			light_constant_data[light_idx].spot_power);
			break;
		}
	}
	
    return color;
}
