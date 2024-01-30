#include "ModelManipulator.h"
#include "AStaticMesh.h"
#include "APBRStaticMesh.h"
#include "NormalImageFile.h"

using namespace std;
using namespace ImGui;

ModelManipulator::ModelManipulator(AStaticMesh** ppSelectedStaticMeshIn)
	: ppSelectedStaticMesh(ppSelectedStaticMeshIn)
{
}

ModelManipulator::~ModelManipulator()
{
}

void ModelManipulator::PopAsDialog()
{
	Begin("Model Manipulator");

	ImGui::Text("Average %.3f ms/frame (%.1f FPS)",
		1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);

	if (*ppSelectedStaticMesh != nullptr)
	{
		(*ppSelectedStaticMesh)->AcceptModelManipulator(this);
	}
	End();
}

void ModelManipulator::VisitModel(AStaticMesh& staticMesh)
{
	DrawTransformation(&staticMesh);
}

void ModelManipulator::VisitModel(APBRStaticMesh& pbrStaticMesh)
{
	DrawTransformation((AStaticMesh*)(&pbrStaticMesh));
	Separator();
	DrawModelTexture(&pbrStaticMesh);
}

void ModelManipulator::DrawTransformation(AStaticMesh* pStaticMesh)
{
	if (CollapsingHeader("Transformation", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
	{
		Separator();
		DragFloat3("Translation", pStaticMesh->xmvPosition.m128_f32, 1.f, -1000000.f, 1000000.f, "%.2f");
		DragFloat3("Rotaion", &pStaticMesh->sAnglesDegree.fPitchDeg, 1.f, 0.f, 360.f, "%.2f");
		DragFloat3("Scale", &pStaticMesh->sScales.fScaleX, 0.005f, -1000000.f, 1000000.f, "%.2f");
	}
}

void ModelManipulator::DrawModelTexture(APBRStaticMesh* pPBRStaticMesh)
{
	if (CollapsingHeader("PBR Model Textures"))
	{
		DragFloat3("Frenel Reflectance", pPBRStaticMesh->sPBRConstant.fFrenelConstant, 0.005f, 0.f, 1.f, "%.3f");
		DragFloat("Height Factor", &pPBRStaticMesh->sPBRConstant.fHeightFactor, 0.005f, 0.f, 1.f, "%.3f");

		for (WORD idx = 0; idx < APBRStaticMesh::TEXTURE_MAP_NUM; ++idx)
		{
			Separator();
			Text(APBRStaticMesh::unmapTextureNames[idx].c_str());
			if (pPBRStaticMesh->pModelTexture[idx] != nullptr)
			{
				Image(pPBRStaticMesh->pModelTexture[idx]->cpSRV.Get(), ImVec2(60.f, 60.f));
			}
			else
			{
				Image(nullptr, ImVec2(60.f, 60.f));
			}

			if (BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Texture2D"))
				{
					pPBRStaticMesh->pModelTexture[idx] = *(shared_ptr<NormalImageFile>*)payload->Data;
				}
				ImGui::EndDragDropTarget();
			}

			if (pPBRStaticMesh->pModelTexture[idx] != nullptr)
			{
				SameLine();
				Text(pPBRStaticMesh->pModelTexture[idx]->strFileName.c_str());
			}
			else
			{
				SameLine();
				Text("");
			}
		}
	}
}

