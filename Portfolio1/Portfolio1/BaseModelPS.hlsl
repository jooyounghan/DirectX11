#include "Common.hlsli"
#include "BrdfFunction.hlsli"

SamplerState WrapSampler : register(s0);
SamplerState ClampSampler : register(s1);

Texture2D   AOTexture           : register(t0);
Texture2D   ColorTexture        : register(t1);
Texture2D   MetalnessTexture    : register(t2);
Texture2D   RoughnessTexture    : register(t3);
Texture2D   EmissionTexture     : register(t4);
Texture2D   NormalTexture       : register(t5);
TextureCube EnvSpecularTexture  : register(t6);
TextureCube EnvDiffuseTexture   : register(t7);
Texture2D   EnvBrdfTexture      : register(t8);


cbuffer LightSetsBuffer : register(b0)
{
    LightSet sLightSets[MAX_LIGHT_NUM];
};

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
    float3 fAmbientColor = { 0.f, 0.f, 0.f };
    
    float roughness = RoughnessTexture.Sample(ClampSampler, input.f2TexCoord).x;
    float metallic = MetalnessTexture.Sample(ClampSampler, input.f2TexCoord).x;
    float4 normalSampled;
        
    if (bIsNormalTexture)
    {
        normalSampled = GetSampledNormalFromTBN(ClampSampler, NormalTexture, input.f2TexCoord, input.f4ModelNormal, input.f4ModelTangent, input.f4ModelBiTangent);
    }
    else
    {
        normalSampled = input.f4ModelNormal;
    }
    
    
    float3 surfaceColor = ColorTexture.Sample(ClampSampler, input.f2TexCoord).xyz;
    float3 ao = AOTexture.Sample(ClampSampler, input.f2TexCoord).xyz;

    float3 normalVec = normalize(normalSampled.xyz);
    float3 toEyes = normalize(f4CameraPos.xyz - input.f4ModelPos.xyz);

    float NDotE = max(0.f, dot(normalVec, toEyes));
    float3 F0 = lerp(fFrenelConstant, surfaceColor, metallic);
    float3 F = GetFrenelSchlick(NDotE, F0);
    float3 diffuseColor = lerp(surfaceColor, float3(0, 0, 0), metallic);
    
    // Ambient Lighting 계산
    float3 diffuseSampled = EnvDiffuseTexture.Sample(ClampSampler, normalVec).xyz;
    float3 specularIBLSampled = EnvSpecularTexture.SampleLevel(ClampSampler, reflect(-toEyes, normalVec), roughness * 5.f).xyz;
    
    float2 IBLBrdf = EnvBrdfTexture.Sample(ClampSampler, float2(1.f - roughness, NDotE)).xy;
    
    float3 diffuseIBL = (float3(1.f, 1.f, 1.f) - F) * diffuseColor * diffuseSampled;
    float3 specularIBL = (F0 * IBLBrdf.x + IBLBrdf.y) * specularIBLSampled;

    fAmbientColor = (diffuseIBL + specularIBL) * ao;
    
    // Direct Lighting 계산
    [unroll]
    for (int i = 0; i < MAX_LIGHT_NUM; ++i)
    {        
        float3 toLight = toLight = normalize(-sLightSets[i].f4Direction);
        float fLightPower = sLightSets[i].fLightStrength;
        float toLightDistance;
        
        if (sLightSets[i].LightType != DIRECT_LIGHT)
        {
            toLight = normalize(sLightSets[i].f4Location.xyz - input.f4ModelPos.xyz);
            toLightDistance = length(sLightSets[i].f4Location.xyz - input.f4ModelPos.xyz);
            fLightPower = fLightPower * (1 - saturate((toLightDistance - sLightSets[i].fFallOffStart) / (sLightSets[i].fFallOffEnd - sLightSets[i].fFallOffStart)));
        }
     
        float3 halfwayVec = normalize(toEyes + toLight);
    
        float NDotH = max(0.f, dot(normalVec, halfwayVec));
        float NDotL = max(0.f, dot(normalVec, toLight));
        
        if (sLightSets[i].LightType == SPOT_LIGHT)
        {
            NDotL = pow(NDotL, sLightSets[i].fSpotPower);
        }
        
        float G = GetGMasking(NDotL, NDotE, roughness);
        float D = GetNDF(NDotH, roughness);
        
        float3 diffuseBrdf = (float3(1, 1, 1) - F) * diffuseColor;
        float3 specularBrdf = (F * D * G) / (max(1e-6, 4.0 * NDotL * NDotE));
        
        fDirectColor += (diffuseBrdf + specularBrdf) * fLightPower * NDotL * sLightSets[i].f4Color.xyz;
    }
    
       
    result.pixelColor = float4(fDirectColor + fAmbientColor, 1.f);
    result.modleId = float4(sModelId.uiModelID.x, sModelId.uiModelID.y, sModelId.uiModelID.z, sModelId.uiModelIDStd) / sModelId.uiModelIDStd;
    return result;
}