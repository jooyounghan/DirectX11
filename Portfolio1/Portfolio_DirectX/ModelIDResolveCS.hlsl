#include "CommonFilterArgs.hlsli"

Texture2DMS<uint> InputTexture2D : register(t0);

RWTexture2D<uint> ResolvedTexture : register(u0);

[numthreads(32, 32, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint uiWidth;
    uint uiHeight;
    uint uiLevels;

    InputTexture2D.GetDimensions(uiWidth, uiHeight, uiLevels);
    ResolvedTexture[DTid.xy] = 0;
    
    uint sampleValues[MAX_MS_COUNT];
    uint sampleHistos[MAX_MS_COUNT];
    uint pointIdx = 0;
    for (uint sampleIdx = 0; sampleIdx < MAX_MS_COUNT; ++sampleIdx)
    {
        float2 loadPos = GetClampedTextureCoord(
            uiWidth, uiHeight,
            DTid.x + Offsets3x3[sampleIdx].x,
            DTid.y + Offsets3x3[sampleIdx].y
        );
        
        uint loadedValue = InputTexture2D.Load(loadPos, sampleIdx);

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
    
    ResolvedTexture[DTid.xy] = InputTexture2D.Load(DTid.xy, maxIdx);
}