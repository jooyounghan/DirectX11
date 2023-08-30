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
    return DiffuseTexture.Sample(Sampler, float2(input.fTexCoord.x, input.fTexCoord.y)) * sLightSets[0].fColor;
}
