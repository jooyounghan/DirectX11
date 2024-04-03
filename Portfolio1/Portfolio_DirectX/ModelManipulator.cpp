#include "ModelManipulator.h"

#include "DefineVar.h"

#include "PBRStaticMesh.h"
#include "AIBLMesh.h"
#include "MirrorModel.h"
#include "SkeletalModel.h"

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
	//AddObject(make_shared<CubeModel>(0.f, 5.f, 0.f, 1.f, false));
	//AddModel(make_shared<CubeModel>(0.f, 0.f, 0.f, 1.f, false));
	//AddModel(make_shared<MirrorModel>(3.f, 3.f, 0.f, 0.f, 3.f, 0.f, 0.f, 0.f));
	//AddModel(make_shared<MirrorModel>(10.f, 10.f, 0.f, -2.f, 0.f, 270.f, 0.f, 0.f));
	//AddModel(make_shared<CubeModel>(0.f, 0.f, 0.f, 10.f, true));
	spIBLModel = make_shared<CubeMapModel>(500.f);
	AddObject(spIBLModel);
}

ModelManipulator::~ModelManipulator()
{

}

void ModelManipulator::SetSelctedModel(const uint32_t& selected_id)
{
	uiSelectedModelIdx = selected_id;
	if (pObjects.find(selected_id) != pObjects.end())
	{
		spSelectedObject = pObjects[selected_id];
	}
	else
	{
		spSelectedObject = nullptr;
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

	if (spSelectedObject != nullptr)
	{
		spSelectedObject->AcceptModelManipulating(this);
	}
	End();
}

void ModelManipulator::ListUpModel()
{
	Separator();
	if (CollapsingHeader("Model List", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
	{
		BeginGroup();
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
		if (BeginListBox("Model List", ImVec2(0.f, GetTextLineHeight() * 10.f)))
		{
			for (auto& model : pObjects)
			{
				model.second->AcceptModelAsList(this);
				Separator();
			}
			EndListBox();
		}
		ImGui::PopStyleVar();
		EndGroup();

		if (BeginDragDropTarget())
		{
			const ImGuiPayload* payload = nullptr;
			if (payload = ImGui::AcceptDragDropPayload(DRAG_DROP_MESH_KEY))
			{
				MeshFile* pMeshFile = (MeshFile*)payload->Data;
				shared_ptr<MeshFile> spMeshFile = pMeshFile->shared_from_this();
				shared_ptr<PBRStaticMesh> meshObject = 
					spMeshFile->IsSkeletal() ? 
					make_shared<SkeletalModel>(spMeshFile)
					: make_shared<PBRStaticMesh>(spMeshFile);
				AddObject(meshObject);
			}
			ImGui::EndDragDropTarget();
		}
	}

}

void ModelManipulator::SetModelAsList(PBRStaticMesh& pbrStaticMesh)
{
	const size_t meshNums = pbrStaticMesh.GetMeshNums();

	int treeNodeStyle = ImGuiTreeNodeFlags_OpenOnArrow | 
		ImGuiTreeNodeFlags_OpenOnDoubleClick | 
		ImGuiTreeNodeFlags_SpanAvailWidth;
	uiSelectedModelIdx == pbrStaticMesh.GetMeshID() ? treeNodeStyle |= ImGuiTreeNodeFlags_Selected : false;

	bool node_open = ImGui::TreeNodeEx(
		(void*)(intptr_t)pbrStaticMesh.GetMeshID(),
		treeNodeStyle,
		pbrStaticMesh.GetObjectName().c_str()
	);

	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	{
		SetSelctedModel(pbrStaticMesh.GetMeshID());
	}

	if (node_open)
	{
		for (size_t meshIdx = 0; meshIdx < meshNums; ++meshIdx)
		{
			const string meshDataText = string("Mesh " + to_string(meshIdx));
			BulletText(meshDataText.c_str());
		}
		ImGui::TreePop();
	}
}

void ModelManipulator::SetModelAsList(MirrorModel& mirrorModel)
{
	if (Selectable(mirrorModel.GetObjectName().c_str(), mirrorModel.GetMeshID() == uiSelectedModelIdx))
	{
		SetSelctedModel(mirrorModel.GetMeshID());
	}
}

void ModelManipulator::SetModelAsList(AIBLMesh& iblMesh)
{
	if (Selectable(iblMesh.GetObjectName().c_str(), iblMesh.GetMeshID() == uiSelectedModelIdx))
	{
		SetSelctedModel(iblMesh.GetMeshID());
	}
}

void ModelManipulator::AddObject(shared_ptr<IObject> spObject)
{
	pObjects.emplace(spObject->GetMeshID(), spObject);
}

void ModelManipulator::ManipulateModel(PBRStaticMesh& pbrStaticMesh)
{
	DrawTransformation(pbrStaticMesh);
	Separator();
	DrawPBRTexture(pbrStaticMesh);
}

void ModelManipulator::ManipulateModel(SkeletalModel& skeletalModel)
{
	DrawTransformation(skeletalModel);
	Separator();

	MeshFile* pMeshFile = skeletalModel.GetMeshFile();
	if (pMeshFile != nullptr)
	{
		BoneFile* pBoneFile = pMeshFile->GetBoneFile();
		if (pBoneFile != nullptr && CollapsingHeader("Bone Data"))
		{
			DrawBone(pBoneFile->GetRootBone());
			Separator();
		}
	}

	DrawAnimInformation(skeletalModel);
	Separator();

	DrawPBRTexture(skeletalModel);
	Separator();
}

void ModelManipulator::ManipulateModel(AIBLMesh& iblModel)
{
	DrawTransformation(iblModel);
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

void ModelManipulator::DrawPBRTexture(PBRStaticMesh& pbrStaticMesh)
{
	if (CollapsingHeader(("PBR Model Material " + pbrStaticMesh.GetObjectName()).c_str()))
	{
		DragFloat3("Fresnel Reflectance", pbrStaticMesh.GetFresnelConstantAddress(), 0.005f, 0.f, 1.f, "%.3f");
		DragFloat("Height Factor", pbrStaticMesh.GetHeightFactorAddress(), 0.005f, 0.f, 1.f, "%.3f");


		const size_t meshNums = pbrStaticMesh.GetMeshNums();
		for (size_t meshIdx = 0; meshIdx < meshNums; ++meshIdx)
		{
			BeginGroup();
			MaterialFile* pMaterial = pbrStaticMesh.GetMaterialFile(meshIdx);
			for (size_t idx = 0; idx < TEXTURE_MAP_NUM; ++idx)
			{
				SetTextureDragAndDrop(
					MaterialFile::GetTextureName(idx).c_str(),
					pMaterial->GetTextureImageFileRef((EModelTextures)idx),
					DRAG_DROP_TEXTURE_KEY
				);
			}
			EndGroup();
			if (BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DRAG_DROP_MATERIAL_KEY))
				{
					MaterialFile* pMaterial = (MaterialFile*)payload->Data;
					pbrStaticMesh.SetMaterialFile(meshIdx, pMaterial->shared_from_this());
				}
				ImGui::EndDragDropTarget();
			}

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
	if (CollapsingHeader(("Mirror Properties " + mirrorModel.GetObjectName()).c_str()))
	{
		DragFloat("Alpha Constant", mirrorModel.GetAlphaAddress(), 0.005f, 0.f, 1.f, "%.3f");
	}
}

void ModelManipulator::DrawBone(const BoneData& boneData)
{
	const ImGuiTreeNodeFlags style = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;

	if (boneData.vBoneChildren.size() > 0)
	{
		bool node_open = ImGui::TreeNodeEx(
			boneData.strBoneName.c_str(),
			style,
			boneData.strBoneName.c_str()
		);

		if (node_open)
		{
			for (auto& boneChild : boneData.vBoneChildren)
			{
				DrawBone(boneChild);
			}
			ImGui::TreePop();
		}
	}
	else
	{
		BulletText(boneData.strBoneName.c_str());
	}
}

void ModelManipulator::DrawAnimInformation(SkeletalModel& skeletalModel)
{
	if (CollapsingHeader("Animation Data"))
	{
		AnimationFile* pAnimationFile = skeletalModel.GetAnimationFile();
		if (pAnimationFile != nullptr)
		{
			double dblMin = 0.1;
			double dblMax = 3.0;
			DragScalar(
				"Animation Speed", ImGuiDataType_::ImGuiDataType_Double,
				skeletalModel.GetAnimSpeedAddress(),
				0.001f,
				&dblMin, &dblMax, "%.3f"
			);
		}

		ShowFileWithThumbNail(pAnimationFile);

		if (BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DRAG_DROP_ANIMATION_KEY))
			{
				AnimationFile* pAnimFile = (AnimationFile*)payload->Data;
				skeletalModel.SetAnimationFile(pAnimFile->shared_from_this());
			}
			ImGui::EndDragDropTarget();
		}
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
	ShowFileWithThumbNail(spFile.get());

	if (BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(pDragDropLabel))
		{
			IImageFile* imagePtr = (IImageFile*)payload->Data;
			spFile = imagePtr->shared_from_this();
		}
		ImGui::EndDragDropTarget();
	}
}