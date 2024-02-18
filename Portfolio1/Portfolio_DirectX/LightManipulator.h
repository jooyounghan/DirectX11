#pragma once
#include <windows.h>
#include <vector>
#include <memory>
#include <string>

class LightManipulator
{
	friend class SpotLight;
	friend class PointLight;

public:
	LightManipulator();
	~LightManipulator();

private:
	size_t ullSelectedLightKind;
	static const char* lightKind[];
	enum LightKind : size_t
	{
		LightKindSpot,
		LightKindPoint
	};

private:
	float	fLightPos[3];
	float	fLightAngleDeg[3];
	float	fLightColor[3];
	float	fFallOffStart;
	float	fFallOffEnd;
	float	fLightPower;
	float	fSpotPower;

private:
	std::vector<std::shared_ptr<class ILight>> pLights;

private:
	std::shared_ptr<ILight> spSelectedLight;

private:
	bool bIsAddingLight;
	bool bIsShowingLightInfo;

private:
	int iLightSelectedIdx;
	size_t ullLightListCheckIdx;

private:
	void InitLightVariable();

public:
	inline const std::vector<std::shared_ptr<ILight>>& GetLights() { return pLights; }
	inline ILight* GetSelectedLight() { return spSelectedLight.get(); }

public:
	void SetLightGui();

private:
	void ShowLightSpotSetting(
		float* fLightPosIn,
		float* fLightAngleDegIn,
		float* fLightColorIn,
		float* fFallOffStartIn,
		float* fFallOffEndIn,
		float* fLightPower,
		float* fSpotPower
	);
	void ShowLightPointSetting(
		float* fLightPosIn,
		float* fLightColorIn,
		float* fFallOffStartIn,
		float* fFallOffEndIn,
		float* fLightPower
	);

private:
	void AddLight();

private:
	void VisitLightList(class SpotLight& spotLight);
	void VisitLightList(class PointLight& pointLight);

private:
	void SetLightList(const std::string& strLightName);

private:
	void VisitLightSetting(class SpotLight& spotLight);
	void VisitLightSetting(class PointLight& pointLight);
};

