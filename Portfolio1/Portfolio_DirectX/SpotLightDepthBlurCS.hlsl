#include "CommonFilterArgs.hlsli"

Texture2D<float> InputTexture2D : register(t0);
RWTexture2DArray<float> OutputTexture : register(u0);

SamplerState WrapSampler : register(s0);

[numthreads(32, 32, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint uiWidth;
    uint uiHeight;

    InputTexture2D.GetDimensions(uiWidth, uiHeight);
    
    float sampleDepth = 0.f;
    
    for (uint idx = 0; idx < Gaussian3x3Count; ++idx)
    {
        float2 loadPos = GetClampedTextureCoord(
            uiWidth, uiHeight,
            DTid.x + Offsets3x3[idx].x,
            DTid.y + Offsets3x3[idx].y
        );
        
        sampleDepth += InputTexture2D.SampleLevel(WrapSampler, loadPos, 0.f) * Gaussian3x3Kernel[idx];
    }
    
    OutputTexture[float3(DTid.xy, 0)] = sampleDepth;
}