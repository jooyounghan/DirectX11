#pragma once
#include "IGuiMenu.h"
#include <windows.h>
#include <vector>
#include <memory>

class StageManipulator : public IGuiMenu
{
public:
	StageManipulator(
		UINT& uiWidthIn,
		UINT& uiHeightIn,
		std::vector<std::shared_ptr<class ILight>>& pLightsIn,
		std::vector<std::shared_ptr<class ACamera>>& pCamerasIn
	);
	virtual ~StageManipulator();

protected:
	UINT& uiWidth;
	UINT& uiHeight;

protected:
	float	fCameraPos[3];
	float	fCameraAngle[3];
	int		uiCameraWidth;
	int		uiCameraHeight;
	float	fFovAngle;
	float	fNearPlane;
	float	fFarPlane;
	bool	bSupportMultiSampled;
	bool	bSupportHDR;
	bool	bSupportPicking;

protected:
	int		iSelectedVPWidth;
	int		iSelectedVPHeight;
	float	fSelecedFovDegree;
	bool	bIsSelectedSupportMultiSampled;
	bool	bIsSelectedSupportHDR;
	bool	bIsSelectedSupportPicking;

protected:
	float	fLightPos[3];

protected:
	void InitCameraVariable();

protected:
	std::vector<std::shared_ptr<ILight>>& pLights;
	std::vector<std::shared_ptr<ACamera>>& pCameras;

protected:
	std::shared_ptr<ILight> spSelectedLight;
	std::shared_ptr<ACamera> spSelectedCamera;

protected:
	bool bIsAddingCamera;
	bool bIsShowingCameraInfo;

protected:
	int iCameraSelectedIdx;

public:
	inline ILight* GetSelectedLight() { return spSelectedLight.get(); }
	inline ACamera* GetSelectedCamera() { return spSelectedCamera.get(); }

public:
	virtual void PopAsDialog() override;
	virtual bool IsGuiAvailable() override;

protected:
	void SetCameraGui();
	void AddCamera();
	void ShowCameraSetting(
		const bool& bIsEditable,
		float* pCameraPosIn,
		float* pCameraAngleIn,
		int* pViewportWidth,
		int* pViewportHeight,
		float* pFovAngle,
		float* pNearZ,
		float* pFarZ,
		bool* pIsSupportMultiSampled,
		bool* pSupportHDR,
		bool* pSupportPicking
	);
	void UpdateSelectedVariable();

protected:
	void SetLightGui();
};

