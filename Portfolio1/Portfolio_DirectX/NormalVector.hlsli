struct NormalVectorVSInput
{
    float3 f4WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f4WorldNormal : NORMAL;
};

struct NormalVectorVSOutput
{
    float4 f4ModelPos : SV_Position;
    float2 f2TexCoord : TEXCOORD;
    float4 f4ModelNormal : NORMAL;
};

struct NormalVectorGSOutput
{
    float4 f4ProjPos : SV_Position;
    float2 f2TexCoord : TEXCOORD;
};

struct NormalVectorPSOutput
{
    float4 pixelColor : SV_Target;
};