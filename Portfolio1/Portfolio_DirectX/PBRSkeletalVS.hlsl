#include "Common.hlsli"


cbuffer ModelMatrix : register(b0)
{
    matrix mModel;
    matrix mModelInvTranspose;
};

cbuffer CameraViewProj : register(b1)
{
    matrix mViewProj;
    matrix mViewProjInv;
};

StructuredBuffer<float4x4> boneTransformation : register(t0);

PBRModelVertexOutput main(PBRSkeletalVertexInput input)
{
    float weights[8];
    weights[0] = input.f4BoneWeights0.x;
    weights[1] = input.f4BoneWeights0.y;
    weights[2] = input.f4BoneWeights0.z;
    weights[3] = input.f4BoneWeights0.w;
    weights[4] = input.f4BoneWeights1.x;
    weights[5] = input.f4BoneWeights1.y;
    weights[6] = input.f4BoneWeights1.z;
    weights[7] = input.f4BoneWeights1.w;
    
    uint indices[8];
    indices[0] = input.ui4BoneIndices0.x;
    indices[1] = input.ui4BoneIndices0.y;
    indices[2] = input.ui4BoneIndices0.z;
    indices[3] = input.ui4BoneIndices0.w;
    indices[4] = input.ui4BoneIndices1.x;
    indices[5] = input.ui4BoneIndices1.y;
    indices[6] = input.ui4BoneIndices1.z;
    indices[7] = input.ui4BoneIndices1.w;
    
    PBRModelVertexOutput result;
    
    for (int i = 0; i < 8; ++i)
    {
        result.f3ModelNormal += weights[i] * mul(input.f3WorldNormal, (float3x3)boneTransformation[indices[i]]);
        result.f3ModelTangent += weights[i] * mul(input.f3WorldTangent, (float3x3) boneTransformation[indices[i]]);
        result.f4ModelPos += weights[i] * mul(float4(input.f3WorldPos, 1.f), boneTransformation[indices[i]]);
    }
    
    result.f3ModelNormal = normalize(mul(result.f3ModelNormal, (float3x3) mModelInvTranspose));
    result.f3ModelTangent = normalize(mul(result.f3ModelTangent, (float3x3)mModel));
    
    result.f4ModelPos = mul(result.f4ModelPos, mModel);
    result.f4ProjPos = result.f4ModelPos;
    
    result.f4ProjPos = mul(result.f4ProjPos, mViewProj);
    result.f2TexCoord = input.f2TexCoord;
    
    return result;
}