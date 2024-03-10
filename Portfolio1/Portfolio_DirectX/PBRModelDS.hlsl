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

Texture2D HeightTexture : register(t0);

SamplerState ClampSampler : register(s0);

[domain("tri")]
PBRModelDomainOutput main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<PBRModelHullOutput, NUM_CONTROL_POINTS> patch)
{
    PBRModelDomainOutput Output;
    
    Output.f4ModelPos = patch[0].f4ModelPos * domain.x + patch[1].f4ModelPos * domain.y + patch[2].f4ModelPos * domain.z;
    Output.f2TexCoord = patch[0].f2TexCoord * domain.x + patch[1].f2TexCoord * domain.y + patch[2].f2TexCoord * domain.z;
              
    Output.f3ModelNormal = normalize(patch[0].f3ModelNormal * domain.x + patch[1].f3ModelNormal * domain.y + patch[2].f3ModelNormal * domain.z);
    Output.f3ModelTangent = normalize(patch[0].f3ModelTangent * domain.x + patch[1].f3ModelTangent * domain.y + patch[2].f3ModelTangent * domain.z);
    Output.f3ModelBiTangent = normalize(cross(Output.f3ModelNormal.xyz, Output.f3ModelTangent.xyz));
    
    Output.f4ModelPos = GetHeightedModelPos(
        bIsHeightOn, HeightTexture, ClampSampler, Output.f2TexCoord, fHeightFactor, Output.f3ModelNormal, Output.f4ModelPos
    );
    
    Output.f4ProjPos = mul(Output.f4ModelPos, mViewProj);
    
    return Output;
}
