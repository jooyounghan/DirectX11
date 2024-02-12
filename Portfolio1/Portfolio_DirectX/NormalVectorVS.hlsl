#include "NormalVector.hlsli"

cbuffer ModelMatrix : register(b0)
{
    matrix mModel;
    matrix mModelInvTranspose;
};

NormalVectorVSOutput main(NormalVectorVSInput input)
{
    NormalVectorVSOutput result;
       
    float3 fModelNormal = mul(float4(input.f4WorldNormal, 0.f), mModelInvTranspose).xyz;
    fModelNormal = normalize(fModelNormal);
    
    result.f4ModelNormal = float4(fModelNormal, 0.f);
    result.f4ModelPos = mul(float4(input.f4WorldPos, 1.f), mModel);
    result.f2TexCoord = input.f2TexCoord;
    
    return result;
}