#include "Common.hlsli"

#define NUM_CONTROL_POINTS 3

HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<HullInput, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

    Output.EdgeTessFactor[0] = 64;
    Output.EdgeTessFactor[1] = 64;
    Output.EdgeTessFactor[2] = 64;
    Output.InsideTessFactor = 64;

	return Output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
DomainInput main(
	InputPatch<HullInput, NUM_CONTROL_POINTS> ip,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID )
{
    DomainInput Output;

    Output.f4ProjPos = ip[i].f4ProjPos;
    Output.f4ModelPos = ip[i].f4ModelPos;
    Output.f2TexCoord = ip[i].f2TexCoord;
    Output.f4ModelNormal = ip[i].f4ModelNormal;
    Output.f4ModelTangent = ip[i].f4ModelTangent;
	return Output;
}
