SamplerState Sampler : register(s0);
Texture2D Texture : register(t0);

struct PixelInput
{
    float4 fProjPos : SV_Position;
    float4 fWorldPos : POSITION;
    float2 fTexCoord : TEXCOORD;
    float4 fNorVec : NORMAL;
};

float4 main(PixelInput input) : SV_TARGET
{
    return Texture.Sample(Sampler, float2(input.fTexCoord.x, input.fTexCoord.y));
}
