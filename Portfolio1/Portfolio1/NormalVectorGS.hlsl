#include "Common.hlsli"
#include "NormalVectorShaderType.hlsli"

[maxvertexcount(2)]
void main(
	point NormalVectorVSOutput input[1],
	inout LineStream<NormalVectorGSOutput> output
)
{
	for (uint i = 0; i < 2; i++)
	{
        NormalVectorGSOutput element;
        element.f4ProjNormal = input[0].f4ProjNormal;
        element.f2TexCoord = input[0].f2TexCoord;
        element.f2TexCoord.x = (float1) i;
        element.f4ProjPos = input[0].f4ProjPos + element.f4ProjNormal * i;
		output.Append(element);
	}
}