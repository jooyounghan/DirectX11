#include "Common.hlsli"

#define NUM_CONTROL_POINTS 3

HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VertexOutput, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

    Output.EdgeTessFactor[0] = 8;
    Output.EdgeTessFactor[1] = 8;
    Output.EdgeTessFactor[2] = 8;
    Output.InsideTessFactor = 8;

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
    Output.f4ProjNormal = ip[i].f4ProjNormal;
	return Output;
}
