#define MAX_LIGHT_NUM 10

struct VertexInput
{
    float4 f4WorldPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float4 f4WorldNormal : NORMAL;
};
    
struct VertexOutput
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


struct HullOutput
{
    float4 f4ProjPos : SV_Position;
    float4 f4ModelPos : POSITION;
    float2 f2TexCoord : TEXCOORD;
    float4 f4ModelNormal : NORMAL;
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
    float4 pixelColor : SV_Target0;
    float4 modleId : SV_Target1;
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

struct ModelID
{
    
    uint3   uiModelID;
    uint    uiModelIDStd;
};

float2x2 Get2X2InvMatrix(float2x2 input)
{
    float ad_bc = (1.0 / (input._m00 * input._m11 - input._m01 * input._m10));
    float2x2 f22Result = float2x2(input._m11, -input._m01, -input._m10, input._m00);
    return ad_bc * f22Result;
}