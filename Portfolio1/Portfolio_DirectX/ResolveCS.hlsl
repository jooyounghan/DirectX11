#include "CommonFilterArgs.hlsli"

SamplerState ClampSampler : register(s0);

Texture2DMS<uint> InputTexture2D : register(t0);

RWTexture2D<uint> ResolvedTexture : register(u0);

[numthreads(256, 1, 1)]
void main( uint3 DTId : SV_DispatchThreadID )
{
    uint uiWidth;
    uint uiHeight;
    uint uiLevels;

    InputTexture2D.GetDimensions(uiWidth, uiHeight, uiLevels);
    ResolvedTexture[DTId.xy] = 0;
    
    uint sampleValues[MAX_MS_COUNT];
    uint sampleHistos[MAX_MS_COUNT];
    uint pointIdx = 0;
    for (uint sampleIdx = 0; sampleIdx < MAX_MS_COUNT; ++sampleIdx)
    {
        uint loadedValue = InputTexture2D.Load(DTId.xy, sampleIdx);

        for (uint idx = 0; idx < MAX_MS_COUNT; ++idx)
        {
            if (idx < pointIdx)
            {
                if (sampleValues[idx] == loadedValue)
                {
                    sampleHistos[idx]++;
                    break;
                }
            }
            else
            {
                sampleValues[pointIdx] = loadedValue;
                sampleHistos[pointIdx] = 1;
                pointIdx++;
                break;
            }
        }
    }

    uint maxIdx = 0;
    for (uint i = 0; i < MAX_MS_COUNT; ++i)
    {
        if (sampleHistos[maxIdx] < sampleHistos[i])
        {
            maxIdx = i;
        }
    }
    
    ResolvedTexture[DTId.xy] = InputTexture2D.Load(DTId.xy, maxIdx);
}