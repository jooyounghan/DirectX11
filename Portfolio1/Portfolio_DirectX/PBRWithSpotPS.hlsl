#include "Common.hlsli"
#include "BrdfHelper.hlsli"
#include "MathematicalHelper.hlsli"

Texture2D ColorTexture : register(t0);
Texture2D MetalnessTexture : register(t1);
Texture2D RoughnessTexture : register(t2);
Texture2D EmissionTexture : register(t3);
Texture2D NormalTexture : register(t4);

Texture2D ShadowMap : register(t5);

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
    matrix mLightViewProj;
    matrix mLightViewProjInvTranspose;
};

SamplerState ClampSampler : register(s0);
SamplerState BorderToOne : register(s1);

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

    float4 f4LightProjPos = mul(input.f4ModelPos, mLightViewProj);
    f4LightProjPos /= f4LightProjPos.w;
    
    float2 f2LightTex = float2(f4LightProjPos.x, -f4LightProjPos.y);
    f2LightTex = (f2LightTex + 1.f) * 0.5f;

    float fSpotFactor = clamp(1 - f4LightProjPos.x * f4LightProjPos.x - f4LightProjPos.y * f4LightProjPos.y, 0.f, 1.f);

    float3 f3ToLight = normalize(f4LightPos.xyz - input.f4ModelPos.xyz);
    float fToLightDistance = length(f4LightPos.xyz - input.f4ModelPos.xyz);
    
    float fLightPowerSaturated = fLightPower * fSpotFactor * (1 - saturate((fToLightDistance - fFallOffStart) / (fFallOffEnd - fFallOffStart)));
     
    float3 halfwayVec = normalize(toEyes + f3ToLight);
    float NDotH = max(0.f, dot(f3NormalVector, halfwayVec));
    float NDotL = max(0.f, dot(f3NormalVector, f3ToLight));
    NDotL = pow(NDotL, fSpotPower);
        
    float G = GetGMasking(NDotL, NDotE, fRoughness);
    float D = GetNDF(NDotH, fRoughness);
        
    float3 diffuseBrdf = (float3(1, 1, 1) - F) * f3DiffuseColor;
    float3 specularBrdf = (F * D * G) / (max(1e-6, 4.0 * NDotL * NDotE));

    float fDepth = ShadowMap.Sample(BorderToOne, f2LightTex).x;    
    float3 fDirectColor = float3(0.f, 0.f, 0.f);
    
    if (fDepth < 1.f && fDepth + 1E-6 > f4LightProjPos.z)
    {
        fDirectColor = (diffuseBrdf + specularBrdf) * 1.f * NDotL * f3LightColor;
        fDirectColor = float3(1.f, 0.f, 0.f);
    }
       
    result.pixelColor = float4(fDirectColor, 1.f);
    result.modelID = uiModelId;
    return result;
}