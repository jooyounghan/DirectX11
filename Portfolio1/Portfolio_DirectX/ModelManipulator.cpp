#include "ModelManipulator.h"

#include "DefineVar.h"

#include "SinglePBRModel.h"
#include "GroupPBRModel.h"
#include "AIBLMesh.h"

#include "ModelFile.h"
#include "NormalImageFile.h"
#include "DDSImageFile.h"

// TODO : Áö¿ì±â
#include "CubeModel.h"
#include "CubeMapModel.h"


using namespace std;
using namespace ImGui;

ModelManipulator::ModelManipulator()
	: bIsDrawingNormal(false)
{
	//AddModel(make_shared<CubeModel>(-5.f, 0.f, 0.f, 1.f, false));
	//AddModel(make_shared<CubeModel>(5.f, 0.f, 0.f, 1.f, false));
	//AddModel(make_shared<CubeModel>(0.f, -5.f, 0.f, 1.f, false));
	//AddModel(make_shared<CubeModel>(0.f, 5.f, 0.f, 1.f, false));
	//AddModel(make_shared<CubeModel>(0.f, 0.f, 5.f, 1.f, false));
	//AddModel(make_shared<CubeModel>(0.f, 0.f, -5.f, 1.f, false));

	spIBLModel = make_shared<CubeMapModel>(500.f);
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
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
		if (BeginListBox("Model List", ImVec2(0.f, GetTextLineHeight() * 10.f)))
		{
			for (auto& model : pModels)
			{
				model.second->AcceptModelAsList(this);
				Separator();
			}
			EndListBox();
		}
		ImGui::PopStyleVar();

		if (BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DRAG_DROP_MESH_KEY))
			{
				ModelFile* tmpPtr = (ModelFile*)payload->Data;
				shared_ptr<ModelFile> modelFile = tmpPtr->shared_from_this();

				const vector<MeshFileSet>& vMeshFileSets = modelFile->GetMeshFileSet();
				for (auto& meshFile : vMeshFileSets)
				{
					shared_ptr<PBRStaticMesh> spPBRMesh = make_shared<PBRStaticMesh>(meshFile);
					AddModel(spPBRMesh);
				}
			}
			ImGui::EndDragDropTarget();
		}
	}

}

void ModelManipulator::SetModelAsList(SinglePBRModel& singlePBRModel)
{
	Selectable(singlePBRModel.GetMeshName().c_str());
}

void ModelManipulator::SetModelAsList(GroupPBRModel& groupPBRModel)
{
	const vector<PBRStaticMesh>& vPBRMeshes = groupPBRModel.GetChildrenMeshes();

	int treeNodeStyle = ImGuiTreeNodeFlags_OpenOnDoubleClick & ImGuiTreeNodeFlags_SpanAvailWidth;

	bool node_open = ImGui::TreeNodeEx(
		(void*)(intptr_t)groupPBRModel.GetMeshID(),
		treeNodeStyle,
		groupPBRModel.GetMeshName().c_str()
	);

	if (node_open)
	{
		for (auto& pbrMesh : vPBRMeshes)
		{
			Selectable(pbrMesh.GetMeshName().c_str());
		}
		ImGui::TreePop();
	}

	Selectable(groupPBRModel.GetMeshName().c_str());
}

void ModelManipulator::SetModelAsList(AIBLMesh& iblMesh)
{
	Selectable(iblMesh.GetMeshName().c_str());
}

void ModelManipulator::AddModel(shared_ptr<IMesh> spMesh)
{
	pModels.emplace(spMesh->GetMeshID(), spMesh);
}

void ModelManipulator::ManipulateModel(SinglePBRModel& singlePBRModel)
{
	DrawTransformation(&singlePBRModel);
}

