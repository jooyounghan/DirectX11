#include "Common.hlsli"
#include "BrdfFunction.hlsli"

SamplerState WrapSampler : register(s0);
SamplerState ClampSampler : register(s1);

Texture2D   ColorTexture        : register(t1);
Texture2D   MetalnessTexture    : register(t2);
Texture2D   RoughnessTexture    : register(t3);
Texture2D   EmissionTexture     : register(t4);
Texture2D   NormalTexture       : register(t5);

Texture2D XShadowMap            : register(t12);
Texture2D NegativeXShadowMap    : register(t13);
Texture2D YShadowMap            : register(t14);
Texture2D NegativeYShadowMap    : register(t15);
Texture2D ZShadowMap            : register(t16);
Texture2D NegativeZShadowMap    : register(t17);

cbuffer BaseLight : register(b0)
{
    float4 f4LightPos;
    float4 f4LightColor;
    float fFallOffStart;
    float fFallOffEnd;
    float fLightPower;
    float fDummy;
}

cbuffer ModelIDBuffer : register(b1)
{
    ModelID sModelId;
};

cbuffer TextureFlagBuffer : register(b2)
{
    bool    bIsAOTexture;
    bool    bIsColorTexture;
    bool    bIsMetalnessTexture;
    bool    bIsRoughnessTexture;
    bool    bIsEmissionTexture;
    bool    bIsNormalTexture;
    uint2   uiDummy;
};

cbuffer TextureConstantBuffer : register(b3)
{
    float3  fFrenelConstant;
    uint    uiDummy2;
};

cbuffer CameraInfo : register(b6)
{
    float4 f4CameraPos;
    matrix mViewProj;
    matrix mViewProjInv;
};

PixelOutput main(DomainOutput input)
{  
    PixelOutput result;
    
    float3 fDirectColor = { 0.f, 0.f, 0.f };
    
    float roughness = RoughnessTexture.Sample(WrapSampler, input.f2TexCoord).x;
    float metallic = MetalnessTexture.Sample(WrapSampler, input.f2TexCoord).x;
    float4 normalSampled;
        
    if (bIsNormalTexture)
    {
        normalSampled = GetSampledNormalFromTBN(WrapSampler, NormalTexture, input.f2TexCoord, input.f4ModelNormal, input.f4ModelTangent, input.f4ModelBiTangent);
    }
    else
    {
        normalSampled = input.f4ModelNormal;
    }
    
    
    float3 surfaceColor = ColorTexture.Sample(WrapSampler, input.f2TexCoord).xyz;

    float3 normalVec = normalize(normalSampled.xyz);
    float3 toEyes = normalize(f4CameraPos.xyz - input.f4ModelPos.xyz);

    float NDotE = max(0.f, dot(normalVec, toEyes));
    float3 F0 = lerp(fFrenelConstant, surfaceColor, metallic);
    float3 F = GetFrenelSchlick(NDotE, F0);
    float3 diffuseColor = lerp(surfaceColor, float3(0, 0, 0), metallic);
    
    float3 toLight = normalize(f4LightPos.xyz - input.f4ModelPos.xyz);
    float toLightDistance = length(f4LightPos.xyz - input.f4ModelPos.xyz);
    float fLightPowerSaturated = fLightPower * (1 - saturate((toLightDistance - fFallOffStart) / (fFallOffEnd - fFallOffStart)));
     
    float3 halfwayVec = normalize(toEyes + toLight);
    
    float NDotH = max(0.f, dot(normalVec, halfwayVec));
    float NDotL = max(0.f, dot(normalVec, toLight));
               
    float G = GetGMasking(NDotL, NDotE, roughness);
    float D = GetNDF(NDotH, roughness);
        
    float3 diffuseBrdf = (float3(1, 1, 1) - F) * diffuseColor;
    float3 specularBrdf = (F * D * G) / (max(1e-6, 4.0 * NDotL * NDotE));
        
    //fDirectColor += (diffuseBrdf + specularBrdf) * fLightPowerSaturated * NDotL * f4LightColor.xyz;
    fDirectColor = float4(1.f, 0.f, 0.f, 1.f);
    result.pixelColor = float4(fDirectColor, 1.f);
    result.modleId = float4(sModelId.uiModelID.x, sModelId.uiModelID.y, sModelId.uiModelID.z, sModelId.uiModelIDStd) / sModelId.uiModelIDStd;
    return result;
}