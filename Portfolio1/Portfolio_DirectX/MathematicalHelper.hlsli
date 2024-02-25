float Logistic(float fInput, float fMaxValue, float fMinRange, float fMaxRange)
{
    float k = -18.4205f / (fMinRange - fMaxRange);
    return fMaxValue / (1 + exp(-k * (fInput - ((fMinRange + fMaxRange) / 2.f))));
}

float2x2 Get2X2InvMatrix(float2x2 input)
{
    float ad_bc = (1.0 / (input._m00 * input._m11 - input._m01 * input._m10));
    float2x2 f22Result = float2x2(input._m11, -input._m01, -input._m10, input._m00);
    return ad_bc * f22Result;
}

float3 GetNormalFromTexture(bool bIsNormalOn, Texture2D NormalTexture, SamplerState Sampler, float2 f2Tex, float3 T, float3 B, float3 N)
{
    float3 Result = N;
    if (bIsNormalOn)
    {
        float3 f3NormalVector = NormalTexture.SampleLevel(Sampler, f2Tex, 0.f).xyz;
        f3NormalVector = 2.f * f3NormalVector - 1.f;

        float3x3 TBN = float3x3(T, B, N);
        N = normalize(mul(f3NormalVector, TBN));
    }
    return N;
}

float4 GetHeightedModelPos(bool bIsHeightOn, Texture2D HeightTexture, SamplerState Sampler, float2 f2Tex, float fHeightFactor, float3 f3ModelNormal, float4 f4ModelPos)
{
    float4 Result = f4ModelPos;
    if (bIsHeightOn)
    {
        float fHeightSampled = 2.f * HeightTexture.SampleLevel(Sampler, f2Tex, 0.f).x - 1.f;
        fHeightSampled = fHeightFactor * fHeightSampled;
        Result += fHeightSampled * float4(f3ModelNormal, 0.f);
    }
    return Result;
}