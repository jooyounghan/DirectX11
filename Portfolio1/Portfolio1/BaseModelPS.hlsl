#include "Common.hlsli"

SamplerState Sampler : register(s0);

Texture2D AOTexture : register(t0);
Texture2D DiffuseTexture : register(t1);
Texture2D ReflectTexture : register(t2);

cbuffer LightSet : register(b0)
{
    unsigned int LightType;
    float4 fLocation;
    float4 fColor;
    float4 fDirection;
    float3 fDummy;
};


float4 main(PixelInput input) : SV_TARGET
{
    return DiffuseTexture.Sample(Sampler, float2(input.fTexCoord.x, input.fTexCoord.y)) * fColor;
}
