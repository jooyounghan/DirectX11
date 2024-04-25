#include "CommonFilterArgs.hlsli"

Texture2D<float> InputTexture2D[6] : register(t0);
RWTexture2DArray<float> OutputTexture[6] : register(u0);

SamplerState WrapSampler : register(s0);

[numthreads(16, 16, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint uiWidth;
    uint uiHeight;

    [unroll]
    for (uint dir = 0; dir < 6; ++dir)
    {
        float sampleDepth = 0.f;
        InputTexture2D[dir].GetDimensions(uiWidth, uiHeight);
        for (uint idx = 0; idx < Gaussian3x3Count; ++idx)
        {
            float2 loadPos = GetClampedTextureCoord(
            uiWidth, uiHeight,
            DTid.x + Offsets5x5[idx].x,
            DTid.y + Offsets5x5[idx].y);
        
            sampleDepth += InputTexture2D[dir].SampleLevel(WrapSampler, loadPos, 0.f) * Gaussian3x3Kernel[idx];
        }
    
        OutputTexture[dir][float3(DTid.xy, 0)] = sampleDepth;
    }
}