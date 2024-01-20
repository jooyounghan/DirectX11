#include "Common.hlsli"

PixelOutput main(VertexOutput input) : SV_Target0
{    
    PixelOutput result;
    result.pixelColor = float4(input.f2TexCoord.x, input.f2TexCoord.y, 0.0f, 1.0f);
    return result;
}