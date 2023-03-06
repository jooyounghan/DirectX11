#include "BaseShaderCommon.hlsli"

cbuffer VertexConstantData : register(b0)
{
    matrix model;
    matrix inv_tranpose;
};

cbuffer MeshGroupConstantData : register(b1)
{
    matrix  view;
    matrix  projection;
    float3  eye_world_pos;
    float   dummy;
};

BasePixelShaderInput main(BaseVertexShaderInput input)
{
    BasePixelShaderInput output;
    
    float4 pos = float4(input.pos_model, 1.0f);
    output.pos_world = pos.xyz;

    pos = mul(pos, model);

    pos = mul(pos, view);
    pos = mul(pos, projection);

    output.pos_proj = pos;
    output.eye_world_pos = eye_world_pos;
    output.tex_coord = input.tex_coord;
    output.normal_model = mul(float4(input.normal_model, 0.f), inv_tranpose);

    return output;
}