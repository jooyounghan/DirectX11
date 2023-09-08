#include "Common.hlsli"

SamplerState Sampler : register(s0);

Texture2D AOTexture : register(t0);
Texture2D DiffuseTexture : register(t1);
Texture2D ReflectTexture : register(t2);
Texture2D NormalTexture : register(t3);

cbuffer LightSetsBuffer : register(b0)
{
    LightSet sLightSets[MAX_LIGHT_NUM];
};


float4 main(PixelInput input) : SV_TARGET
{
    float4 fResultColor = { 0.f, 0.f, 0.f, 0.f };
    float4 fDiffuseColor = DiffuseTexture.Sample(Sampler, float2(input.f2TexCoord.x, input.f2TexCoord.y));
     
    float3 fNormalSampled = 2.f * NormalTexture.SampleLevel(Sampler, input.f2TexCoord, 0.f).xyz - 1.f;

    float3 f3ModelNormal = input.f4ModelNormal.xyz;
    float3 f3ModelTangent = input.f4ModelTangent.xyz;

    f3ModelNormal = normalize(f3ModelNormal);
    f3ModelTangent = normalize(f3ModelTangent);
    f3ModelTangent = normalize(f3ModelTangent - dot(f3ModelTangent, f3ModelNormal) * f3ModelNormal);
    float3 f3ModelBiTangent = normalize(cross(f3ModelNormal, f3ModelTangent));
    
    float3x3 TBN = float3x3(f3ModelTangent, f3ModelBiTangent, f3ModelNormal);
    fNormalSampled = normalize(mul(fNormalSampled, TBN));
    
    [unroll]
    for (int i = 0; i < MAX_LIGHT_NUM; ++i)
    {
        float4 tolight = normalize(sLightSets[i].f4Location - input.f4ModelPos);
        float fLightPower = 10 * clamp(dot(tolight, float4(fNormalSampled, 0.f)), 0.f, 1.f);
        fResultColor += fDiffuseColor * fLightPower * sLightSets[i].f4Color;
    }
    
    float4 temp = { 1.f, 0.f, 0.f, 0.f };
    return fResultColor;
}