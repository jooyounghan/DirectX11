#include "CommonFilterArgs.hlsli"

Texture2D<float4> InputTexture2D : register(t0);

RWTexture2D<unorm float4> ResolvedTexture : register(u0);

SamplerState ClampSampler : register(s0);

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint uiWidth, uiHeight;
    InputTexture2D.GetDimensions(uiWidth, uiHeight);
    
    float4 accumulatedColor = float4(0, 0, 0, 0);
    uint validSampleCount = 0;

    float2 loadPos = float2(clamp(DTid.x, 0, uiWidth), clamp(DTid.y, 0, uiHeight));
    ResolvedTexture[DTid.xy] = InputTexture2D.SampleLevel(ClampSampler, loadPos, 0.f);
}