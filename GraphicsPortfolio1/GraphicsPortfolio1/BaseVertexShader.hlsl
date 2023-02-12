struct VertexShaderInput
{
    float3 posModel : POSITION;
    float3 normalModel : NORMAL;
    float2 texcoord : TEXCOORD; 
};

struct PixelShaderInput
{
    float4 posProj : SV_POSITION;
    float3 posWorld : POSITION;
    float2 texcoord : TEXCOORD;
    float3 color : COLOR; //
};

cbuffer BasicVertexConstantData : register(b0)
{
    matrix view;
    matrix projection;
};

PixelShaderInput main(VertexShaderInput input)
{
    PixelShaderInput output;
    
    float4 pos = float4(input.posModel, 1.0f);
    output.posWorld = pos.xyz;

    pos = mul(pos, view);
    pos = mul(pos, projection);

    output.posProj = pos;
    output.texcoord = input.texcoord;

    output.color = float3(0.0f, 0.0f, 0.0f);

    return output;
}