#include "Common.hlsli"

SamplerState Sampler : register(s0);

cbuffer ModelMatrix : register(b0)
{
    matrix mModel;
    matrix mModelInv;
};

cbuffer ViewProjMatrix : register(b1)
{
    matrix mViewProj;
    matrix mViewProjInv;
};

Texture2D HeightTexture : register(t0);
Texture2D NormalTexture : register(t1);

PixelInput main(VertexInput input)
{
    PixelInput result;
        
    float3 fNormalSampled = 2.f * NormalTexture.SampleLevel(Sampler, input.fTexCoord, 0.f).xyz - 1.f;
    float fHeightSampled = 2.f * HeightTexture.SampleLevel(Sampler, input.fTexCoord, 0.f).x - 1.f;
    
    float3 fModelNormal = mul(input.fWorldNormal, mModelInv).xyz;
    fModelNormal = normalize(fModelNormal);
    
    float3 fTangent = mul(input.fWorldTangent, mModel).xyz;
    fTangent = normalize(fTangent);
    fTangent = normalize(fTangent - dot(fTangent, fModelNormal) * fModelNormal);
    
    float3 fBiTangent = cross(fModelNormal, fTangent);
    
    float3x3 TBN = float3x3(fTangent, fBiTangent, fModelNormal);
    
    fNormalSampled = normalize(mul(fNormalSampled, TBN));
    
    result.fWorldNorVec = float4(fNormalSampled, 0.f);
    result.fWorldPos = input.fWorldPos;
    result.fProjPos = result.fWorldPos;
    result.fProjPos = mul(result.fProjPos, mModel);
    result.fProjPos = mul(result.fProjPos, mViewProj);
    result.fTexCoord = input.fTexCoord;
    return result;
}