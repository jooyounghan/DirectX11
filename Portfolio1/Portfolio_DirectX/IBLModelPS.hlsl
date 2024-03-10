#include "Common.hlsli"

cbuffer ModelIDBuffer : register(b0)
{
    uint uIMeshId;
    uint uiDummy[3];
};

cbuffer DiffuseRateBuffer : register(b1)
{
    float DiffuseRate;
    float3 fDummy;
};

TextureCube SpecularIBLTexture : register(t0);
TextureCube DiffuseIBLTexture : register(t1);

SamplerState ClampSampler : register(s0);

PBRModelPixelOutput main(PBRModelVertexOutput input)
{
    PBRModelPixelOutput result;
    result.pixelColor = 
    (1.f - DiffuseRate) * SpecularIBLTexture.Sample(ClampSampler, input.f4ModelPos.xyz)
    + DiffuseRate * DiffuseIBLTexture.Sample(ClampSampler, input.f4ModelPos.xyz);
    result.modelID = uIMeshId;
    return result;
}