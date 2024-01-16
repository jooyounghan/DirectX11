#include "Common.hlsli"

float4 main(VertexOutput input) : SV_Target0
{
    return float4(input.f2TexCoord.x, input.f2TexCoord.y, 0.0f, 1.0f);
}