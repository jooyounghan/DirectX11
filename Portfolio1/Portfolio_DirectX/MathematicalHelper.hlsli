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

float2x3 GetTBMatrix(float4 f4ModelPos0, float4 f4ModelPos1, float4 f4ModelPos2, float2 f2Tex0, float2 f2Tex1, float2 f2Tex2)
{
    float3 e1 = normalize((f4ModelPos1 - f4ModelPos0).xyz);
    float3 e2 = normalize((f4ModelPos2 - f4ModelPos0).xyz);
    
    float2 dtexXY1 = f2Tex1 - f2Tex0;
    float dtexXY1Lenght = length(dtexXY1);
    float2 dtexXY2 = f2Tex2 - f2Tex0;
    float dtexXY2Lenght = length(dtexXY2);
    
    float2x2 dTexXY = float2x2(dtexXY1, dtexXY2);
    float2x2 dTexXYInv = Get2X2InvMatrix(dTexXY);
    float2x3 e = float2x3(dtexXY1Lenght * e1, dtexXY2Lenght * e2);
    
    return mul(dTexXYInv, e);
}

float3 GetNormalFromTexture(Texture2D NormalTexture, SamplerState Sampler, float2 f2Tex, float3 T, float3 B, float3 N)
{
    float3 f3NormalVector = NormalTexture.SampleLevel(Sampler, f2Tex, 0.f).xyz;
    f3NormalVector = 2.f * f3NormalVector - 1.f;

    float3x3 TBN = float3x3(T, B, N);
    return normalize(mul(f3NormalVector, TBN));
}

float3 GetNormalFromTexture(Texture2D NormalTexture, SamplerState Sampler, float2 f2Tex, float4 T, float4 B, float4 N)
{
    return GetNormalFromTexture(NormalTexture, Sampler, f2Tex, T.xyz, B.xyz, N.xyz);
}

