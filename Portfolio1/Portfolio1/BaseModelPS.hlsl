#include "Common.hlsli"

SamplerState Sampler : register(s0);

Texture2D AOTexture : register(t0);
Texture2D DiffuseTexture : register(t1);
Texture2D ReflectTexture : register(t2);
Texture2D NormalTexture : register(t3);

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
    bool bIsAOTexture;
    bool bIsDiffuseTexture;
    bool bIsReflectTexture;
    bool bIsNormalTexture;
    int3 iDummy;
};

PixelOutput main(DomainOutput input) : SV_TARGET
{
    PixelOutput result;
    
    float4 fResultColor = { 0.f, 0.f, 0.f, 0.f };
    float4 fDiffuseColor = DiffuseTexture.SampleLevel(Sampler, input.f2TexCoord, 0.f);
    
    float4 fNormalSampled;
    if (bIsNormalTexture)
    {
        fNormalSampled = GetSampledNormalFromTBN(Sampler, NormalTexture, input.f2TexCoord, input.f4ModelNormal, input.f4ModelTangent, input.f4ModelBiTangent);
    }
    else
    {
        fNormalSampled = input.f4ModelNormal;
    }


    [unroll]
    for (int i = 0; i < MAX_LIGHT_NUM; ++i)
    {
        float4 tolight = normalize(sLightSets[i].f4Location - input.f4ModelPos);
        float fLightPower = clamp(dot(tolight, fNormalSampled), 0.f, 1.f);
        fResultColor += fDiffuseColor * fLightPower * sLightSets[i].f4Color;
    }
    
    result.pixelColor = fResultColor + float4(0.3f, 0.3f, 0.3f, 0.3f);
    result.modleId = float4(sModelId.uiModelID.x, sModelId.uiModelID.y, sModelId.uiModelID.z, sModelId.uiModelIDStd) / sModelId.uiModelIDStd;
    return result;
}