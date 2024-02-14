#pragma once
#include "IGuiMenu.h"

#include <memory>

class ModelManipulator : public IGuiMenu
{
public:
	ModelManipulator();
	virtual ~ModelManipulator();

private:
	class AStaticMesh** ppSelectedStaticMesh;
	bool bIsDrawingNormal;

public:
	inline AStaticMesh** GetAddressOfSelectedMesh() { return ppSelectedStaticMesh; }
	inline void SetAddressOfSelectedMesh(AStaticMesh** ppSelectedMeshIn) { ppSelectedStaticMesh = ppSelectedMeshIn; }
	inline bool GetIsDrawingNormal() { return bIsDrawingNormal; }

public:
	virtual void PopAsDialog() override;
	inline virtual bool IsGuiAvailable() override { return *ppSelectedStaticMesh != nullptr; }

public:
	void ManipulateModel(class AStaticMesh& staticMesh);
	void ManipulateModel(class APBRStaticMesh& pbrStaticMesh);
	void ManipulateModel(class AIBLModel& iblModel);

protected:
	void DrawTransformation(AStaticMesh* pStaticMesh);
	void DrawPBRTexture(APBRStaticMesh* pPBRStaticMesh);
	void DrawIBLTexture(AIBLModel* pIBLModel);

private:
	template<typename T>
	void SetTextureDragAndDrop(
		const char* pDescription, 
		std::shared_ptr<T>& spFile,
		const char* pDragDropLabel
	);
};
