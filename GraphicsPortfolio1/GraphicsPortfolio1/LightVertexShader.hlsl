#include "BaseShaderCommon.hlsli"

PixelShaderInput main(VertexShaderInput input)
{
    PixelShaderInput output;
    
    float4 pos = float4(input.pos_model, 1.0f);
    output.pos_world = pos.xyz;

    pos = mul(pos, model);

    pos = mul(pos, view);
    pos = mul(pos, projection);

    output.pos_proj = pos;
    output.eye_world_pos = eye_world_pos;
    output.tex_coord = input.tex_coord;
    output.normal_model = input.normal_model;

    return output;
}