#include "NormalVector.hlsli"

cbuffer ModelMatrix : register(b0)
{
    matrix mModel;
    matrix mModelInvTranspose;
};


NormalVectorVSOutput main(NormalVectorVSInput input)
{
    NormalVectorVSOutput result;
       
    result.f4ModelNormal = mul(float4(input.f4WorldNormal, 0.f), mModelInvTranspose);
    result.f4ModelNormal = normalize(result.f4ModelNormal);
    
    result.f4ModelPos = mul(float4(input.f4WorldPos, 1.f), mModel);
    result.f2TexCoord = input.f2TexCoord;
    
    return result;
}