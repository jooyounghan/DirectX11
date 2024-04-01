#include "ModelManipulator.h"

#include "DefineVar.h"

#include "SkeletalModel.h"
#include "AIBLMesh.h"
#include "MirrorModel.h"
#include "Bone.h"

#include "SkeletalModelFile.h"
#include "StaticModelFile.h"
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
	AddObject(make_shared<CubeModel>(0.f, 5.f, 0.f, 1.f, false));
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

		if (BeginDragDropTarget())
		{
			const ImGuiPayload* payload = nullptr;
			if (payload = ImGui::AcceptDragDropPayload(DRAG_DROP_STATIC_MODEL_KEY))
			{
				StaticModelFile* tmpPtr = (StaticModelFile*)payload->Data;
				shared_ptr<StaticModelFile> staticModelFile = tmpPtr->shared_from_this();
				shared_ptr<GroupPBRModel> groupModel = make_shared<GroupPBRModel>(staticModelFile);
				AddObject(groupModel);
			}
			else if (payload = ImGui::AcceptDragDropPayload(DRAG_DROP_SKELETAL_MODEL_KEY))
			{
				SkeletalModelFile* tmpPtr = (SkeletalModelFile*)payload->Data;
				shared_ptr<SkeletalModelFile> skeletalModelFile = tmpPtr->shared_from_this();
				shared_ptr<SkeletalModel> skeletalModel = make_shared<SkeletalModel>(skeletalModelFile);
				AddObject(skeletalModel);
			}
			ImGui::EndDragDropTarget();
		}
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
		groupPBRModel.GetObjectName().c_str()
	);

	if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
	{
		SetSelctedModel(groupPBRModel.GetMeshID());
	}

	if (node_open)
	{
		for (auto& pbrMesh : vPBRMeshes)
		{
			BulletText(pbrMesh.GetObjectName().c_str());
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

void ModelManipulator::ManipulateModel(GroupPBRModel& groupPBRModel)
{
	DrawTransformation(groupPBRModel);
	Separator();
	vector<PBRStaticMesh>& vPBRMeshes = groupPBRModel.GetChildrenMeshesRef();
	for (auto& pbrMesh : vPBRMeshes)
	{
		Separator();
		DrawPBRTexture(pbrMesh);
	}
}

void ModelManipulator::ManipulateModel(SkeletalModel& skeletalModel)
{
	DrawTransformation(skeletalModel);
	Separator();

	Bone* pBone = skeletalModel.GetBone();
	if (pBone != nullptr && CollapsingHeader("Bone Data"))
	{
		DrawBone(*pBone);
		Separator();
	}

	DrawAnimInformation(skeletalModel);
	Separator();

	vector<PBRStaticMesh>& vPBRMeshes = skeletalModel.GetChildrenMeshesRef();
	for (auto& pbrMesh : vPBRMeshes)
	{
		DrawPBRTexture(pbrMesh);
		Separator();
	}
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

void ModelManipulator::DrawPBRTexture(PBRStaticMesh& pBRStaticMesh)
{
	MaterialFile* pMaterialFromMesh = pBRStaticMesh.GetMeshFileRef()->GetMaterial().get();


	if (pMaterialFromMesh != nullptr && CollapsingHeader(("PBR Model Material " + pMaterialFromMesh->GetFileLabel()).c_str()))
	{
		DragFloat3("Fresnel Reflectance", pBRStaticMesh.GetFresnelConstantAddress(), 0.005f, 0.f, 1.f, "%.3f");

		if (pMaterialFromMesh->GetTextureImageFileRef(HEIGHT_TEXTURE_MAP).get())
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

		BeginGroup();
		for (WORD idx = 0; idx < TEXTURE_MAP_NUM; ++idx)
		{
			SetTextureDragAndDrop(
				MaterialFile::GetTextureName(idx).c_str(), 
				pMaterialFromMesh->GetTextureImageFileRef((EModelTextures)idx),
				DRAG_DROP_TEXTURE_KEY
			);
		}
		EndGroup();

		if (BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(DRAG_DROP_MATERIAL_KEY))
			{
				MaterialFile* pMaterial = (MaterialFile*)payload->Data;
				pBRStaticMesh.GetMeshFileRef()->SetMaterial(pMaterial->shared_from_this());
			}
			ImGui::EndDragDropTarget();
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

void ModelManipulator::DrawBone(Bone& bone)
{
	vector<Bone>& bones = bone.GetBoneChildren();
	const ImGuiTreeNodeFlags style = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;

	if (bones.size() > 0)
	{
		bool node_open = ImGui::TreeNodeEx(
			bone.GetBoneName().c_str(),
			style,
			bone.GetBoneName().c_str()
		);

		if (node_open)
		{
			for (auto& boneChild : bone.GetBoneChildren())
			{
				DrawBone(boneChild);
			}
			ImGui::TreePop();
		}
	}
	else
	{
		BulletText(bone.GetBoneName().c_str());
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