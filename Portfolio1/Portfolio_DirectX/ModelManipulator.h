#pragma once
#include "IGuiMenu.h"

#include <memory>

class ModelManipulator : public IGuiMenu
{
public:
	ModelManipulator(class AStaticMesh** ppSelectedStaticMeshIn);
	virtual ~ModelManipulator();

public:
	AStaticMesh** ppSelectedStaticMesh;

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
