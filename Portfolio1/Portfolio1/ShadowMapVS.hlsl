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

// TODO Point�� ��쿡�� 6���� ���� �ƿ�ǲ�� �� ���;��ϴµ� �̸� �ѹ��� ���� �� ����. ������ �׷����ϹǷ� buffer�� ������ 6�� ����� ���� �ʿ