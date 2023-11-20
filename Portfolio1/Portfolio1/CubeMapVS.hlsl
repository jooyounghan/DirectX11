#include "CubeMapType.hlsli"

cbuffer CameraInfo : register(b1)
{
    float4 f4CameraPos;
    matrix mViewProj;
    matrix mViewProjInv;
};

    
CubeMapPixelInput main(CubeMapVertexInput input)
{
    CubeMapPixelInput output;
    output.f4WorldPos = input.f4WorldPos;
    output.f4ViewProjPos = mul(input.f4WorldPos, mViewProj);
    return output;
}