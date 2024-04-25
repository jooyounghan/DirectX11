#include "CommonFilterArgs.hlsli"

Texture2DMS<float4> InputTexture2D : register(t0);

RWTexture2D<unorm float4> ResolvedTexture : register(u0);

[numthreads(32, 32, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint uiWidth, uiHeight, uiSampleCount;
    InputTexture2D.GetDimensions(uiWidth, uiHeight, uiSampleCount);
    
    float4 accumulatedColor = float4(0, 0, 0, 0);
    uint validSampleCount = 0;

    for (uint idx = 0; idx < uiSampleCount; ++idx)
    {
        float4 sampleColor = InputTexture2D.Load(DTid.xy, idx);
        
        if (length(sampleColor) > 0.0f)
        {
            accumulatedColor += sampleColor;
            validSampleCount++;
        }
    }

    if (validSampleCount > 0)
    {
        ResolvedTexture[DTid.xy] = accumulatedColor / validSampleCount;
    }
    else
    {
        ResolvedTexture[DTid.xy] = float4(0, 0, 0, 1);
    }
}