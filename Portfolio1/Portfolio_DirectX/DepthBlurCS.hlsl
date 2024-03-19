#include "CommonFilterArgs.hlsli"

Texture2D<float> InputTexture2D : register(t0);
RWTexture2DArray<float> OutputTexture : register(u0);

SamplerState WrapSampler : register(s0);

[numthreads(200, 4, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint uiWidth;
    uint uiHeight;

    InputTexture2D.GetDimensions(uiWidth, uiHeight);
    
    float sampleDepth = 0.f;
    
    for (uint idx = 0; idx < Gaussian7x7Count; ++idx)
    {
        float2 loadPos = float2(
            clamp(DTid.x + Offsets7x7[idx].x, 0, uiWidth) / uiWidth,
            clamp(DTid.y + Offsets7x7[idx].y, 0, uiHeight) / uiHeight
        );
        sampleDepth += InputTexture2D.SampleLevel(WrapSampler, loadPos, 0.f) * GaussianKernel7x7[idx];
    }
    OutputTexture[float3(DTid.xy, 0)] = sampleDepth;
}