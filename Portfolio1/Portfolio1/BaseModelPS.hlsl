float4 main(float4 input : SV_Position) : SV_TARGET
{
    return float4(input.r / 1000.f, input.g / 1000.f, 1.f, 1.f);
}
