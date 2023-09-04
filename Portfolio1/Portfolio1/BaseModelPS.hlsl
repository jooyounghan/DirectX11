#include "Common.hlsli"

SamplerState Sampler : register(s0);

Texture2D AOTexture : register(t0);
Texture2D DiffuseTexture : register(t1);
Texture2D ReflectTexture : register(t2);

cbuffer LightSetsBuffer : register(b0)
{
    LightSet sLightSets[MAX_LIGHT_NUM];
};


float4 main(PixelInput input) : SV_TARGET
{
    float4 fResultColor = { 0.f, 0.f, 0.f, 0.f };
    float4 fDiffuseColor = DiffuseTexture.Sample(Sampler, float2(input.f2TexCoord.x, input.f2TexCoord.y));
    
    [unroll]
    for (int i = 0; i < MAX_LIGHT_NUM; ++i)
    {
        float4 tolight = normalize(sLightSets[i].f4Location - input.f4WorldPos);
        float fLightPower = 10 * clamp(dot(tolight, input.f4WorldNorVec), 0.f, 1.f);
        fResultColor += fDiffuseColor * fLightPower * sLightSets[i].f4Color;
    }
    
    return fResultColor;
}
