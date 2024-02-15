#include "CommonFilterArgs.hlsli"

Texture2D<float4> InputTexture2D : register(t0);

RWTexture2D<unorm float4> ResolvedTexture : register(u0);

SamplerState ClampSampler : register(s0);

[numthreads(256, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint uiWidth;
    uint uiHeight;
    InputTexture2D.GetDimensions(uiWidth, uiHeight);
    
    float3 f3loadPos = float3(clamp(DTid.x, 0, uiWidth), clamp(DTid.y, 0, uiHeight), 0);
    ResolvedTexture[DTid.xy] = InputTexture2D.Load(f3loadPos);
}