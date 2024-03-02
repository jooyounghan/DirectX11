#pragma once
#include "IGuiMenu.h"
#include <vector>
#include <memory>
#include <unordered_map>

class ModelManipulator : public IGuiMenu
{
public:
	ModelManipulator();
	virtual ~ModelManipulator();

private:
	bool bIsDrawingNormal;

public:
	void SetSelctedMesh(const uint32_t selected_id);
	inline bool GetIsDrawingNormal() { return bIsDrawingNormal; }

private:
	std::unordered_map<uint32_t, std::shared_ptr<class AStaticMesh>> pModels;

private:
	std::vector<std::shared_ptr<AStaticMesh>> vModels;

private:
	std::shared_ptr<class AIBLModel> spIBLModel;
	std::shared_ptr<AStaticMesh> spSelectedMesh;

public:
	inline const std::unordered_map<uint32_t, std::shared_ptr<AStaticMesh>>& GetModels() { return pModels; }
	inline const std::shared_ptr<AIBLModel>& GetIBLModel() { return spIBLModel; }
public:
	void AddModel(std::shared_ptr<AStaticMesh> spMesh);

public:
	virtual void PopAsDialog() override;
	inline virtual bool IsGuiAvailable() override { return spSelectedMesh != nullptr; }

private:
	void ListUpModel();
	void SetModelAsList(class AStaticMesh& staticMesh);






public:
	void ManipulateModel(class AStaticMesh& staticMesh);
	void ManipulateModel(class PBRStaticMesh& pbrStaticMesh);
	void ManipulateModel(class AIBLModel& iblModel);

protected:
	void DrawTransformation(AStaticMesh* pStaticMesh);

protected:
	void DrawPBRTexture(PBRStaticMesh* pPBRStaticMesh);
	void DrawIBLTexture(AIBLModel* pIBLModel);

private:
	void SetTextureDragAndDrop(
		const char* pDescription, 
		std::shared_ptr<class IImageFile>& spFile,
		const char* pDragDropLabel
	);
};
