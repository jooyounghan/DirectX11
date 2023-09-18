#include "Common.hlsli"

SamplerState Sampler : register(s0);

Texture2D HeightTexture : register(t0);

cbuffer ViewProjMatrix : register(b0)
{
    matrix mViewProj;
    matrix mViewProjInv;
};


#define NUM_CONTROL_POINTS 3

[domain("tri")]
PixelInput main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<DomainInput, NUM_CONTROL_POINTS> patch)
{
    PixelInput Output;
    
    Output.f2TexCoord = patch[0].f2TexCoord * domain.x + patch[1].f2TexCoord * domain.y + patch[2].f2TexCoord * domain.z;
    Output.f4ModelNormal = patch[0].f4ModelNormal * domain.x + patch[1].f4ModelNormal * domain.y + patch[2].f4ModelNormal * domain.z;
    
    float3 e1 = normalize((patch[1].f4ModelPos - patch[0].f4ModelPos).xyz);
    float3 e2 = normalize((patch[2].f4ModelPos - patch[0].f4ModelPos).xyz);
    
    float2 dtexXY1 = patch[1].f2TexCoord - patch[0].f2TexCoord;
    float dtexXY1Lenght = length(dtexXY1);
    float2 dtexXY2 = patch[2].f2TexCoord - patch[0].f2TexCoord;
    float dtexXY2Lenght = length(dtexXY2);
    
    float2x2 dTexXY = float2x2(dtexXY1, dtexXY2);
    float2x2 dTexXYInv = Get2X2InvMatrix(dTexXY);
    float2x3 e = float2x3(dtexXY1Lenght * e1, dtexXY2Lenght * e2);
    
    float2x3 TB = mul(dTexXYInv, e);

    Output.f4ModelNormal = normalize(Output.f4ModelNormal);
    Output.f4ModelTangent = normalize(float4(TB[0], 0.f));
    Output.f4ModelBiTangent = normalize(float4(TB[1], 0.f));

    Output.f4ModelPos = patch[0].f4ModelPos * domain.x + patch[1].f4ModelPos * domain.y + patch[2].f4ModelPos * domain.z;

    float fHeightSampled = 2.f * HeightTexture.SampleLevel(Sampler, Output.f2TexCoord, 0.f).x - 1.f;
    fHeightSampled = 0.1 * fHeightSampled;
    
    Output.f4ModelPos += fHeightSampled * Output.f4ModelNormal;
    Output.f4ProjPos = Output.f4ModelPos;
    Output.f4ProjPos = mul(Output.f4ProjPos, mViewProj);   
	return Output;
}
