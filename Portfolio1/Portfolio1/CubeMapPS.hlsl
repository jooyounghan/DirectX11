#include "CubeMapType.hlsli"

cbuffer TextureFlagBuffer : register(b4)
{
    bool    bIsSpecularOn;
    bool    bIsIrradianceOn;
    bool    bIsBRDFOn;
    uint3   uiDummy[1];
};

SamplerState Sampler : register(s0);

TextureCube EnvSpecularCubeMap : register(t6);
TextureCube EnvIrradianceCubeMap : register(t7);

float4 main(CubeMapPixelInput input) : SV_Target
{
    float3 result = float3(0.f, 0.f, 0.f);
    int divider = 0;
    if (bIsSpecularOn)
    {
        result += EnvSpecularCubeMap.SampleLevel(Sampler, input.f4WorldPos.xyz, 0.f).xyz;
        divider += 1;
    }
    if (bIsIrradianceOn)
    {
        result += EnvIrradianceCubeMap.SampleLevel(Sampler, input.f4WorldPos.xyz, 0.f).xyz;
        divider += 1;
    }
    
    if (divider > 0)
    {
        result = result / divider;

    }
    
    return float4(result, 0.f);
}