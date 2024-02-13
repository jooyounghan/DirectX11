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
DomainOutput main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<HullOutput, NUM_CONTROL_POINTS> patch)
{
    DomainOutput Output;
    
    Output.f4ModelPos = patch[0].f4ModelPos * domain.x + patch[1].f4ModelPos * domain.y + patch[2].f4ModelPos * domain.z;
    Output.f2TexCoord = patch[0].f2TexCoord * domain.x + patch[1].f2TexCoord * domain.y + patch[2].f2TexCoord * domain.z;
              
    float2x3 TB = GetTBMatrix(patch[0].f4ModelPos, patch[1].f4ModelPos, patch[2].f4ModelPos, patch[0].f2TexCoord, patch[1].f2TexCoord, patch[2].f2TexCoord);
    
    Output.f4ModelNormal = normalize(patch[0].f4ModelNormal * domain.x + patch[1].f4ModelNormal * domain.y + patch[2].f4ModelNormal * domain.z); 
    Output.f4ModelTangent = normalize(float4(TB[0], 0.f));
    Output.f4ModelBiTangent = normalize(float4(TB[1], 0.f));
    
    if (bIsHeightOn)
    {
        float fHeightSampled = 2.f * HeightTexture.SampleLevel(ClampSampler, Output.f2TexCoord, 0.f).x - 1.f;
        fHeightSampled = fHeightFactor * fHeightSampled;
        Output.f4ModelPos += fHeightSampled * Output.f4ModelNormal;        
    }

    Output.f4ProjPos = mul(Output.f4ModelPos, mViewProj);
    
    return Output;
}
