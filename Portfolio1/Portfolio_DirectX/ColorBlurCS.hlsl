#include "CommonFilterArgs.hlsli"

Texture2D<unorm float4> InputTexture2D : register(t0);
RWTexture2D<unorm float4> OutputTexture : register(u0);

SamplerState WrapSampler : register(s0);

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint uiWidth;
    uint uiHeight;

    InputTexture2D.GetDimensions(uiWidth, uiHeight);
    
    float4 sampleColor = float4(0.f, 0.f, 0.f, 0.f);
    
    for (uint idx = 0; idx < Gaussian3x3Count; ++idx)
    {
        float2 loadPos = float2(
            clamp(DTid.x + Offsets3x3[idx].x, 0, uiWidth) / uiWidth, 
            clamp(DTid.y + Offsets3x3[idx].y, 0, uiHeight) / uiHeight
        );
        sampleColor += InputTexture2D.SampleLevel(WrapSampler, loadPos, 0.f) * Gaussian3x3Kernel[idx];
    }
    OutputTexture[DTid.xy] = sampleColor;
}