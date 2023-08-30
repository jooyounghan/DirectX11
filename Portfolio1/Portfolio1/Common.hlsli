#define MAX_LIGHT_NUM 10

struct VertexInput
{
    float4 f4WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float4 f4WorldNormal : NORMAL;
    float4 f4WorldTangent : TANGENT;
};
    
struct PixelInput
{
    float4 f4ProjPos : SV_Position;
    float4 f4WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float4 f4WorldNorVec : NORMAL;
};

struct LightSet
{
    unsigned int LightType;
    float4 f4Location;
    float4 f4Color;
    float4 f4Direction;
    float4 f4LightStrength;
    float fFallOffStart;
    float fFallOffEnd;
};