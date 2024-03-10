#pragma once
#include "IGuiMenu.h"

#include <vector>
#include <memory>
#include <unordered_map>

class IMesh;
class AIBLMesh;
class PBRStaticMesh;
class ATransformerable;
class SinglePBRModel;
class GroupPBRModel;
class AIBLMesh;
class MirrorModel;

class ModelManipulator : public IGuiMenu
{
	friend SinglePBRModel;
	friend GroupPBRModel;
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
	std::unordered_map<uint32_t, std::shared_ptr<IMesh>> pModels;

private:
	uint32_t uiSelectedModelIdx;

private:
	std::shared_ptr<AIBLMesh> spIBLModel;
	std::shared_ptr<IMesh> spSelectedMesh;

public:
	inline const std::unordered_map<uint32_t, std::shared_ptr<IMesh>>& GetModels() { return pModels; }
	inline const std::shared_ptr<AIBLMesh>& GetIBLModel() { return spIBLModel; }

public:
	void AddModel(std::shared_ptr<IMesh> spMesh);

public:
	virtual void PopAsDialog() override;
	inline virtual bool IsGuiAvailable() override { return spSelectedMesh != nullptr; }

private:
	void ListUpModel();

private:
	void SetModelAsList(SinglePBRModel& singlePBRModel);
	void SetModelAsList(GroupPBRModel& groupPBRModel);
	void SetModelAsList(AIBLMesh& iblMesh);
	void SetModelAsList(MirrorModel& mirrorModel);

private:
	void ManipulateModel(SinglePBRModel& singlePBRModel);
	void ManipulateModel(GroupPBRModel& groupPBRModel);
	void ManipulateModel(AIBLMesh& iblMesh);
	void ManipulateModel(MirrorModel& mirrorModel);

private:
	void DrawTransformation(ATransformerable& transformable);

private:
	void DrawPBRTexture(PBRStaticMesh& pBRStaticMesh);
	void DrawIBLTexture(AIBLMesh& iBLModel);
	void DrawMirrorProperties(MirrorModel& mirrorModel);

private:
	void SetTextureDragAndDrop(
		const char* pDescription, 
		std::shared_ptr<class IImageFile>& spFile,
		const char* pDragDropLabel
	);
};
