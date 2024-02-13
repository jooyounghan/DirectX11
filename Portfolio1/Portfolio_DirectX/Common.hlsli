
#define NUM_CONTROL_POINTS 3

struct VertexInput
{
    float3  f3WorldPos      : POSITION;
    float2  f2TexCoord      : TEXCOORD;
    float3  f3WorldNormal   : NORMAL;
};

struct VertexOutput
{
    float4  f4ProjPos       : SV_Position;
    float4  f4ModelPos      : POSITION;
    float2  f2TexCoord      : TEXCOORD;
    float4  f4ModelNormal   : NORMAL;
};

struct HullOutput
{
    float4 f4ProjPos : SV_Position;
    float4 f4ModelPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float4 f4ModelNormal : NORMAL;
};

struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[3] : SV_TessFactor;
    float InsideTessFactor : SV_InsideTessFactor;
};

struct DomainOutput
{
    float4 f4ProjPos : SV_Position;
    float4 f4ModelPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float4 f4ModelNormal : NORMAL;
    float4 f4ModelTangent : TANGENT;
    float4 f4ModelBiTangent : BINORMAL;
};


struct PixelOutput
{
    float4  pixelColor   : SV_Target0;
    uint modelID         : SV_Target1;
};