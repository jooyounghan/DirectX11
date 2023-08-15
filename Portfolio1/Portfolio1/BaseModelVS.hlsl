cbuffer MeshConstants : register(b0)
{
    matrix ViewProj;
};

float4 main(float4 input : POSITION) : SV_Position
{
    input = mul(ViewProj, input);
    return input;
}