
#define NUM_CONTROL_POINTS 3

struct BasicVertexInput
{
    float3 f3WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3WorldNormal : NORMAL;
};

struct BasicVertexOutput
{
    float4 f4ProjPos : SV_Position;
    float2 f2TexCoord : TEXCOORD;
    float3 f3ModelNormal : NORMAL;
};

struct PBRModelVertexInput
{
    float3  f3WorldPos      : POSITION;
    float2  f2TexCoord      : TEXCOORD;
    float3  f3WorldNormal   : NORMAL;
    float3  f3WorldTangent  : TANGENT;
};

struct PBRSkeletalVertexInput
{
    float3 f3WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3WorldNormal : NORMAL;
    float3 f3WorldTangent : TANGENT;
    float4 f4BoneWeights0 : BLENDWEIGHT0;
    float4 f4BoneWeights1 : BLENDWEIGHT1;
    uint4  ui4BoneIndices0 : BLENDINDICES0;
    uint4  ui4BoneIndices1 : BLENDINDICES1;
};

struct PBRModelVertexOutput
{
    float4  f4ProjPos       : SV_Position;
    float4  f4ModelPos      : POSITION;
    float2  f2TexCoord      : TEXCOORD;
    float3  f3ModelNormal   : NORMAL;
    float3  f3ModelTangent  : TANGENT;
};

struct PBRModelHullOutput
{
    float4 f4ProjPos : SV_Position;
    float4 f4ModelPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3ModelNormal : NORMAL;
    float3 f3ModelTangent : TANGENT;
};

struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[3] : SV_TessFactor;
    float InsideTessFactor : SV_InsideTessFactor;
};

struct PBRModelDomainOutput
{
    float4 f4ProjPos : SV_Position;
    float4 f4ModelPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float3 f3ModelNormal : NORMAL;
    float3 f3ModelTangent : TANGENT;
    float3 f3ModelBiTangent : BINORMAL;
};


struct PBRModelPixelOutput
{
    float4  f4PixelColor  : SV_Target0;
    uint    uiModelID     : SV_Target1;
};