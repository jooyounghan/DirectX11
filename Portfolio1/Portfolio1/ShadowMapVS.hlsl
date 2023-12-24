#include "Common.hlsli"

cbuffer ModelMatrix : register(b0)
{
    matrix mModel;
    matrix mModelInvTranspose;
};

cbuffer LightSetsBuffer : register(b2)
{
    unsigned int uiLightType;
    float4 f4Location;
    float4 f4Direct;
    float3 f3LightColor;
    float fFallOffStart;
    float fFallOffEnd;
    float fLightPower;
    float fSpotPower;
};

cbuffer LightViewProj : register(b3)
{
    matrix mLightViewProj;
    matrix mLightViewProjInvTranspose;
};


float4 main(VertexInput input) : SV_Position
{
    return mul(mul(input.f4WorldPos, mModel), mLightViewProj);
}