void ModelManipulator::ManipulateModel(GroupPBRModel& groupPBRModel)
{
	DrawTransformation((&groupPBRModel));
	Separator();
	vector<PBRStaticMesh>& vPBRMeshes = groupPBRModel.GetChildrenMeshesRef();
	for (auto& pbrMesh : vPBRMeshes)
	{
		Separator();
		Text(pbrMesh.GetMeshName().c_str());
		DrawPBRTexture(&pbrMesh);
	}
}

void ModelManipulator::ManipulateModel(AIBLMesh& iblModel)
{
	DrawTransformation((&iblModel));
	Separator();
	DrawIBLTexture(&iblModel);
}

void ModelManipulator::DrawTransformation(ATransformerable* pTransformable)
{
	if (CollapsingHeader("Transformation", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
	{
		Separator();
		DragFloat3("Translation", pTransformable->GetPosition().m128_f32, 1.f, -1000000.f, 1000000.f, "%.2f");
		DragFloat3("Rotaion", pTransformable->GetAngles(), 1.f, 0.f, 360.f, "%.2f");
		DragFloat3("Scale", pTransformable->GetScale(), 0.005f, -1000000.f, 1000000.f, "%.2f");
	}
}

void ModelManipulator::DrawPBRTexture(PBRStaticMesh* pPBRStaticMesh)
{
	if (CollapsingHeader("PBR Model Textures"))
	{
		DragFloat3("Fresnel Reflectance", pPBRStaticMesh->GetFresnelConstantAddress(), 0.005f, 0.f, 1.f, "%.3f");

		if (pPBRStaticMesh->GetTextureImageFileRef(HEIGHT_TEXTURE_MAP).get())
		{
			DragFloat("Height Factor", pPBRStaticMesh->GetHeightFactorAddress(), 0.005f, 0.f, 1.f, "%.3f");
		}
		else
		{
			BeginDisabled();
			pPBRStaticMesh->SetHeightFactor(0.f);
			DragFloat("Height Factor", pPBRStaticMesh->GetHeightFactorAddress(), 0.005f, 0.f, 1.f, "%.3f");
			EndDisabled();
		}

		for (WORD idx = 0; idx < TEXTURE_MAP_NUM; ++idx)
		{
			SetTextureDragAndDrop(
				PBRStaticMesh::GetTextureName(idx).c_str(), 
				pPBRStaticMesh->GetTextureImageFileRef((EModelTextures)idx), 
				DRAG_DROP_TEXTURE_KEY
			);
		}
	}
}

void ModelManipulator::DrawIBLTexture(AIBLMesh* pIBLModel)
{
	if (CollapsingHeader("IBL Model Textures"))
	{
		DragFloat(
			"Diffuse Rate",
			pIBLModel->GetDiffuseRateAddress(),
			0.005f,
			0.f, 1.f, "%.4f"
		);

		SetTextureDragAndDrop(
			"Specular IBL Texture",
			pIBLModel->GetSpecularTextureFileRef(),
			DRAG_DROP_IBL_KEY
		);
		SetTextureDragAndDrop(
			"Diffuse IBL Texture",
			pIBLModel->GetDiffuseTextureFileRef(),
			DRAG_DROP_IBL_KEY
		);
		SetTextureDragAndDrop(
			"BRDF LUT Texture",
			pIBLModel->GetBRDFTextureFileRef(),
			DRAG_DROP_TEXTURE_KEY
		);
	}
}

void ModelManipulator::SetTextureDragAndDrop(
	const char* pDescription, 
	std::shared_ptr<IImageFile>& spFile, 
	const char* pDragDropLabel
)
{
	Separator();
	Text(pDescription);
	if (spFile != nullptr)
	{
		Image(spFile->GetThumbNailSRV(), ImVec2(60.f, 60.f));
	}
	else
	{
		Dummy(ImVec2(60.f, 60.f));
	}

	if (BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(pDragDropLabel))
		{
			IImageFile* imagePtr = (IImageFile*)payload->Data;
			spFile = imagePtr->shared_from_this();
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