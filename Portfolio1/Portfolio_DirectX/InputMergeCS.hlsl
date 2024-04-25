#include "CommonFilterArgs.hlsli"

Texture2D<unorm float4> Input1Texture2D : register(t0);
Texture2D<unorm float4> Input2Texture2D : register(t1);

RWTexture2D<unorm float4> OutputTexture : register(u0);

SamplerState WrapSampler : register(s0);

[numthreads(32, 32, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint uiWidth1;
    uint uiHeight1;
    uint uiWidth2;
    uint uiHeight2;
    
    Input1Texture2D.GetDimensions(uiWidth1, uiHeight1);
    Input2Texture2D.GetDimensions(uiWidth2, uiHeight2);
    
    float2 load1Pos = GetClampedTextureCoord(uiWidth1, uiHeight1, DTid.x, DTid.y);
    float2 load2Pos = GetClampedTextureCoord(uiWidth2, uiHeight2, DTid.x, DTid.y);
    
    OutputTexture[DTid.xy] = Input1Texture2D.SampleLevel(WrapSampler, load1Pos, 0.f) * 0.5 +
        Input2Texture2D.SampleLevel(WrapSampler, load2Pos, 0.f) * 0.5;
}