SamplerState Sampler : register(s0);

Texture2D InputSRV1 : register(t8);
Texture2D InputSRV2 : register(t9);

cbuffer BlendConstantData : register(b3)
{
    float fBlendStrength;
    float fExposure;
    float fGamma;
    float fDummy;
}

struct VertexOutput
{
    float4 f4ProjPos : SV_Position;
    float2 f2TexCoord : TEXCOORD;
};

float4 main(VertexOutput vsOut) : SV_TARGET
{
    float4 blendedResult = fBlendStrength * InputSRV1.Sample(Sampler, vsOut.f2TexCoord) + (1 - fBlendStrength) * InputSRV2.Sample(Sampler, vsOut.f2TexCoord);

    float3 result = clamp(fExposure * blendedResult.xyz, 0.0, 1.0);
    float3 gammaInv = float3(1.0, 1.0, 1.0) / fGamma;
    result = pow(result, gammaInv);
    
    return float4(result.rgb, blendedResult.a);
}