struct NormalVectorVSInput
{
    float3 f3WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3WorldNormal : NORMAL;
    float3 f3WorldTangent : TANGENT;
};

struct NormalVectorVSOutput
{
    float4 f4ModelPos : SV_Position;
    float2 f2TexCoord : TEXCOORD;
    float3 f3ModelNormal : NORMAL;
    float3 f3ModelTangent : TANGENT;
};


struct NormalVectorGSOutput
{
    float4 f4ProjPos : SV_Position;
    float2 f2TexCoord : TEXCOORD;
};

struct NormalVectorPSOutput
{
    float4 f4PixelColor : SV_Target0;
};