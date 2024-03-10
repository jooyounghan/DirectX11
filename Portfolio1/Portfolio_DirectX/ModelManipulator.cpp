#include "ModelManipulator.h"

#include "DefineVar.h"

#include "SinglePBRModel.h"
#include "GroupPBRModel.h"
#include "AIBLMesh.h"
#include "MirrorModel.h"

#include "ModelFile.h"
#include "NormalImageFile.h"
#include "DDSImageFile.h"

// TODO : Áö¿ì±â
#include "CubeModel.h"
#include "CubeMapModel.h"
#include "MirrorModel.h"

using namespace std;
using namespace ImGui;

ModelManipulator::ModelManipulator()
	: bIsDrawingNormal(false), uiSelectedModelIdx(0)
{
	//AddModel(make_shared<CubeModel>(-5.f, 0.f, 0.f, 1.f, false));
	//AddModel(make_shared<CubeModel>(5.f, 0.f, 0.f, 1.f, false));
	//AddModel(make_shared<CubeModel>(0.f, -5.f, 0.f, 1.f, false));
	//AddModel(make_shared<CubeModel>(0.f, 5.f, 0.f, 1.f, false));
	AddModel(make_shared<CubeModel>(0.f, 0.f, 0.f, 1.f, false));
	AddModel(make_shared<MirrorModel>(3.f, 3.f, 0.f, 0.f, 3.f, 0.f, 0.f, 0.f));
	AddModel(make_shared<MirrorModel>(3.f, 3.f, 0.f, 0.f, -3.f, 0.f, 0.f, 0.f));
	spIBLModel = make_shared<CubeMapModel>(500.f);
	AddModel(spIBLModel);
}

ModelManipulator::~ModelManipulator()
{

}

void ModelManipulator::SetSelctedModel(const uint32_t& selected_id)
{
	uiSelectedModelIdx = selected_id;
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
				shared_ptr<GroupPBRModel> groupModel = make_shared<GroupPBRModel>(modelFile);
				AddModel(groupModel);
			}
			ImGui::EndDragDropTarget();
		}
	}

}

void ModelManipulator::SetModelAsList(SinglePBRModel& singlePBRModel)
{
	if (Selectable(singlePBRModel.GetMeshName().c_str(), singlePBRModel.GetMeshID() == uiSelectedModelIdx))
	{
		SetSelctedModel(singlePBRModel.GetMeshID());
	}
}

void ModelManipulator::SetModelAsList(GroupPBRModel& groupPBRModel)
{
	const vector<PBRStaticMesh>& vPBRMeshes = groupPBRModel.GetChildrenMeshes();

	int treeNodeStyle = ImGuiTreeNodeFlags_OpenOnArrow | 
		ImGuiTreeNodeFlags_OpenOnDoubleClick | 
		ImGuiTreeNodeFlags_SpanAvailWidth;
	uiSelectedModelIdx == groupPBRModel.GetMeshID() ? treeNodeStyle |= ImGuiTreeNodeFlags_Selected : false;

	bool node_open = ImGui::TreeNodeEx(
		(void*)(intptr_t)groupPBRModel.GetMeshID(),
		treeNodeStyle,
		groupPBRModel.GetMeshName().c_str()
	);

	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	{
		SetSelctedModel(groupPBRModel.GetMeshID());
	}

	if (node_open)
	{
		for (auto& pbrMesh : vPBRMeshes)
		{
			BulletText(pbrMesh.GetMeshName().c_str());
		}
		ImGui::TreePop();
	}
}

void ModelManipulator::SetModelAsList(MirrorModel& mirrorModel)
{
	if (Selectable(mirrorModel.GetMeshName().c_str(), mirrorModel.GetMeshID() == uiSelectedModelIdx))
	{
		SetSelctedModel(mirrorModel.GetMeshID());
	}
}

void ModelManipulator::SetModelAsList(AIBLMesh& iblMesh)
{
	if (Selectable(iblMesh.GetMeshName().c_str(), iblMesh.GetMeshID() == uiSelectedModelIdx))
	{
		SetSelctedModel(iblMesh.GetMeshID());
	}
}

void ModelManipulator::AddModel(shared_ptr<IMesh> spMesh)
{
	pModels.emplace(spMesh->GetMeshID(), spMesh);
}

void ModelManipulator::ManipulateModel(SinglePBRModel& singlePBRModel)
{
	DrawTransformation(singlePBRModel);
	DrawPBRTexture(singlePBRModel);
}

