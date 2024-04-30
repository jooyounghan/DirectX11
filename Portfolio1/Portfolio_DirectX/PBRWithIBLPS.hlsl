#include "Common.hlsli"
#include "BrdfHelper.hlsli"
#include "MathematicalHelper.hlsli"

TextureCube EnvSpecularTexture : register(t0);
TextureCube EnvDiffuseTexture : register(t1);
Texture2D EnvBrdfTexture : register(t2);

Texture2D AOTexture : register(t3);
Texture2D ColorTexture : register(t4);
Texture2D DiffuseTexture : register(t5);
Texture2D SpecularTexture : register(t6);
Texture2D MetalnessTexture : register(t7);
Texture2D RoughnessTexture : register(t8);
Texture2D EmissionTexture : register(t9);
Texture2D NormalTexture : register(t10);

cbuffer ModelID : register(b0)
{
    uint uIMeshId;
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

SamplerState WrapSampler : register(s0);
SamplerState ClampSampler : register(s1);

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

    float f3AmbientOcclusion = 1.f;
    
    if (bIsAOOn)
    {
        f3AmbientOcclusion = AOTexture.Sample(WrapSampler, input.f2TexCoord).r;
    }

    float3 f3NormalVector = GetNormalFromTexture(
        bIsNormalOn, NormalTexture, WrapSampler, input.f2TexCoord, input.f3ModelTangent, input.f3ModelBiTangent, input.f3ModelNormal
    );
    
    float3 toEyes = normalize(f4CameraPos.xyz - input.f4ModelPos.xyz);

    float NDotE = max(0.f, dot(f3NormalVector, toEyes));

    float3 F0 = lerp(fFresnelConstant, f3Color, fMetallic);
    float3 F = GetFresnelSchlick(NDotE, F0);
    
    float3 kd = lerp(float3(1.f, 1.f, 1.f) - F, float3(0, 0, 0), fMetallic);
    float3 f3DiffuseSampled = EnvDiffuseTexture.Sample(WrapSampler, f3NormalVector).xyz;
    float3 f3SpecularSampled = EnvSpecularTexture.SampleLevel(WrapSampler, reflect(-toEyes, f3NormalVector), fRoughness * 5.f).xyz;
    
    float2 IBLBrdf = EnvBrdfTexture.Sample(ClampSampler, float2(1.f - fRoughness, NDotE)).xy;
    
    float3 diffuseIBL = kd * f3Color;
    diffuseIBL += DiffuseTexture.Sample(WrapSampler, input.f2TexCoord).rgb;
    diffuseIBL *= f3DiffuseSampled;
    
    float3 specularIBL = (F0 * IBLBrdf.x + IBLBrdf.y);
    specularIBL += SpecularTexture.Sample(WrapSampler, input.f2TexCoord).rgb;
    specularIBL *= f3SpecularSampled;
    
    float3 fAmbientColor = (diffuseIBL + specularIBL) * f3AmbientOcclusion * 0.25f;
    
    fAmbientColor += EmissionTexture.Sample(WrapSampler, input.f2TexCoord).rgb;
    
    result.f4PixelColor = float4(fAmbientColor, 1.f);
    result.uiModelID = uIMeshId;
    return result;
}
