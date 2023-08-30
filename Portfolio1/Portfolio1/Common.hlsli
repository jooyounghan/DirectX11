#define MAX_LIGHT_NUM 10

struct VertexInput
{
    float4 fWorldPos : POSITION;
    float2 fTexCoord : TEXCOORD;
    float4 fWorldNormal : NORMAL;
    float4 fWorldTangent : TANGENT;
};
    
struct PixelInput
{
    float4 fProjPos : SV_Position;
    float4 fWorldPos : POSITION;
    float2 fTexCoord : TEXCOORD;
    float4 fWorldNorVec : NORMAL;
};

struct LightSet
{
    unsigned int LightType;
    float4 fLocation;
    float4 fColor;
    float4 fDirection;
    float3 fDummy;
};