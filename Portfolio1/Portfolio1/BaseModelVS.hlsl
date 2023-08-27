cbuffer ModelMatrix : register(b0)
{
    matrix mModel;
};

cbuffer ViewProjMatrix : register(b1)
{
    matrix mViewProj;
};


struct VertexInput
{
    float4 fWorldPos : POSITION;
    float2 fTexCoord : TEXCOORD;
    float4 fNorVec : NORMAL;
};
    
struct PixelInput
{
    float4 fProjPos : SV_Position;
    float4 fWorldPos : POSITION;
    float2 fTexCoord : TEXCOORD;
    float4 fNorVec : NORMAL;
};

PixelInput main(VertexInput input)
{
    PixelInput result;
    result.fWorldPos = input.fWorldPos;
    result.fProjPos = input.fWorldPos;
    result.fProjPos = mul(result.fProjPos, mModel);
    result.fProjPos = mul(result.fProjPos, mViewProj);
    
    result.fTexCoord = input.fTexCoord;
    result.fNorVec = input.fNorVec;
    return result;
}