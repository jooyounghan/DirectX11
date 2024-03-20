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
    
    result.pixelColor = float4(fAmbientColor, 1.f);
    result.modelID = uIMeshId;
    return result;
}


//// HLSL Pixel Shader

//Texture2D DiffuseTexture : register(t0); // 디퓨즈 텍스쳐
//Texture2D SpecularTexture : register(t1); // 스페큘러 텍스쳐
//SamplerState Sampler : register(s0); // 텍스쳐 샘플러

//// 조명 및 카메라 정보
//struct Light
//{
//    float3 Position;
//    float3 Intensity;
//};

//cbuffer LightBuffer : register(b0)
//{
//    Light light;
//    float3 camPosition;
//};

//// 정점 셰이더에서 전달된 값
//struct PixelInputType
//{
//    float4 position : SV_POSITION;
//    float2 tex : TEXCOORD0;
//    float3 normal : NORMAL;
//};

//// 조명 계산 함수
//float4 CalculateLighting(float3 normal, float3 lightDirection, float3 viewDirection, float2 texCoords)
//{
//    // 디퓨즈 반사 계산
//    float diffuseFactor = max(dot(normal, lightDirection), 0.0);
//    float4 diffuseColor = diffuseFactor * DiffuseTexture.Sample(Sampler, texCoords);

//    // 스페큘러 반사 계산
//    float3 halfwayDir = normalize(lightDirection + viewDirection);
//    float specularFactor = pow(max(dot(normal, halfwayDir), 0.0), 16.0); // 16은 스페큘러 하이라이트의 강도를 조절하는 shininess 값
//    float4 specularColor = specularFactor * SpecularTexture.Sample(Sampler, texCoords);

//    // 조명 적용
//    float3 lighting = diffuseColor.rgb * light.Intensity + specularColor.rgb * light.Intensity;
//    return float4(lighting, 1.0);
//}

//// 픽셀 셰이더 메인 함수
//float4 MainPS(PixelInputType input) : SV_TARGET
//{
//    float3 normal = normalize(input.normal);
//    float3 lightDir = normalize(light.Position - input.position.xyz);
//    float3 viewDir = normalize(camPosition - input.position.xyz);

//    float4 color = CalculateLighting(normal, lightDir, viewDir, input.tex);
//    return color;
//}

//technique10 Tech
//{
//    pass Pass0
//    {
//        SetVertexShader(CompileShader(vs_4_0, VS()));
//        SetGeometryShader(NULL);
//        SetPixelShader(CompileShader(ps_4_0, MainPS()));
//    }
//}