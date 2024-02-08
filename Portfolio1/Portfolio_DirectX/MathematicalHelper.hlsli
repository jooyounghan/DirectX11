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
