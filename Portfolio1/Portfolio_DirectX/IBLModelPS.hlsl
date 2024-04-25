#include "Common.hlsli"

cbuffer ModelIDBuffer : register(b0)
{
    uint uIMeshId;
    uint uiDummy[3];
};

Texture2D IBLImageTexture : register(t0);

SamplerState ClampSampler : register(s0);

PBRModelPixelOutput main(PBRModelVertexOutput input)
{
    PBRModelPixelOutput result;
    result.pixelColor = IBLImageTexture.Sample(ClampSampler, input.f2TexCoord.xy);
    result.modelID = uIMeshId;
    return result;
}