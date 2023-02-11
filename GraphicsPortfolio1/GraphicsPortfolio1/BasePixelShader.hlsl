struct PixelShaderInput
{
    float4 posProj : SV_POSITION;
    float3 posWorld : POSITION;
    float2 texcoord : TEXCOORD;
    float3 color : COLOR; //
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	return float4(input.color, 1.0f);
}