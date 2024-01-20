struct VertexInput
{
    float3  f4WorldPos      : POSITION;
    float2  f2TexCoord      : TEXCOORD;
    float3  f4WorldNormal   : NORMAL;
    uint    uiModelID       : BLENDINDICES;
};

struct VertexOutput
{
    float4  f4ProjPos       : SV_Position;
    float4  f4ModelPos      : POSITION;
    float2  f2TexCoord      : TEXCOORD;
    float4  f4ModelNormal   : NORMAL;
};

struct PixelOutput
{
    float4 pixelColor   : SV_Target0;
};