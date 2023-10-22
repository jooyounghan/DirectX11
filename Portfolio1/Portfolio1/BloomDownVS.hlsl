struct VertexInput
{
    float4 f4ProjPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
};

struct VertexOutput
{
    float4 f4ProjPos : SV_Position;
    float2 f2TexCoord : TEXCOORD;
};


VertexOutput main(VertexInput pos)
{
    VertexOutput result;
    result.f4ProjPos = pos.f4ProjPos;
    result.f2TexCoord = pos.f2TexCoord;
    return result;
}