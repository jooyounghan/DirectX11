#include "Common.hlsli"
#include "BrdfHelper.hlsli"
#include "MathematicalHelper.hlsli"

Texture2D ColorTexture : register(t0);
Texture2D MetalnessTexture : register(t1);
Texture2D RoughnessTexture : register(t2);
Texture2D EmissionTexture : register(t3);
Texture2D NormalTexture : register(t4);

TextureCube ShadowMap : register(t5);

cbuffer ModelIDBuffer : register(b0)
{
    uint uiModelId;
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

cbuffer PBRConstant : register(b4)
{
    float3 fFresnelConstant;
    float fHeightFactor;
};

cbuffer IsPBRTextureOn : register(b5)
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

cbuffer CameraPos : register(b6)
{
    float4 f4CameraPos;
};

cbuffer LightViewProj : register(b7)
{
    matrix mXLightViewProj;
    matrix mXLightViewProjInvTranspose;
};

cbuffer LightViewProj : register(b8)
{
    matrix mNXLightViewProj;
    matrix mNXLightViewProjInvTranspose;
};

cbuffer LightViewProj : register(b9)
{
    matrix mYLightViewProj;
    matrix mYLightViewProjInvTranspose;
};

cbuffer LightViewProj : register(b10)
{
    matrix mNYLightViewProj;
    matrix mNYLightViewProjInvTranspose;
};

cbuffer LightViewProj : register(b11)
{
    matrix mZLightViewProj;
    matrix mZLightViewProjInvTranspose;
};

cbuffer LightViewProj : register(b12)
{
    matrix mNZLightViewProj;
    matrix mNZLightViewProjInvTranspose;
};


SamplerState ClampSampler : register(s0);
SamplerComparisonState CmopareBorderToOne : register(s1);
SamplerComparisonState CompareClampSampler : register(s2);

PixelOutput main(DomainOutput input)
{
    PixelOutput result;
    
    float fRoughness = RoughnessTexture.Sample(ClampSampler, input.f2TexCoord).x;
    float fMetallic = MetalnessTexture.Sample(ClampSampler, input.f2TexCoord).x;
    float3 f3Color = ColorTexture.Sample(ClampSampler, input.f2TexCoord).xyz;

    float3 f3NormalVector = input.f4ModelNormal.xyz;
    if (bIsNormalOn)
    {
        f3NormalVector = GetNormalFromTexture(NormalTexture, ClampSampler, input.f2TexCoord, input.f4ModelTangent, input.f4ModelBiTangent, input.f4ModelNormal);
    }
    
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

    float fDepthFactor = ShadowMap.SampleCmpLevelZero(CompareClampSampler, f3fromLight, f4LightProjPos.z - 1E-6).x;
    float3 fDirectColor = (diffuseBrdf + specularBrdf) * NDotL * f3LightColor * fLightPowerSaturated * fDepthFactor;
           
    result.pixelColor = float4(fDirectColor, 1.f);
    result.modelID = uiModelId;
    return result;
}