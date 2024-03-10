#include "Common.hlsli"

cbuffer ModelMatrix : register(b0)
{
    matrix mModel;
    matrix mModelInvTranspose;
};

cbuffer CameraViewProj : register(b1)
{
    matrix mViewProj;
    matrix mViewProjInv;
};

BasicVertexOutput main(BasicVertexInput input)
{
    BasicVertexOutput result;
    result.f3ModelNormal = normalize(mul(float4(input.f3WorldNormal, 0.f), mModelInvTranspose)).xyz;
    result.f4ProjPos = mul(mul(float4(input.f3WorldPos, 1.f), mModel), mViewProj);
    result.f2TexCoord = input.f2TexCoord;    
    return result;
}