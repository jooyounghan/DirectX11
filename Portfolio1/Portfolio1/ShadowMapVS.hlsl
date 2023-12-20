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

cbuffer PointViewProj : register(b3)
{
    matrix mPointViewProj[6];
    matrix mPointViewProjInvTranspose[6];
};

cbuffer SpotViewProj: register(b4)
{
    matrix mSpotViewProj;
    matrix mSpotViewProjInvTranspose;
};

float4 main(float4 f4WorldPos : POSITION) : SV_Position
{
    return mul(mul(f4WorldPos, mModel), mViewProj);
}

// TODO Point의 경우에는 6개에 대한 아웃풋이 다 나와야하는데 이를 한번에 해줄 수 없음. 각각에 그려야하므로 buffer를 별도로 6개 만드는 수정 필요