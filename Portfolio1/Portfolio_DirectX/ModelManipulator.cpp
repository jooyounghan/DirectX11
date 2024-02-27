#include "ModelManipulator.h"

#include "APBRStaticMesh.h"
#include "NormalImageFile.h"

#include "AIBLModel.h"
#include "DDSImageFile.h"

#include "CubeModel.h"
#include "CubeMapModel.h"


using namespace std;
using namespace ImGui;

ModelManipulator::ModelManipulator()
	: bIsDrawingNormal(false)
{
	AddModel(make_shared<CubeModel>(-5.f, 0.f, 0.f, 1.f, false, 8));
	AddModel(make_shared<CubeModel>(5.f, 0.f, 0.f, 1.f, false, 8));
	AddModel(make_shared<CubeModel>(0.f, -5.f, 0.f, 1.f, false, 8));
	AddModel(make_shared<CubeModel>(0.f, 5.f, 0.f, 1.f, false, 8));
	AddModel(make_shared<CubeModel>(0.f, 0.f, 5.f, 1.f, false, 8));
	AddModel(make_shared<CubeModel>(0.f, 0.f, -5.f, 1.f, false, 8));
	AddModel(make_shared<CubeModel>(0.f, 0.f, -5.f, 1.f, false, 8));

	spIBLModel = make_shared<CubeMapModel>(500.f, 15);
	AddModel(spIBLModel);
}

ModelManipulator::~ModelManipulator()
{
}

void ModelManipulator::SetSelctedMesh(const uint32_t selected_id)
{
	if (pModels.find(selected_id) != pModels.end())
	{
		spSelectedMesh = pModels[selected_id];
	}
	else
	{
		spSelectedMesh = nullptr;
	}
};

void ModelManipulator::PopAsDialog()
{
	Begin("Model Manipulator");

	ImGui::Text("Average %.3f ms/frame (%.1f FPS)",
		1000.0f / ImGui::GetIO().Framerate,
		ImGui::GetIO().Framerate);

	Checkbox("Drawing Normal Vector", &bIsDrawingNormal);

	ListUpModel();

	if (spSelectedMesh != nullptr)
	{
		spSelectedMesh->AcceptModelManipulating(this);
	}
	End();
}

