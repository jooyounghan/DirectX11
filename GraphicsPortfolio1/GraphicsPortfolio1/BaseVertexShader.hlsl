struct VertexShaderInput
{
    float3 posModel : POSITION;
    float3 normalModel : NORMAL;
    float2 texcoord : TEXCOORD0; 
};

struct PixelShaderInput
{
    float4 posWorld : SV_POSITION; // World position (조명 계산에 사용)
    float2 texcoord : TEXCOORD;
    float3 color : COLOR; // Normal lines 쉐이더에서 사용
};

PixelShaderInput main(VertexShaderInput input)
{
    PixelShaderInput output;
    float4 pos = float4(input.posModel, 1.0f);

    output.posWorld = float4(pos.xyz, 1.0f); // 월드 위치 따로 저장
    output.texcoord = input.texcoord;

    output.color = output.posWorld; // 다른 쉐이더에서 사용

    return output;
}