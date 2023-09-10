#include "Common.hlsli"

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


HullInput main(VertexInput input)
{
    HullInput result;
       
    float3 fModelNormal = mul(input.f4WorldNormal, mModelInvTranspose).xyz;
    fModelNormal = normalize(fModelNormal);
    
    result.f4ModelNormal = float4(fModelNormal, 0.f);
    
    result.f4ModelPos = mul(input.f4WorldPos, mModel);
    result.f4ProjPos = result.f4ModelPos;
    
    result.f4ProjPos = mul(result.f4ProjPos, mViewProj);
    result.f2TexCoord = input.f2TexCoord;
    return result;
}