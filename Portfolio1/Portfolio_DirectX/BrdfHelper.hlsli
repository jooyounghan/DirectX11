#define PI 3.14159265359

float GetNDF(float NDotH, float fRoughness)
{
    float alpha2 = pow(fRoughness, 4);
    return alpha2 / (PI * pow((pow(NDotH, 2) * (alpha2 - 1) + 1), 2));
}

float3 GetFresnelSchlick(float NDotE, float3 f3SpecularReflectance)
{
    return f3SpecularReflectance + (1 - f3SpecularReflectance) * pow(2, (-5.55473 * NDotE - 6.98316) * NDotE);
}

float GetG1(float NDotOther, float fRoughness)
{
    float k = pow(fRoughness + 1, 2) / 8;
    return NDotOther / (NDotOther * (1 - k) + 1);
}

float GetGMasking(float NDotL, float NDotE, float fRoughness)
{
    return GetG1(NDotL, fRoughness) * GetG1(NDotE, fRoughness);
}