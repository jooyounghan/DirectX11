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

VertexOutput main(VertexInput input)
{
    VertexOutput result;
       
    float3 fModelNormal = mul(float4(input.f4WorldNormal, 0.f), mModelInvTranspose).xyz;
    fModelNormal = normalize(fModelNormal);
    
    result.f4ModelNormal = float4(fModelNormal, 0.f);
    
    result.f4ModelPos = mul(float4(input.f4WorldPos, 1.f), mModel);
    result.f4ProjPos = result.f4ModelPos;
    
    result.f4ProjPos = mul(result.f4ProjPos, mViewProj);
    result.f2TexCoord = input.f2TexCoord;
    
    return result;
}