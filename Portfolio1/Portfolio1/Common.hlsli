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
    float4 fProjNorVec : NORMAL;
};

