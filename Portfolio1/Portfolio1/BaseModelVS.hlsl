cbuffer ModelMatrix : register(b0)
{
    matrix mModel;
};

cbuffer ViewProjMatrix : register(b1)
{
    matrix mViewProj;
};

float4 main(float4 input : POSITION) : SV_Position
{
    input = mul(input, mModel);
    input = mul(input, mViewProj);
    return input;
}