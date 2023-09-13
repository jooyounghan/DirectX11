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

cbuffer ModelIDBuffer : register(b1)
{
    ModelID sModelId;
};

PixelOutput main(PixelInput input) : SV_TARGET
{
    PixelOutput result;
    
    float4 fResultColor = { 0.f, 0.f, 0.f, 0.f };
    float4 fDiffuseColor = DiffuseTexture.Sample(Sampler, float2(input.f2TexCoord.x, input.f2TexCoord.y));
    float3 fNormalSampled = NormalTexture.Sample(Sampler, float2(input.f2TexCoord.x, input.f2TexCoord.y)).xyz;
    fNormalSampled = 2.f * fNormalSampled - 1.f;
    float3 f3ModelNormal = input.f4ModelNormal.xyz;
    float3 f3ModelTangent = input.f4ModelTangent.xyz;
    float3 f3ModelBiTangent = input.f4ModelBiTangent.xyz;

    
    float3x3 TBN = float3x3(f3ModelTangent, f3ModelBiTangent, f3ModelNormal);
    fNormalSampled = normalize(mul(fNormalSampled, TBN));


    
    [unroll]
    for (int i = 0; i < MAX_LIGHT_NUM; ++i)
    {
        float4 tolight = normalize(sLightSets[i].f4Location - input.f4ModelPos);
        float fLightPower = 10 * clamp(dot(tolight, float4(fNormalSampled, 0.f)), 0.f, 1.f);
        fResultColor += fDiffuseColor * fLightPower * sLightSets[i].f4Color;
    }
    
    result.pixelColor = fResultColor;
    result.modleId = float4(0.f, 0.f, 0.f, sModelId.uiModelID);
    return result;
}