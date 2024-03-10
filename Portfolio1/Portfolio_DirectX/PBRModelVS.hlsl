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

PBRModelVertexOutput main(PBRModelVertexInput input)
{
    PBRModelVertexOutput result;
       
    result.f3ModelNormal = normalize(mul(float4(input.f3WorldNormal, 0.f), mModelInvTranspose)).xyz;
    result.f3ModelTangent = normalize(mul(float4(input.f3WorldTangent, 0.f), mModel)).xyz;
    
    result.f4ModelPos = mul(float4(input.f3WorldPos, 1.f), mModel);
    result.f4ProjPos = result.f4ModelPos;
    
    result.f4ProjPos = mul(result.f4ProjPos, mViewProj);
    result.f2TexCoord = input.f2TexCoord;
    
    return result;
}