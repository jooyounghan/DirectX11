#pragma once
#include <windows.h>
#include <vector>
#include <memory>
#include <string>

class CameraManipulator
{
	friend class ACamera;
	friend class FilteredCamera;
	friend class PickableCamera;
	friend class BlurFilter;
	friend class BloomFilter;

public:
	CameraManipulator(
		UINT& uiWidthIn,
		UINT& uiHeightIn,
		class ModelManipulator* pModelManipulatorIn
	);
	~CameraManipulator();

private:
	UINT& uiWidth;
	UINT& uiHeight;

private:
	size_t ullSelectedCameraKind;
	static const char* cameraKind[];
	enum CameraKind : size_t
	{
		CameraKindFiltered,
		CameraKindPickable
	};

private:
	float	fCameraPos[3];
	float	fCameraAngle[3];
	int		uiCameraWidth;
	int		uiCameraHeight;
	float	fFovAngleDegree;
	float	fNearPlane;
	float	fFarPlane;
	bool	bSupportMultiSampled;
	bool	bSupportHDR;

private:
	std::vector<std::shared_ptr<ACamera>> pCameras;

private:
	std::shared_ptr<ACamera> spSelectedCamera;

private:
	bool bIsAddingCamera;
	bool bIsShowingCameraInfo;

private:
	int iCameraSelectedIdx;
	size_t ullCameraListCheckIdx;

private:
	int iFilterSelectedIdx;
	size_t ullFilterListCheckIdx;

private:
	class ModelManipulator* pModelManipulator;

private:
	void InitCameraVariable();

public:
	inline const std::vector<std::shared_ptr<ACamera>>& GetCameras() { return pCameras; }
	inline ACamera* GetSelectedCamera() { return spSelectedCamera.get(); }

public:
	void SetCameraGui();

private:
	void AddCamera();

public:
	void ResizeSelectedCamera(const UINT uiWidthIn, const UINT uiHeightIn);
	void ProcWindowMsg(UINT msg, WPARAM wParam, LPARAM lParam);

private:
	void ShowCameraSetting(
		float* pCameraPosIn,
		float* pCameraAngleIn,
		int* pViewportWidth,
		int* pViewportHeight,
		float* pFovAngle,
		float* pNearZ, float* pFarZ,
		bool* pIsSupportMultiSampled,
		bool* pSupportHDR
	);

private:
	void VisitCameraList(FilteredCamera& filtered);
	void VisitCameraList(PickableCamera& pickable);

private:
	void SetCameraList(const std::string& strCameraName);

private:
	void VisitCameraInfo(FilteredCamera& fFiltered);
	void VisitCameraInfo(PickableCamera& pickable);

private:
	void VisitFilterList(BlurFilter& blurFilter);
	void VisitFilterList(BloomFilter& bloomFilter);
	void VisitFilterList(ACamera& camera);
	void VisitFilterList(PickableCamera& pickable);

private:
	void SetFilterList(const std::string& strFilterName);

private:
	void VisitLButtonDown(
		FilteredCamera& fFiltered,
		const int& xPosIn,
		const int& yPosIn
	);
	void VisitLButtonDown(
		PickableCamera& pickable,
		const int& xPosIn,
		const int& yPosIn
	);
};

