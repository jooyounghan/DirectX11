#include "Common.hlsli"

Texture2D MirrorResult : register(t0);

cbuffer ModelID : register(b0)
{
    uint uIMeshId;
    uint uiDummy[3];
};

cbuffer MirrorProperties : register(b1)
{
    float Alpha;
    float fDummy[3];
};

SamplerState WrapSampler : register(s0);

PBRModelPixelOutput main(BasicVertexOutput input)
{
    PBRModelPixelOutput result;
    result.pixelColor = Alpha * float4(1.f, 1.f, 1.f, 0.f) + (1 - Alpha) *
        MirrorResult.SampleLevel(WrapSampler, input.f2TexCoord, 0.f);
    result.modelID = uIMeshId;
    return result;
}