#include "Common.hlsli"

cbuffer ModelIDBuffer : register(b1)
{
    ModelID sModelId;
};


PixelOutput main(VertexOutput input) : SV_TARGET
{
    PixelOutput result;
    result.pixelColor = float4(0.0f, 1.0f, 0.0f, 1.0f);
    result.modleId = float4(sModelId.uiModelID.x, sModelId.uiModelID.y, sModelId.uiModelID.z, sModelId.uiModelIDStd) / sModelId.uiModelIDStd;
    return result;
}