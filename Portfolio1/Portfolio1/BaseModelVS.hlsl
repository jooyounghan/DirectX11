#include "Common.hlsli"

SamplerState Sampler : register(s0);

cbuffer ModelMatrix : register(b0)
{
    matrix mModel;
    matrix mModelInvTranspose;
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
        
    float3 fNormalSampled = 2.f * NormalTexture.SampleLevel(Sampler, input.f2TexCoord, 0.f).xyz - 1.f;
    float fHeightSampled = 2.f * HeightTexture.SampleLevel(Sampler, input.f2TexCoord, 0.f).x - 1.f;
    
    float3 fModelNormal = mul(input.f4WorldNormal, mModelInvTranspose).xyz;
    fModelNormal = normalize(fModelNormal);
    
    float3 fTangent = mul(input.f4WorldTangent, mModel).xyz;
    fTangent = normalize(fTangent);
    fTangent = normalize(fTangent - dot(fTangent, fModelNormal) * fModelNormal);
    
    float3 fBiTangent = cross(fModelNormal, fTangent);
    
    float3x3 TBN = float3x3(fTangent, fBiTangent, fModelNormal);
    
    fNormalSampled = normalize(mul(fNormalSampled, TBN));
    
    result.f4WorldNorVec = float4(fNormalSampled, 0.f);
    result.f4WorldPos = input.f4WorldPos/* + (fHeightSampled * 0.2f) DrawIndexed로 들어온 점들의 인터폴레이션으로 정해지므로 도메인 셰이더에서!*/;
    result.f4ProjPos = result.f4WorldPos;
    result.f4ProjPos = mul(result.f4ProjPos, mModel);
    result.f4ProjPos = mul(result.f4ProjPos, mViewProj);
    result.f2TexCoord = input.f2TexCoord;
    return result;
}