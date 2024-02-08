#include "Common.hlsli"
#include "NormalVectorShaderType.hlsli"

Texture2D NormalTexture : register(t0);

cbuffer IsPBRTextureOn : register(b0)
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

cbuffer CameraInfoBuffer : register(b1)
{
    float4 f4CameraPos;
    matrix mViewProj;
    matrix mViewProjInv;
};

SamplerState ClampSampler : register(s0);

[maxvertexcount(6)]
void main(
	triangle DomainOutput input[3],
	inout LineStream<NormalVectorGSOutput> output
)
{  
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
    
        float2x3 TB = mul(dTexXYInv, e);
        float3 fNormalSampled = 2.f * NormalTexture.SampleLevel(ClampSampler, Output.f2TexCoord, 0.f).xyz - 1.f;
        float3x3 TBN = float3x3(TB[0], TB[1], Output.f4ModelNormal.xyz);
    
        Output.f4ModelNormal = float4(normalize(mul(fNormalSampled, TBN)), 0.f);
    }
    
    
	for (uint i = 0; i < 3; i++)
	{
        NormalVectorGSOutput element;
        
        float4 fNormalSampled;
        if (bIsNormalOn)
        {
            fNormalSampled = GetSampleLeveledNormalFromTBN(
            Sampler, NormalTexture, input[i].f2TexCoord, 
            input[i].f4ModelNormal, input[i].f4ModelTangent, input[i].f4ModelBiTangent
            );
        }
        else
        {
            fNormalSampled = input[i].f4ModelNormal;
        }
        
        for (uint j = 0; j < 2; j++)
        {
            element.f4ProjNormal = mul(fNormalSampled, mViewProj);
            element.f2TexCoord = input[i].f2TexCoord;
            element.f2TexCoord.x = (float)j;
            element.f4ProjPos = input[i].f4ProjPos + element.f4ProjNormal * j;
            output.Append(element);
        }
        output.RestartStrip();
    }
}