#include "CommonFilterArgs.hlsli"

Texture2D<unorm float4> InputTexture2D : register(t0);
RWTexture2D<unorm float4> OutputTexture : register(u0);

SamplerState WrapSampler : register(s0);

[numthreads(32, 32, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint uiWidth;
    uint uiHeight;

    InputTexture2D.GetDimensions(uiWidth, uiHeight);
    
    float4 sampleColor = float4(0.f, 0.f, 0.f, 0.f);
    
    for (uint idx = 0; idx < Gaussian5x5Count; ++idx)
    {
        float2 loadPos = GetClampedTextureCoord(
            uiWidth, uiHeight,
            DTid.x + Offsets5x5[idx].x,
            DTid.y + Offsets5x5[idx].y
        );
                
        sampleColor += InputTexture2D.SampleLevel(WrapSampler, loadPos, 0.f) * Gaussian5x5Kernel[idx];
    }
    OutputTexture[DTid.xy] = sampleColor;
}