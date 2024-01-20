#include "Common.hlsli"

cbuffer ModelID : register(b0)
{
    uint uiModelId;
    uint uiDummy[3];
};

PixelOutput main(VertexOutput input) : SV_Target0
{    
    PixelOutput result;
    result.pixelColor = float4(input.f2TexCoord.x, input.f2TexCoord.y, 0.0f, 1.0f);
    result.modelID = uiModelId;
    return result;
}