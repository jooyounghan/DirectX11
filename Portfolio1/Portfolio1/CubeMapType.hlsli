struct CubeMapVertexInput
{
    float4 f4WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float4 f4WorldNormal : NORMAL;
};

struct CubeMapPixelInput
{
    float4 f4WorldPos : POSITION;
    float4 f4ViewProjPos : SV_Position;
};