#include "Common.hlsli"

cbuffer CameraPos : register(b0)
{
    float4 f4CameraPos;
};

HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<PBRModelVertexOutput, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;
	
    float3 fAveragePos = (ip[0].f4ModelPos.xyz + ip[1].f4ModelPos.xyz + ip[2].f4ModelPos.xyz) / 3.f;
    float distance = length(fAveragePos - f4CameraPos.xyz);
    float fact = max(3 * exp(-(distance / 100.f)), 1.f);
	
    fact = 1.f;
	
    Output.EdgeTessFactor[0] = fact;
	Output.EdgeTessFactor[1] = fact;
	Output.EdgeTessFactor[2] = fact;
	Output.InsideTessFactor = fact;

	return Output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
PBRModelHullOutput main(
	InputPatch<PBRModelVertexOutput, NUM_CONTROL_POINTS> ip,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
    PBRModelHullOutput Output;
    Output.f4ProjPos = ip[i].f4ProjPos;
    Output.f4ModelPos = ip[i].f4ModelPos;
    Output.f2TexCoord = ip[i].f2TexCoord;
    Output.f3ModelNormal = ip[i].f3ModelNormal;
    Output.f3ModelTangent = ip[i].f3ModelTangent;
	return Output;
}
