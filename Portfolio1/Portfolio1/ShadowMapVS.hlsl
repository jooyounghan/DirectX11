#include "Common.hlsli"

cbuffer ModelMatrix : register(b0)
{
    matrix mModel;
    matrix mModelInvTranspose;
};

cbuffer LightViewProj : register(b3)
{
    matrix mLightViewProj;
    matrix mLightViewProjInvTranspose;
};


float4 main(VertexInput input) : SV_Position
{
    float4 f4ModelPos = mul(input.f4WorldPos, mModel);
    return mul(f4ModelPos, mLightViewProj);
}