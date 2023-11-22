#include "Common.hlsli"
#include "NormalVectorShaderType.hlsli"

SamplerState Sampler : register(s0);

Texture2D NormalTexture : register(t0);

cbuffer TextureFlagBuffer : register(b0)
{
    bool bIsAOTexture;
    bool bIsColorTexture;
    bool bIsMetalnessTexture;
    bool bIsRoughnessTexture;
    bool bIsEmissionTexture;
    bool bIsNormalTexture;
    uint2 uiDummy;
};

cbuffer CameraInfoBuffer : register(b1)
{
    float4 f4CameraPos;
    matrix mViewProj;
    matrix mViewProjInv;
};


[maxvertexcount(6)]
void main(
	triangle DomainOutput input[3],
	inout LineStream<NormalVectorGSOutput> output
)
{  
	for (uint i = 0; i < 3; i++)
	{
        NormalVectorGSOutput element;
        
        float4 fNormalSampled;
        if (bIsNormalTexture)
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