struct NormalVectorGSOutput
{
    float4 f4ProjPos : SV_Position;
    float2 f2TexCoord : TEXCOORD;
    float4 f4ProjNormal : NORMAL;
};

struct NormalVectorPSOutput
{
    float4 pixelColor : SV_Target;
};