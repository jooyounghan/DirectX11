#include "CubeMapType.hlsli"

cbuffer TextureFlagBuffer : register(b2)
{
    bool    bTextureCubeOn;
    uint3   uiDummy[3];
};

SamplerState Sampler : register(s0);

TextureCube DDSCubeMap : register(t6);

float4 main(CubeMapPixelInput input) : SV_Target
{
    float4 result = (1.f, 0.f, 0.f, 1.f);

    if (bTextureCubeOn)
    {
        result = DDSCubeMap.SampleLevel(Sampler, input.f4WorldPos.xyz, 0.f);
    }
    return result;
}