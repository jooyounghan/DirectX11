#include "Common.hlsli"
#include "NormalVectorShaderType.hlsli"

NormalVectorPSOutput main(NormalVectorGSOutput input) : SV_TARGET
{
    NormalVectorPSOutput result;
    result.pixelColor = float4(1.f, 1.f - input.f2TexCoord.x, 0.f, 1.f);
    return result;
}