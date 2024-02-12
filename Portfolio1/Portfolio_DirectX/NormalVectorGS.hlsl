#include "NormalVector.hlsli"
#include "MathematicalHelper.hlsli"

cbuffer PBRConstant : register(b0)
{
    float3 fFresnelConstant;
    float fHeightFactor;
};

cbuffer CameraViewProj : register(b1)
{
    matrix mViewProj;
    matrix mViewProjInv;
};

cbuffer IsPBRTextureOn : register(b2)
{
    bool bIsAOOn;
    bool bIsColorOn;
    bool bIsMetalnessOn;
    bool bIsRoughnessOn;
    bool bIsEmissionOn;
    bool bIsNormalOn;
    bool bIsHeightOn;
    bool bDummy;
};

Texture2D NormalTexture : register(t0);
Texture2D HeightTexture : register(t1);

SamplerState ClampSampler : register(s0);

[maxvertexcount(6)]
void main(
	triangle NormalVectorVSOutput input[3],
	inout LineStream<NormalVectorGSOutput> output
)
{    
    float2x3 TB;
    if (bIsNormalOn)
    {
        float3 e1 = normalize((input[1].f4ModelPos - input[0].f4ModelPos).xyz);
        float3 e2 = normalize((input[2].f4ModelPos - input[0].f4ModelPos).xyz);
    
        float2 dtexXY1 = input[1].f2TexCoord - input[0].f2TexCoord;
        float dtexXY1Lenght = length(dtexXY1);
        float2 dtexXY2 = input[2].f2TexCoord - input[0].f2TexCoord;
        float dtexXY2Lenght = length(dtexXY2);
    
        float2x2 dTexXY = float2x2(dtexXY1, dtexXY2);
        float2x2 dTexXYInv = Get2X2InvMatrix(dTexXY);
        float2x3 e = float2x3(dtexXY1Lenght * e1, dtexXY2Lenght * e2);
    
        TB = mul(dTexXYInv, e);
    }
    
    for (uint i = 0; i < 3; i++)
    {
        NormalVectorGSOutput element;

        float4 f4ModelNormal = input[i].f4ModelNormal;
        if (bIsNormalOn)
        {
            float3 fNormalSampled = 2.f * NormalTexture.SampleLevel(ClampSampler, input[i].f2TexCoord, 0.f).xyz - 1.f;
            float3x3 TBN = float3x3(TB[0], TB[1], input[i].f4ModelNormal.xyz);
            f4ModelNormal = float4(normalize(mul(fNormalSampled, TBN)), 0.f);
        }
        
        float4 f4ModelPos = input[i].f4ModelPos;
        if (bIsHeightOn)
        {
            float fHeightSampled = 2.f * HeightTexture.SampleLevel(ClampSampler, input[i].f2TexCoord, 0.f).x - 1.f;
            fHeightSampled = fHeightFactor * fHeightSampled;
            f4ModelPos += fHeightSampled * f4ModelNormal;
        }
       
        for (uint j = 0; j < 2; j++)
        {
            element.f4ProjPos = mul(f4ModelPos + f4ModelNormal * j, mViewProj);
            element.f2TexCoord = float2(j, 0);
            output.Append(element);
        }
        output.RestartStrip();
    }
}