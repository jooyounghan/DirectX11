Texture2D g_texture0 : register(t0);
SamplerState g_sampler : register(s0);

struct PixelShaderInput
{
    float4 posProj : SV_POSITION;
    float3 posWorld : POSITION;
    float2 texcoord : TEXCOORD;
};

struct LightConstantData
{
	int light_type;
	float3 light_color;
	float3 position;
	float3 direction;
	float light_power;
	float fall_off_start;
	float fall_off_end;
	float spot_power;
	float dummy[2];
};

cbuffer LigthBufferData : register(b0)
{
	LightConstantData	light_constant_data[50];
	int					num_lights;
	float3				dummy;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	int test1 = num_lights - 1;
	float3 test3 = dummy;
	float4 test2 = (light_constant_data[test1].light_color, 1.f);
	
	// 빛 색이 잘 들어가는지 확인 및 HLSL 디버깅 방법 확인
	 

	return g_texture0.Sample(g_sampler, input.texcoord) * test2 * test1;

}


//struct LightConstantData
//{
//    int light_type;
//    float3 light_color;
//    float3 position;
//    float3 direction;
//    float light_power;
//    float fall_off_start;
//    float fall_off_end;
//    float spot_power;
//    float dummy[2];
//};
//
//StructuredBuffer<LightConstantData> light_constant_data_buffer : register(t0);
//
//cbuffer PixelConstantBuffer : register(b0)
//{
//    float4 Color;
//    float4 AmbientColor;
//    float3 LightDirection;
//    float3 CameraPosition;
//    float3 LightColor;
//    float SpecularPower;
//    float SpecularIntensity;
//    int NumLights;
//    float3 Dummy;
//};
//
//void main()
//{
//    // Access the LightConstantData array using the light_constant_data_buffer.
//    // You can index into the buffer using the SV_PrimitiveID semantic,
//    // which contains the index of the current primitive being rendered.
//    LightConstantData light = light_constant_data_buffer[SV_PrimitiveID];
//
//    // Perform lighting calculations using the data in the constant buffer.
//    // ...
//}