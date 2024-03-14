#include "Common.hlsli"
#include "BrdfHelper.hlsli"
#include "MathematicalHelper.hlsli"
#include "ShadowHelper.hlsli"

Texture2D ColorTexture : register(t0);
Texture2D MetalnessTexture : register(t1);
Texture2D RoughnessTexture : register(t2);
Texture2D EmissionTexture : register(t3);
Texture2D NormalTexture : register(t4);

TextureCube ShadowMap : register(t5);

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
    float3 f3LightColor;
    float fFallOffStart;
    float fFallOffEnd;
    float fLightPower;
    float2 f2Dummy;
}

cbuffer PBRConstant : register(b3)
{
    float3 fFresnelConstant;
    float fHeightFactor;
};

cbuffer IsPBRTextureOn : register(b4)
{
    bool bIsAOOn;
    bool bIsColorOn;
    bool bIsMetalnessOn;
    bool bIsRoughnessOn;
    bool bIsEmissionOn;
    bool bIsNormalOn;
    bool bIsHeightOn;
    bool bIsGLTF;
};

cbuffer CameraPos : register(b5)
{
    float4 f4CameraPos;
};

cbuffer LightViewProj : register(b6)
{
    matrix mXLightViewProj;
    matrix mXLightViewProjInvTranspose;
};

cbuffer LightViewProj : register(b7)
{
    matrix mNXLightViewProj;
    matrix mNXLightViewProjInvTranspose;
};

cbuffer LightViewProj : register(b8)
{
    matrix mYLightViewProj;
    matrix mYLightViewProjInvTranspose;
};

cbuffer LightViewProj : register(b9)
{
    matrix mNYLightViewProj;
    matrix mNYLightViewProjInvTranspose;
};

cbuffer LightViewProj : register(b10)
{
    matrix mZLightViewProj;
    matrix mZLightViewProjInvTranspose;
};

cbuffer LightViewProj : register(b11)
{
    matrix mNZLightViewProj;
    matrix mNZLightViewProjInvTranspose;
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
        fRoughness = RoughnessTexture.Sample(WrapSampler, input.f2TexCoord).y;
        fMetallic = MetalnessTexture.Sample(WrapSampler, input.f2TexCoord).z;
    }
    else
    {
        fRoughness = RoughnessTexture.Sample(WrapSampler, input.f2TexCoord).x;
        fMetallic = MetalnessTexture.Sample(WrapSampler, input.f2TexCoord).x;
    }
    
    float3 f3Color = ColorTexture.Sample(WrapSampler, input.f2TexCoord).xyz;

    float3 f3NormalVector = GetNormalFromTexture(
        bIsNormalOn, NormalTexture, WrapSampler, input.f2TexCoord, input.f3ModelTangent, input.f3ModelBiTangent, input.f3ModelNormal
    );
    
    float3 toEyes = normalize(f4CameraPos.xyz - input.f4ModelPos.xyz);

    float NDotE = max(0.f, dot(f3NormalVector, toEyes));
    
    float3 F0 = lerp(fFresnelConstant, f3Color, fMetallic);
    float3 F = GetFresnelSchlick(NDotE, F0);
    
    float3 f3DiffuseColor = lerp(f3Color, float3(0, 0, 0), fMetallic);

    float3 f3fromLight = normalize(input.f4ModelPos.xyz - f4LightPos.xyz);

    float4 f4LightProjPos;
    float3 f3AbsFromLight = abs(f3fromLight);
    
    if (f3AbsFromLight.x > f3AbsFromLight.y && f3AbsFromLight.x > f3AbsFromLight.z)
    {
        if (f3fromLight.x > 0.f)
        {
            f4LightProjPos = mul(input.f4ModelPos, mXLightViewProj);
        }
        else
        {
            f4LightProjPos = mul(input.f4ModelPos, mNXLightViewProj);
        }
    }
    else if (f3AbsFromLight.y > f3AbsFromLight.x && f3AbsFromLight.y > f3AbsFromLight.z)
    {
        if (f3fromLight.y > 0.f)
        {
            f4LightProjPos = mul(input.f4ModelPos, mYLightViewProj);
        }
        else
        {
            f4LightProjPos = mul(input.f4ModelPos, mNYLightViewProj);
        }
    }
    else
    {
        if (f3fromLight.z > 0.f)
        {
            f4LightProjPos = mul(input.f4ModelPos, mZLightViewProj);
        }
        else
        {
            f4LightProjPos = mul(input.f4ModelPos, mNZLightViewProj);
        }
    }
    f4LightProjPos /= f4LightProjPos.w;
    

    float fToLightDistance = length(f4LightPos.xyz - input.f4ModelPos.xyz);
    
    float fLightPowerSaturated = fLightPower * (1 - saturate((fToLightDistance - fFallOffStart) / (fFallOffEnd - fFallOffStart)));
     
    float3 halfwayVec = normalize(toEyes + -f3fromLight);
    float NDotH = max(0.f, dot(f3NormalVector, halfwayVec));
    float NDotL = max(0.f, dot(f3NormalVector, -f3fromLight));
        
    float G = GetGMasking(NDotL, NDotE, fRoughness);
    float D = GetNDF(NDotH, fRoughness);
        
    float3 diffuseBrdf = (float3(1, 1, 1) - F) * f3DiffuseColor;
    float3 specularBrdf = (F * D * G) / (max(1e-6, 4.0 * NDotL * NDotE));
    
    //float fShadowFactor = ShadowMap.SampleCmpLevelZero(CompareClampSampler, f3fromLight, f4LightProjPos.z - 1E-3).x;    
    float fShadowFactor = GetShadowFactorByPCF(ShadowMap, f3fromLight, CompareClampSampler, f4LightProjPos.z - 1E-3, 1.f);
    
    float3 fDirectColor = (diffuseBrdf + specularBrdf) * NDotL * f3LightColor * fLightPowerSaturated * fShadowFactor;
           
    result.pixelColor = float4(fDirectColor, 1.f);
    result.modelID = uIMeshId;
    return result;
}