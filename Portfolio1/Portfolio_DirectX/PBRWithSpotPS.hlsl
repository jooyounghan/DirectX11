#include "Common.hlsli"
#include "BrdfHelper.hlsli"
#include "MathematicalHelper.hlsli"
#include "ShadowHelper.hlsli"

Texture2D ColorTexture : register(t0);
Texture2D DiffuseTexture : register(t1);
Texture2D SpecularTexture : register(t2);
Texture2D MetalnessTexture : register(t3);
Texture2D RoughnessTexture : register(t4);
Texture2D EmissionTexture : register(t5);
Texture2D NormalTexture : register(t6);
Texture2D ShadowMap : register(t7);

cbuffer ModelIDBuffer : register(b0)
{
    uint uIMeshId;
    uint uiDummy[3];
};

cbuffer LightPos : register(b1)
{
    float4 f4LightPos;
};

cbuffer LightData : register(b2)
{
    float3  f3LightColor;
    float   fFallOffStart;
    float   fFallOffEnd;
    float   fLightPower;
    float2  f2Dummy;
}

cbuffer SpotLightData : register(b3)
{
    float fSpotPower;
    float3 f3Dummy;
}

cbuffer PBRConstant : register(b4)
{
    float3 fFresnelConstant;
    float fHeightFactor;
};

cbuffer IsPBRTextureOn : register(b5)
{
    bool bIsAOOn;
    bool bIsColorOn;
    bool bIsDiffuseOn;
    bool bIsSpecularOn;
    bool bIsMetalnessOn;
    bool bIsRoughnessOn;
    bool bIsEmissionOn;
    bool bIsNormalOn;
    bool bIsHeightOn;
    bool bIsGLTF;
    bool2 bDummys;
};

cbuffer CameraPos : register(b6)
{
    float4 f4CameraPos;
};

cbuffer LightViewProj : register(b7)
{
    matrix mLightViewProj;
    matrix mLightViewProjInvTranspose;
};

SamplerState WrapSampler : register(s0);
SamplerState ClampSampler : register(s1);
SamplerComparisonState CmopareBorderToOne : register(s2);
SamplerComparisonState CompareClampSampler : register(s3);

PBRModelPixelOutput main(PBRModelDomainOutput input)
{
    PBRModelPixelOutput result;
    
    float fRoughness;
    float fMetallic;
    if (bIsGLTF)
    {
        fRoughness = RoughnessTexture.Sample(WrapSampler, input.f2TexCoord).z;
        fMetallic = MetalnessTexture.Sample(WrapSampler, input.f2TexCoord).y;
    }
    else
    {
        fRoughness = RoughnessTexture.Sample(WrapSampler, input.f2TexCoord).x;
        fMetallic = MetalnessTexture.Sample(WrapSampler, input.f2TexCoord).x;
    }
    
    float3 f3Color = ColorTexture.SampleLevel(WrapSampler, input.f2TexCoord, 5.f * fRoughness).xyz;

    float3 f3NormalVector = GetNormalFromTexture(
        bIsNormalOn, NormalTexture, WrapSampler, input.f2TexCoord, input.f3ModelTangent, input.f3ModelBiTangent, input.f3ModelNormal
    );
    
    float3 toEyes = normalize(f4CameraPos.xyz - input.f4ModelPos.xyz);

    float NDotE = max(0.f, dot(f3NormalVector, toEyes));
    
    float3 F0 = lerp(fFresnelConstant, f3Color, fMetallic);
    float3 F = GetFresnelSchlick(NDotE, F0);
    
    float3 f3DiffuseColor = lerp(f3Color, float3(0, 0, 0), fMetallic);

    float4 f4LightProjPos = mul(input.f4ModelPos, mLightViewProj);
    f4LightProjPos /= f4LightProjPos.w;
    
    float2 f2LightTex = float2(f4LightProjPos.x, -f4LightProjPos.y);
    f2LightTex = (f2LightTex + 1.f) * 0.5f;
     
    float fSpotFactor = clamp(1.f - f4LightProjPos.x * f4LightProjPos.x - f4LightProjPos.y * f4LightProjPos.y, 0.f, 1.f);

    float3 f3ToLight = normalize(f4LightPos.xyz - input.f4ModelPos.xyz);
    float fToLightDistance = length(f4LightPos.xyz - input.f4ModelPos.xyz);
    
    float fLightPowerSaturated = fLightPower * fSpotFactor * (1 - saturate((fToLightDistance - fFallOffStart) / (fFallOffEnd - fFallOffStart)));
     
    float3 halfwayVec = normalize(toEyes + f3ToLight);
    float NDotH = max(0.f, dot(f3NormalVector, halfwayVec));
    float NDotL = max(0.f, dot(f3NormalVector, f3ToLight));
    NDotL = pow(NDotL, fSpotPower);
        
    float G = GetGMasking(NDotL, NDotE, fRoughness);
    float D = GetNDF(NDotH, fRoughness);
        
    float3 diffuseBrdf = lerp(float3(1, 1, 1) - F, float3(0, 0, 0), fMetallic) * f3Color + DiffuseTexture.Sample(WrapSampler, input.f2TexCoord).rgb;
    float3 specularBrdf = (F * D * G) / (max(1e-6, 4.0 * NDotL * NDotE)) + SpecularTexture.Sample(WrapSampler, input.f2TexCoord).rgb;

    uint uiWidth, uiHeight, uiNumLevels;
    ShadowMap.GetDimensions(0, uiWidth, uiHeight, uiNumLevels);

    float fShadowFactor = GetShadowFactorByPCF(ShadowMap, f2LightTex, CompareClampSampler, f4LightProjPos.z - 1E-6, 5.f / uiWidth);
    
    if (f4LightProjPos.z < 0.f)
    {
        fShadowFactor = 0.f;
    }
    float3 fDirectColor = (diffuseBrdf + specularBrdf) * NDotL * f3LightColor * fLightPowerSaturated * fShadowFactor;
           
    result.f4PixelColor = float4(fDirectColor, 1.f);
    result.uiModelID = uIMeshId;
    return result;
}