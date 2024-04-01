#pragma once
#include "IGuiMenu.h"

#include <vector>
#include <memory>
#include <unordered_map>

class IObject;
class IMesh;
class AIBLMesh;
class PBRStaticMesh;
class ATransformerable;
class GroupPBRModel;
class SkeletalModel;
class AIBLMesh;
class MirrorModel;
class Bone;
class Animation;

class ModelManipulator : public IGuiMenu
{
	friend GroupPBRModel;
	friend SkeletalModel;
	friend AIBLMesh;
	friend MirrorModel;

public:
	ModelManipulator();
	virtual ~ModelManipulator();

private:
	bool bIsDrawingNormal;

public:
	void SetSelctedModel(const uint32_t& selected_id);
	const uint32_t& GetSelectedModelID() { return uiSelectedModelIdx; }

public:
	inline bool GetIsDrawingNormal() { return bIsDrawingNormal; }

private:
	std::unordered_map<uint32_t, std::shared_ptr<IObject>> pObjects;

private:
	uint32_t uiSelectedModelIdx;

private:
	std::shared_ptr<AIBLMesh> spIBLModel;
	std::shared_ptr<IObject> spSelectedObject;

public:
	inline const std::unordered_map<uint32_t, std::shared_ptr<IObject>>& GetModels() { return pObjects; }
	inline const std::shared_ptr<AIBLMesh>& GetIBLModel() { return spIBLModel; }

public:
	void AddObject(std::shared_ptr<IObject> spObject);

public:
	virtual void PopAsDialog() override;
	inline virtual bool IsGuiAvailable() override { return spSelectedObject != nullptr; }

private:
	void ListUpModel();

private:
	void SetModelAsList(GroupPBRModel& groupPBRModel);
	void SetModelAsList(AIBLMesh& iblMesh);
	void SetModelAsList(MirrorModel& mirrorModel);

private:
	void ManipulateModel(GroupPBRModel& groupPBRModel);
	void ManipulateModel(SkeletalModel& skeletalModel);
	void ManipulateModel(AIBLMesh& iblMesh);
	void ManipulateModel(MirrorModel& mirrorModel);

private:
	void DrawTransformation(ATransformerable& transformable);

private:
	void DrawPBRTexture(PBRStaticMesh& pBRStaticMesh);
	void DrawIBLTexture(AIBLMesh& iBLModel);
	void DrawMirrorProperties(MirrorModel& mirrorModel);

private:
	void DrawBone(Bone& bone);
	void DrawAnimInformation(SkeletalModel& skeletalModel);

private:
	void SetTextureDragAndDrop(
		const char* pDescription, 
		std::shared_ptr<class IImageFile>& spFile,
		const char* pDragDropLabel
	);
	
private:
	template<typename T>
	void ShowFileWithThumbNail(const T& spFile);
};

template<typename T>
inline void ModelManipulator::ShowFileWithThumbNail(const T& spFile)
{
	ImGui::BeginGroup();
	if (spFile != nullptr)
	{
		ImGui::Image(spFile->GetThumbNailSRV(), ImVec2(60.f, 60.f));
		ImGui::SameLine();
		ImGui::Text(spFile->GetFileLabel().c_str());
	}
	else
	{
		ImGui::Dummy(ImVec2(60.f, 60.f));
		ImGui::SameLine();
		ImGui::Text("");
	}
	ImGui::EndGroup();
}
