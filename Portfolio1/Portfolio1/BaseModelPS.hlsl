#include "Common.hlsli"

SamplerState Sampler : register(s0);

Texture2D AOTexture : register(t0);
Texture2D ColorTexture : register(t1);
Texture2D MetalnessTexture : register(t2);
Texture2D RoughnessTexture : register(t3);
Texture2D NormalTexture : register(t4);

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
    bool    bIsNormalTexture;
    bool    bIsHeightTexture;
    uint2   uiDummy;
};

PixelOutput main(DomainOutput input)
{
    PixelOutput result;
    
    float4 fResultColor = { 0.f, 0.f, 0.f, 0.f };
    float4 fColor = { 0.f, 0.f, 0.f, 0.f };
    
    if (bIsColorTexture)
    {
        fColor = ColorTexture.Sample(Sampler, input.f2TexCoord);
    }
    else
    {
    }
    
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
        float test = dot(tolight, fNormalSampled);
        float fLightPower = clamp(test, 0.f, 1.f);
        fResultColor += fColor * fLightPower * sLightSets[i].f4Color;
    }
    
    result.pixelColor = fResultColor;
    result.modleId = float4(sModelId.uiModelID.x, sModelId.uiModelID.y, sModelId.uiModelID.z, sModelId.uiModelIDStd) / sModelId.uiModelIDStd;
    return result;
}