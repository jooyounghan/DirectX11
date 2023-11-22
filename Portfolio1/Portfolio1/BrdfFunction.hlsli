#define PI 3.141592

float GetNDF(float NDotH, float roughness)
{
    float alpha2 = pow(roughness, 4);
    return alpha2 / (PI * pow((pow(NDotH, 2) * (alpha2 - 1) + 1), 2));
}

float3 GetFrenelSchlick(float EDotH, float3 SpecularReflectance)
{
    return SpecularReflectance + (1 - SpecularReflectance) * pow(2, (-5.55473 * EDotH - 6.98316) * EDotH);
}

float GetG1(float NDotOther, float roughness)
{
    float k = pow(roughness + 1, 2) / 8;
    return NDotOther / (NDotOther * (1 - k) + 1);
}

float GetGMasking(float NDotL, float NDotE, float roughness)
{
    return GetG1(NDotL, roughness) * GetG1(NDotE, roughness);
}