#include "Common.hlsli"

cbuffer ModelID : register(b0)
{
    uint uiModelId;
    uint uiDummy[3];
};

TextureCube SpecularIBLTexture : register(t0);
TextureCube DiffuseIBLTexture : register(t1);

SamplerState ClampSampler : register(s0);

PixelOutput main(VertexOutput input)
{
    PixelOutput result;
    result.pixelColor = 
    0.9 * SpecularIBLTexture.Sample(ClampSampler, input.f4ModelPos.xyz) 
    + 0.1 * DiffuseIBLTexture.Sample(ClampSampler, input.f4ModelPos.xyz);
    result.modelID = uiModelId;
    return result;
}