void ModelManipulator::ManipulateModel(GroupPBRModel& groupPBRModel)
{
	DrawTransformation((groupPBRModel));
	Separator();
	vector<PBRStaticMesh>& vPBRMeshes = groupPBRModel.GetChildrenMeshesRef();
	for (auto& pbrMesh : vPBRMeshes)
	{
		Separator();
		DrawPBRTexture(pbrMesh);
	}
}

void ModelManipulator::ManipulateModel(AIBLMesh& iblModel)
{
	DrawTransformation((iblModel));
	Separator();
	DrawIBLTexture(iblModel);
}

void ModelManipulator::ManipulateModel(MirrorModel& mirrorModel)
{
	if (CollapsingHeader("Transformation", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
	{
		Separator();
		DragFloat3("Translation", mirrorModel.GetPosition().m128_f32, 0.1f, -1000000.f, 1000000.f, "%.2f");
		DragFloat3("Rotaion", mirrorModel.GetAngles(), 0.1f, 0.f, 360.f, "%.2f");
		DragFloat("Fov Angle(Deg)", mirrorModel.GetFovDegreeAddress(), 0.01f, 30.f, 120.f, "%.3f");
	}
	DrawMirrorProperties(mirrorModel);
}

void ModelManipulator::DrawTransformation(ATransformerable& transformable)
{
	if (CollapsingHeader("Transformation", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
	{
		Separator();
		DragFloat3("Translation", transformable.GetPosition().m128_f32, 0.1f, -1000000.f, 1000000.f, "%.2f");
		DragFloat3("Rotaion", transformable.GetAngles(), 0.1f, 0.f, 360.f, "%.2f");
		DragFloat3("Scale", transformable.GetScale(), 0.005f, -1000000.f, 1000000.f, "%.2f");
	}
}

void ModelManipulator::DrawPBRTexture(PBRStaticMesh& pBRStaticMesh)
{
	if (CollapsingHeader(("PBR Model Textures " + pBRStaticMesh.GetMeshName()).c_str()))
	{
		DragFloat3("Fresnel Reflectance", pBRStaticMesh.GetFresnelConstantAddress(), 0.005f, 0.f, 1.f, "%.3f");

		if (pBRStaticMesh.GetTextureImageFileRef(HEIGHT_TEXTURE_MAP).get())
		{
			DragFloat("Height Factor", pBRStaticMesh.GetHeightFactorAddress(), 0.005f, 0.f, 1.f, "%.3f");
		}
		else
		{
			BeginDisabled();
			pBRStaticMesh.SetHeightFactor(0.f);
			DragFloat("Height Factor", pBRStaticMesh.GetHeightFactorAddress(), 0.005f, 0.f, 1.f, "%.3f");
			EndDisabled();
		}

		for (WORD idx = 0; idx < TEXTURE_MAP_NUM; ++idx)
		{
			SetTextureDragAndDrop(
				PBRStaticMesh::GetTextureName(idx).c_str(), 
				pBRStaticMesh.GetTextureImageFileRef((EModelTextures)idx),
				DRAG_DROP_TEXTURE_KEY
			);
		}
	}
}

void ModelManipulator::DrawIBLTexture(AIBLMesh& iBLModel)
{
	if (CollapsingHeader("IBL Model Textures"))
	{
		DragFloat(
			"Diffuse Rate",
			iBLModel.GetDiffuseRateAddress(),
			0.005f,
			0.f, 1.f, "%.4f"
		);

		SetTextureDragAndDrop(
			"Specular IBL Texture",
			iBLModel.GetSpecularTextureFileRef(),
			DRAG_DROP_IBL_KEY
		);
		SetTextureDragAndDrop(
			"Diffuse IBL Texture",
			iBLModel.GetDiffuseTextureFileRef(),
			DRAG_DROP_IBL_KEY
		);
		SetTextureDragAndDrop(
			"BRDF LUT Texture",
			iBLModel.GetBRDFTextureFileRef(),
			DRAG_DROP_TEXTURE_KEY
		);
	}
}

void ModelManipulator::DrawMirrorProperties(MirrorModel& mirrorModel)
{
	if (CollapsingHeader(("Mirror Properties " + mirrorModel.GetMeshName()).c_str()))
	{
		DragFloat("Alpha Constant", mirrorModel.GetAlphaAddress(), 0.005f, 0.f, 1.f, "%.3f");
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