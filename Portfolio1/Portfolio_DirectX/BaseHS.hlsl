#include "Common.hlsli"

cbuffer CameraPos : register(b0)
{
    float4 f4CameraPos;
};

HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VertexOutput, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;
	
    float3 fAveragePos = (ip[0].f4ModelPos.xyz + ip[1].f4ModelPos.xyz + ip[2].f4ModelPos.xyz) / 3.f;
    float distance = length(fAveragePos - f4CameraPos.xyz);
    float fact = max(3 * exp(-(distance / 5.f)), 1.f);
	
	Output.EdgeTessFactor[0] = 
		Output.EdgeTessFactor[1] = 
		Output.EdgeTessFactor[2] = 
		Output.InsideTessFactor = fact;

	return Output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HullOutput main(
	InputPatch<VertexOutput, NUM_CONTROL_POINTS> ip, 
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
    HullOutput Output;
    Output.f4ProjPos = ip[i].f4ProjPos;
    Output.f4ModelPos = ip[i].f4ModelPos;
    Output.f2TexCoord = ip[i].f2TexCoord;
    Output.f4ModelNormal = ip[i].f4ModelNormal;
	return Output;
}
