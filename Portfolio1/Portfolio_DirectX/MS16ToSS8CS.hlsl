#include "CommonFilterArgs.hlsli"

Texture2DMS<float4> InputTexture2D : register(t0);

RWTexture2D<float4> ResolvedTexture : register(u0);

[numthreads(256, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    ResolvedTexture[DTid.xy] = InputTexture2D.Load(DTid.xy, 0);
}