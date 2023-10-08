#include "Common.hlsli"
#include "NormalVectorShaderType.hlsli"

cbuffer ModelMatrix : register(b0)
{
    matrix mModel;
    matrix mModelInvTranspose;
};

cbuffer ViewProjMatrix : register(b1)
{
    matrix mViewProj;
    matrix mViewProjInv;
};

NormalVectorVSOutput main(VertexInput input)
{
    NormalVectorVSOutput result;
       
    float3 fModelNormal = mul(input.f4WorldNormal, mModelInvTranspose).xyz;
    fModelNormal = normalize(fModelNormal);
    
    result.f4ProjNormal = float4(fModelNormal, 0.f);
    result.f4ProjNormal = mul(result.f4ProjNormal, mViewProj);
    
    result.f4ProjPos = mul(input.f4WorldPos, mModel);
    result.f4ProjPos = mul(result.f4ProjPos, mViewProj);
    result.f2TexCoord = input.f2TexCoord;
    return result;
}