struct PixelInput
{
    float4 fProjPos : SV_Position;
    float4 fWorldPos : POSITION;
    float2 fTexCoord : TEXCOORD;
    float4 fNorVec : NORMAL;
};

float4 main(PixelInput input) : SV_TARGET
{
    return float4(abs(input.fNorVec.rgb), 1.f);
}
