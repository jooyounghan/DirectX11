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
	virtual void AttachToBar() override;
	inline virtual bool IsGuiAvailable() override { return *ppSelectedStaticMesh != nullptr; }
};

