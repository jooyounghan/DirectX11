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

    float3 SpecularEnv = EnvSpecularCubeMap.Sample(Sampler, input.f4WorldPos.xyz).xyz;
    float3 IrradianceEnv = EnvIrradianceCubeMap.Sample(Sampler, input.f4WorldPos.xyz).xyz;

    if (bIsSpecularOn && bIsIrradianceOn)
    {
        result = (0.9 * SpecularEnv + 0.1 * IrradianceEnv).xyz;
    }
    else if (bIsSpecularOn)
    {
        result = SpecularEnv.xyz;
    }
    else if (bIsIrradianceOn)
    {
        result = IrradianceEnv.xyz;        
    }
    else;
    
    return float4(result, 0.f);
}