void ModelManipulator::ListUpModel()
{
	Separator();
	if (CollapsingHeader("Model List", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
	{
		static int selection_mask = (1 << 2);
		int node_clicked = -1;

		for (auto& model : vModels)
		{
			SetModelAsList(*model.get());
		}
	}
	Separator();
}

void ModelManipulator::SetModelAsList(AStaticMesh& staticMesh)
{
	const vector<shared_ptr<AStaticMesh>>& childrenMeshes = staticMesh.GetChildren();

	int treeNodeStyle = ImGuiTreeNodeFlags_OpenOnDoubleClick & ImGuiTreeNodeFlags_SpanAvailWidth;
	if (childrenMeshes.size() > 0)
	{
		treeNodeStyle = treeNodeStyle & ImGuiTreeNodeFlags_OpenOnArrow;
	}

	bool node_open = ImGui::TreeNodeEx(
		(void*)(intptr_t)staticMesh.sModelData.uiModelID,
		treeNodeStyle,
		staticMesh.GetMeshName().c_str()
	);

	if (node_open)
	{
		for (auto& child : childrenMeshes)
		{
			SetModelAsList(*child.get());
		}
		ImGui::TreePop();
	}
}

void ModelManipulator::AddModel(shared_ptr<AStaticMesh> spMesh)
{
	pModels.emplace(spMesh->sModelData.uiModelID, spMesh);
	for (auto& model : spMesh->GetChildren())
	{
		pModels.emplace(spMesh->sModelData.uiModelID, spMesh);
	}

	vModels.push_back(spMesh);
}

void ModelManipulator::ManipulateModel(AStaticMesh& staticMesh)
{
	DrawTransformation(&staticMesh);
}

void ModelManipulator::ManipulateModel(APBRStaticMesh& pbrStaticMesh)
{
	DrawTransformation((AStaticMesh*)(&pbrStaticMesh));
	Separator();
	DrawPBRTexture(&pbrStaticMesh);
}

void ModelManipulator::ManipulateModel(AIBLModel& iblModel)
{
	DrawTransformation((AStaticMesh*)(&iblModel));
	Separator();
	DrawIBLTexture(&iblModel);
}

void ModelManipulator::DrawTransformation(AStaticMesh* pStaticMesh)
{
	if (CollapsingHeader("Transformation", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
	{
		Separator();
		DragFloat3("Translation", pStaticMesh->GetPosition().m128_f32, 1.f, -1000000.f, 1000000.f, "%.2f");
		DragFloat3("Rotaion", &pStaticMesh->sAnglesDegree.fPitchDeg, 1.f, 0.f, 360.f, "%.2f");
		DragFloat3("Scale", &pStaticMesh->sScales.fScaleX, 0.005f, -1000000.f, 1000000.f, "%.2f");
	}
}

void ModelManipulator::DrawPBRTexture(APBRStaticMesh* pPBRStaticMesh)
{
	if (CollapsingHeader("PBR Model Textures"))
	{
		DragFloat3("Fresnel Reflectance", pPBRStaticMesh->sPBRConstant.fFresnelConstant, 0.005f, 0.f, 1.f, "%.3f");

		if (pPBRStaticMesh->pModelTexture[HEIGHT_TEXTURE_MAP])
		{
			DragFloat("Height Factor", &pPBRStaticMesh->sPBRConstant.fHeightFactor, 0.005f, 0.f, 1.f, "%.3f");
		}
		else
		{
			BeginDisabled();
			pPBRStaticMesh->sPBRConstant.fHeightFactor = 0.f;
			DragFloat("Height Factor", &pPBRStaticMesh->sPBRConstant.fHeightFactor, 0.005f, 0.f, 1.f, "%.3f");
			EndDisabled();
		}

		for (WORD idx = 0; idx < TEXTURE_MAP_NUM; ++idx)
		{
			SetTextureDragAndDrop(
				APBRStaticMesh::unmapTextureNames[idx].c_str(), 
				pPBRStaticMesh->pModelTexture[idx], 
				"Texture2D"
			);
		}
	}
}

void ModelManipulator::DrawIBLTexture(AIBLModel* pIBLModel)
{
	if (CollapsingHeader("IBL Model Textures"))
	{
		DragFloat(
			"Diffuse Rate",
			&pIBLModel->sIBLData.DiffuseRate,
			0.005f,
			0.f, 1.f, "%.4f"
		);

		SetTextureDragAndDrop(
			"Specular IBL Texture",
			pIBLModel->spEnvSpecularTextureFile,
			"CubeMap"
		);
		SetTextureDragAndDrop(
			"Diffuse IBL Texture",
			pIBLModel->spEnvDiffuseTextureFile,
			"CubeMap"
		);
		SetTextureDragAndDrop(
			"BRDF LUT Texture",
			pIBLModel->spEnvBrdfTextureFile,
			"Texture2D"
		);
	}
}

template<typename T>
void ModelManipulator::SetTextureDragAndDrop(
	const char* pDescription, 
	std::shared_ptr<T>& spFile, 
	const char* pDragDropLabel
)
{
	Separator();
	Text(pDescription);
	if (spFile != nullptr)
	{
		Image(spFile->GetSRV(), ImVec2(60.f, 60.f));
	}
	else
	{
		Dummy(ImVec2(60.f, 60.f));
	}

	if (BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(pDragDropLabel))
		{
			spFile = *(shared_ptr<T>*)payload->Data;
		}
		ImGui::EndDragDropTarget();
	}

	if (spFile != nullptr)
	{
		SameLine();
		Text(spFile->GetFileName().c_str());
	}
	else
	{
		SameLine();
		Text("");
	}
}