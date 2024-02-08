#include "Common.hlsli"
#include "MathematicalHelper.hlsli"

cbuffer PBRConstant : register(b0)
{
    float3 fFresnelConstant;
    float fHeightFactor;
};

cbuffer CameraViewProj : register(b1)
{
    matrix mViewProj;
    matrix mViewProjInv;
};

cbuffer IsPBRTextureOn : register(b2)
{
    bool bIsAOOn;
    bool bIsColorOn;
    bool bIsMetalnessOn;
    bool bIsRoughnessOn;
    bool bIsEmissionOn;
    bool bIsNormalOn;
    bool bIsHeightOn;
    bool bDummy;
};

Texture2D NormalTexture : register(t0);
Texture2D HeightTexture : register(t1);

SamplerState WrapSampler : register(s0);

[domain("tri")]
DomainOutput main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<HullOutput, NUM_CONTROL_POINTS> patch)
{
    DomainOutput Output;
    
    Output.f4ModelPos = patch[0].f4ModelPos * domain.x + patch[1].f4ModelPos * domain.y + patch[2].f4ModelPos * domain.z;
    Output.f2TexCoord = patch[0].f2TexCoord * domain.x + patch[1].f2TexCoord * domain.y + patch[2].f2TexCoord * domain.z;
    Output.f4ModelNormal = normalize(patch[0].f4ModelNormal * domain.x + patch[1].f4ModelNormal * domain.y + patch[2].f4ModelNormal * domain.z);
            
    float fHeightSampled = 2.f * HeightTexture.SampleLevel(WrapSampler, Output.f2TexCoord, 0.f).x - 1.f;
    fHeightSampled = fHeightFactor * fHeightSampled;
    
    Output.f4ModelPos += fHeightSampled * Output.f4ModelNormal;
    Output.f4ProjPos = Output.f4ModelPos;
    Output.f4ProjPos = mul(Output.f4ProjPos, mViewProj);

    if (bIsNormalOn)
    {
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
    
        float3 fNormalSampled = 2.f * NormalTexture.SampleLevel(WrapSampler, Output.f2TexCoord, 0.f).xyz - 1.f;
        float3x3 TBN = float3x3(TB[0], TB[1], Output.f4ModelNormal.xyz);
    
        Output.f4ModelNormal = float4(normalize(mul(fNormalSampled, TBN)), 0.f);
    }

    return Output;
}
