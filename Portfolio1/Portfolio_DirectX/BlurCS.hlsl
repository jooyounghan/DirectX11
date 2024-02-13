#include "CommonFilterArgs.hlsli"

static const float GaussianKernel[9] =
{
    0.0625, 0.125, 0.0625,
    0.125, 0.5, 0.125,
    0.0625, 0.125, 0.0625
};

static const float2 Offsets[9] =
{
    { -1, -1 }, { 0, -1 }, { 1, -1 },
    { -1, 0 }, { 0, 0 }, { 1, 0 },
    { -1, 1 }, { 0, 1 }, { 1, 1 }
};

Texture2D<unorm float4> InputTexture2D : register(t0);
RWTexture2D<unorm float4> OutputTexture : register(u0);

SamplerState ClampSampler : register(s0);

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint uiWidth;
    uint uiHeight;

    InputTexture2D.GetDimensions(uiWidth, uiHeight);
    
    float4 sampleColor = float4(0.f, 0.f, 0.f, 0.f);
    
    for (uint idx = 0; idx < 9; ++idx)
    {
        float2 loadPos = float2(
            clamp(DTid.x + Offsets[idx].x, 0, uiWidth) / uiWidth, 
            clamp(DTid.y + Offsets[idx].y, 0, uiHeight) / uiHeight
        );
        sampleColor += InputTexture2D.SampleLevel(ClampSampler, loadPos, 0.f) * GaussianKernel[idx];
    }
    OutputTexture[DTid.xy] = sampleColor;
}