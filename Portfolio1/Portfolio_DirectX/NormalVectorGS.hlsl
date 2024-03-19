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
    bool bIsDiffuseOn;
    bool bIsSpecularOn;
    bool bIsMetalnessOn;
    bool bIsRoughnessOn;
    bool bIsEmissionOn;
    bool bIsNormalOn;
    bool bIsHeightOn;
    bool bIsGLTF;
    bool2 bDummys;
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
    for (uint i = 0; i < 3; i++)
    {
        NormalVectorGSOutput element;

        float3 f3ModelBiTangent = normalize(cross(input[i].f3ModelNormal.xyz, input[i].f3ModelTangent.xyz));
        float3 f3ModelNormal = normalize(GetNormalFromTexture(
            bIsNormalOn, NormalTexture, ClampSampler, input[i].f2TexCoord, input[i].f3ModelTangent, f3ModelBiTangent, input[i].f3ModelNormal
        ));

        float4 f4ModelPos = GetHeightedModelPos(
            bIsHeightOn, HeightTexture, ClampSampler, input[i].f2TexCoord, fHeightFactor, f3ModelNormal, input[i].f4ModelPos
        );
       
        for (uint j = 0; j < 2; j++)
        {
            element.f4ProjPos = mul(f4ModelPos + 0.1f * float4(f3ModelNormal, 0.f) * j, mViewProj);
            element.f2TexCoord = float2(j, 0);
            output.Append(element);
        }
        output.RestartStrip();
    }
}