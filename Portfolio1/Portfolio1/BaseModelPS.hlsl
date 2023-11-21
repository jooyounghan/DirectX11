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
    
    float4 fResultColor = { 0.f, 0.f, 0.f, 0.f };
    float4 fColor = { 0.f, 0.f, 0.f, 0.f };
    
    float4 fNormalSampled;
    if (bIsNormalTexture)
    {
        fNormalSampled = GetSampledNormalFromTBN(ClampSampler, NormalTexture, input.f2TexCoord, input.f4ModelNormal, input.f4ModelTangent, input.f4ModelBiTangent);
    }
    else
    {
        fNormalSampled = input.f4ModelNormal;
    }
    
    if (bIsColorTexture)
    {
        fColor = ColorTexture.Sample(ClampSampler, input.f2TexCoord);
    }
    else
    {
    }

    float3 toEyes = f4CameraPos.xyz - input.f4ModelPos.xyz;    
    float roughness = RoughnessTexture.Sample(ClampSampler, input.f2TexCoord).x;
    float metallic = MetalnessTexture.Sample(ClampSampler, input.f2TexCoord).x;
   
    
    // 이미지 기반 조명에 대한 계산

    
    
    
    
    
    
    
    float3 diffuseBrdf = lerp(fFrenelConstant, fColor.xyz, metallic) / 3.141592;    
    // 조명에 대한 계산
    
    [unroll]
    for (int i = 0; i < MAX_LIGHT_NUM; ++i)
    {
        float3 toLight = sLightSets[i].f4Location.xyz - input.f4ModelPos.xyz;

        float3 specularBrdf = float3();





        
        float fLightPower = clamp(test, 0.f, 1.f);
        fResultColor += fColor * fLightPower * sLightSets[i].f4Color;
    }
    
    
    
    
    
    
    
    result.pixelColor = fResultColor;
    result.modleId = float4(sModelId.uiModelID.x, sModelId.uiModelID.y, sModelId.uiModelID.z, sModelId.uiModelIDStd) / sModelId.uiModelIDStd;
    return result;
}