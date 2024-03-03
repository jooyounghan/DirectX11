#include "Common.hlsli"

cbuffer ModelID : register(b0)
{
    uint uIMeshId;
    uint uiDummy[3];
};

PixelOutput main(VertexOutput input)
{
    PixelOutput result;
    result.pixelColor = float4(input.f2TexCoord.x, input.f2TexCoord.y, 0.0f, 1.0f);
    result.modelID = uIMeshId;
    return result;
}