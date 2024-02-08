#include "Common.hlsli"
#include "BrdfHelper.hlsli"

TextureCube EnvSpecularTexture : register(t0);
TextureCube EnvDiffuseTexture : register(t1);
Texture2D EnvBrdfTexture : register(t2);

Texture2D AOTexture : register(t3);
Texture2D ColorTexture : register(t4);
Texture2D MetalnessTexture : register(t5);
Texture2D RoughnessTexture : register(t6);
Texture2D EmissionTexture : register(t7);

cbuffer ModelID : register(b0)
{
    uint uiModelId;
    uint uiDummy[3];
};

cbuffer CameraPos : register(b1)
{
    float4 f4CameraPos;
};

cbuffer PBRConstant : register(b2)
{
    float3 fFresnelConstant;
    float fHeightFactor;
};

cbuffer IsPBRTextureOn : register(b3)
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

SamplerState ClampSampler : register(s0);

PixelOutput main(DomainOutput input)
{
    PixelOutput result;
        
    float fRoughness = RoughnessTexture.Sample(ClampSampler, input.f2TexCoord).x;
    float fMetallic = MetalnessTexture.Sample(ClampSampler, input.f2TexCoord).x;   
    float3 f3Color = ColorTexture.Sample(ClampSampler, input.f2TexCoord).xyz;
    float3 f3AmbientOcclusion = AOTexture.Sample(ClampSampler, input.f2TexCoord).xyz;

    float3 toEyes = normalize(f4CameraPos.xyz - input.f4ModelPos.xyz);

    float NDotE = max(0.f, dot(input.f4ModelNormal.xyz, toEyes));

    float3 F0 = lerp(fFresnelConstant, f3Color, fMetallic);
    float3 F = GetFresnelSchlick(NDotE, F0);
    
    float3 f3DiffuseColor = lerp(f3Color, float3(0, 0, 0), fMetallic);

    float3 f3DiffuseSampled = EnvDiffuseTexture.Sample(ClampSampler, input.f4ModelNormal.xyz).xyz;
    float3 f3SpecularSampled = EnvSpecularTexture.SampleLevel(ClampSampler, reflect(-toEyes, input.f4ModelNormal.xyz), fRoughness * 5.f).xyz;
    
    float2 IBLBrdf = EnvBrdfTexture.Sample(ClampSampler, float2(1.f - fRoughness, NDotE)).xy;
    
    float3 diffuseIBL = (float3(1.f, 1.f, 1.f) - F) * f3DiffuseColor * f3DiffuseSampled;
    float3 specularIBL = (F0 * IBLBrdf.x + IBLBrdf.y) * f3SpecularSampled;

    float3 fAmbientColor = (diffuseIBL + specularIBL) * f3AmbientOcclusion;
 
    result.pixelColor = float4(fAmbientColor, 1.f);
    result.modelID = uiModelId;
    return result;
}