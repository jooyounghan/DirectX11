#define MAX_LIGHT_NUM 10

struct VertexInput
{
    float4 f4WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float4 f4WorldNormal : NORMAL;
    float4 f4WorldTangent : TANGENT;
};
    
struct HullInput
{
    float4 f4ProjPos : SV_Position;
    float4 f4ModelPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float4 f4ModelNormal : NORMAL;
    float4 f4ModelTangent : TANGENT;
};

struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[3] : SV_TessFactor;
    float InsideTessFactor : SV_InsideTessFactor;
};


struct DomainInput
{
    float4 f4ProjPos : SV_Position;
    float4 f4ModelPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float4 f4ModelNormal : NORMAL;
    float4 f4ModelTangent : TANGENT;
};


struct PixelInput
{
    float4 f4ProjPos : SV_Position;
    float4 f4ModelPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float4 f4ModelNormal : NORMAL;
    float4 f4ModelTangent : TANGENT;
};

struct LightSet
{
    unsigned int LightType;
    float3 fDummy;
    float4 f4Location;
    float4 f4Color;
    float4 f4Direction;
    float f4LightStrength;
    float fFallOffStart;
    float fFallOffEnd;
    float fSpotPower;
};