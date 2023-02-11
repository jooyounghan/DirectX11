struct PixelShaderInput
{
    float4 posWorld : SV_POSITION; // World position (조명 계산에 사용)
    float2 texcoord : TEXCOORD;
    float3 color : COLOR; // Normal lines 쉐이더에서 사용
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}