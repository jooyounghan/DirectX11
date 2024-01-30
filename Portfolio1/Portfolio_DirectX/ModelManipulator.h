#pragma once
#include "IGuiMenu.h"

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
	void VisitModel(class AStaticMesh& staticMesh);
	void VisitModel(class APBRStaticMesh& pbrStaticMesh);

protected:
	void DrawTransformation(AStaticMesh* pStaticMesh);
	void DrawModelTexture(APBRStaticMesh* pPBRStaticMesh);
};

