#include "Common.hlsli"
#include "BrdfFunction.hlsli"

SamplerState WrapSampler : register(s0);
SamplerState ClampSampler : register(s1);

Texture2D AOTexture : register(t0);
Texture2D ColorTexture : register(t1);
Texture2D MetalnessTexture : register(t2);
Texture2D RoughnessTexture : register(t3);
Texture2D EmissionTexture : register(t4);
Texture2D NormalTexture : register(t5);

TextureCube EnvSpecularTexture : register(t6);
TextureCube EnvDiffuseTexture : register(t7);
Texture2D EnvBrdfTexture : register(t8);

cbuffer ModelIDBuffer : register(b1)
{
    ModelID sModelId;
};

cbuffer TextureFlagBuffer : register(b2)
{
    bool bIsAOTexture;
    bool bIsColorTexture;
    bool bIsMetalnessTexture;
    bool bIsRoughnessTexture;
    bool bIsEmissionTexture;
    bool bIsNormalTexture;
    uint2 uiDummy;
};

cbuffer TextureConstantBuffer : register(b3)
{
    float3 fFrenelConstant;
    uint uiDummy2;
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
    
    float3 fAmbientColor = { 0.f, 0.f, 0.f };
    
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
    float3 ao = AOTexture.Sample(WrapSampler, input.f2TexCoord).xyz;

    float3 normalVec = normalize(normalSampled.xyz);
    float3 toEyes = normalize(f4CameraPos.xyz - input.f4ModelPos.xyz);

    float NDotE = max(0.f, dot(normalVec, toEyes));
    float3 F0 = lerp(fFrenelConstant, surfaceColor, metallic);
    float3 F = GetFrenelSchlick(NDotE, F0);
    float3 diffuseColor = lerp(surfaceColor, float3(0, 0, 0), metallic);
    
    // Ambient Lighting ���
    float3 diffuseSampled = EnvDiffuseTexture.Sample(ClampSampler, normalVec).xyz;
    float3 specularIBLSampled = EnvSpecularTexture.SampleLevel(ClampSampler, reflect(-toEyes, normalVec), roughness * 5.f).xyz;
    
    float2 IBLBrdf = EnvBrdfTexture.Sample(ClampSampler, float2(1.f - roughness, NDotE)).xy;
    
    float3 diffuseIBL = (float3(1.f, 1.f, 1.f) - F) * diffuseColor * diffuseSampled;
    float3 specularIBL = (F0 * IBLBrdf.x + IBLBrdf.y) * specularIBLSampled;

    fAmbientColor = (diffuseIBL + specularIBL) * ao;
    
    result.pixelColor = float4(fAmbientColor, 1.f);
    result.modleId = float4(sModelId.uiModelID.x, sModelId.uiModelID.y, sModelId.uiModelID.z, sModelId.uiModelIDStd) / sModelId.uiModelIDStd;
    return result;
}