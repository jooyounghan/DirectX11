#include "Common.hlsli"

SamplerState Sampler : register(s0);

cbuffer ModelMatrix : register(b0)
{
    matrix mModel;
    matrix mModelInv;
};

cbuffer ViewProjMatrix : register(b1)
{
    matrix mViewProj;
    matrix mViewProjInv;
};

Texture2D HeightTexture : register(t0);
Texture2D NormalTexture : register(t1);

PixelInput main(VertexInput input)
{
    PixelInput result;

    float4x4 TransformedMat = mul(mModel, mViewProj);
    float4x4 InvTransposedTransformedMat = transpose(mul(mViewProjInv, mModel));
 
    result.fProjNorVec = mul(NormalTexture.SampleLevel(Sampler, input.fTexCoord, 0), InvTransposedTransformedMat);
    
    float4 HeightWorldPos = input.fWorldPos
    + result.fProjNorVec * HeightTexture.SampleLevel(Sampler, input.fTexCoord, 0).x;
    
    result.fWorldPos = HeightWorldPos;
    result.fProjPos = HeightWorldPos;
    //result.fProjPos = mul(result.fProjPos, mModel);
    //result.fProjPos = mul(result.fProjPos, mViewProj);
    result.fProjPos = mul(result.fProjPos, TransformedMat);
    result.fTexCoord = input.fTexCoord;
    return result;
}