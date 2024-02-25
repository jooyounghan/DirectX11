#include "NormalVector.hlsli"

cbuffer ModelMatrix : register(b0)
{
    matrix mModel;
    matrix mModelInvTranspose;
};

NormalVectorVSOutput main(NormalVectorVSInput input)
{
    NormalVectorVSOutput result;
       
    result.f3ModelNormal = normalize(mul(float4(input.f3WorldNormal, 0.f), mModelInvTranspose)).xyz;
    result.f3ModelTangent = normalize(mul(float4(input.f3WorldTangent, 0.f), mModel)).xyz;
    result.f4ModelPos = mul(float4(input.f4WorldPos, 1.f), mModel);
    result.f2TexCoord = input.f2TexCoord;
    
    return result;
}