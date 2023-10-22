SamplerState Sampler : register(s0);

Texture2D InputSRV : register(t0);

struct VertexOutput
{
    float4 f4ProjPos : SV_Position;
    float2 f2TexCoord : TEXCOORD;
};

float4 main(VertexOutput vsOut) : SV_TARGET
{
    float4 result = InputSRV.Sample(Sampler, vsOut.f2TexCoord);
    return result;
}