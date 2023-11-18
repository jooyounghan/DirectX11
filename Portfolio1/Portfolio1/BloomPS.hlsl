SamplerState Sampler : register(s0);

Texture2D InputSRV : register(t9);

cbuffer ViewportConstantData : register(b5)
{
    float fDeltaWidth;
    float fDeltaHeight;
    float fDummy[2];
}

struct VertexOutput
{
    float4 f4ProjPos : SV_Position;
    float2 f2TexCoord : TEXCOORD;
};


float4 main(VertexOutput vsOut) : SV_TARGET
{
    float4 a = InputSRV.Sample(Sampler, float2(vsOut.f2TexCoord.x - fDeltaWidth, vsOut.f2TexCoord.y + fDeltaHeight));
    float4 b = InputSRV.Sample(Sampler, float2(vsOut.f2TexCoord.x              , vsOut.f2TexCoord.y + fDeltaHeight));
    float4 c = InputSRV.Sample(Sampler, float2(vsOut.f2TexCoord.x + fDeltaWidth, vsOut.f2TexCoord.y + fDeltaHeight));
    
    float4 d = InputSRV.Sample(Sampler, float2(vsOut.f2TexCoord.x - fDeltaWidth, vsOut.f2TexCoord.y               ));
    float4 e = InputSRV.Sample(Sampler, float2(vsOut.f2TexCoord.x              , vsOut.f2TexCoord.y               ));
    float4 f = InputSRV.Sample(Sampler, float2(vsOut.f2TexCoord.x + fDeltaWidth, vsOut.f2TexCoord.y               ));
    
    float4 g = InputSRV.Sample(Sampler, float2(vsOut.f2TexCoord.x - fDeltaWidth, vsOut.f2TexCoord.y - fDeltaHeight));
    float4 h = InputSRV.Sample(Sampler, float2(vsOut.f2TexCoord.x              , vsOut.f2TexCoord.y - fDeltaHeight));
    float4 i = InputSRV.Sample(Sampler, float2(vsOut.f2TexCoord.x + fDeltaWidth, vsOut.f2TexCoord.y - fDeltaHeight));
    
    float4 result = e * 4.0;
    result += (b + d + f + h) * 2.0;
    result += (a + c + g + i);
    result /= 16.0;
    
    return result;
}