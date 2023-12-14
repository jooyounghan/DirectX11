#include "Common.hlsli"

SamplerState WrapSampler : register(s0);
SamplerState ClampSampler : register(s1);


Texture2D MirrorSelf : register(t11);

cbuffer ModelIDBuffer : register(b2)
{
    ModelID sModelId;
};

cbuffer CameraInfo : register(b7)
{
    float4 f4CameraPos;
    matrix mViewProj;
    matrix mViewProjInv;
};


PixelOutput main(VertexOutput input)
{
    PixelOutput result;
    float2 mirrorTexCoord = float2(1.f - input.f2TexCoord.x, input.f2TexCoord.y);
    result.pixelColor = float4(0.1f, 0.1f, 0.1f, 1.f) + MirrorSelf.Sample(ClampSampler, mirrorTexCoord);
    result.modleId = float4(sModelId.uiModelID.x, sModelId.uiModelID.y, sModelId.uiModelID.z, sModelId.uiModelIDStd) / sModelId.uiModelIDStd;
    return result;
}