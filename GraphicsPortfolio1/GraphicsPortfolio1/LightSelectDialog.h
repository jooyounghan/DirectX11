#pragma once

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <vector>
#include <memory>

#include "Delegator.h"
#include "Light.h"

using std::shared_ptr;

class LightSelectDialog
{
public:
	LightSelectDialog();

public:
	const float& m_vmax_ = 10.f;
	const float& m_vzero_ = 0.f;
	const float& m_vmin_ = -10.f;

public:
	enum LightType
	{
		Directional,
		Point,
		Spot
	};

	enum GridProp
	{
		LightType,
		Position,
		Direction,
		FallOff,
		SpotPower
	};

	bool m_selected_light_type_[3];
	bool& is_directional_light_ = m_selected_light_type_[0];
	bool& is_point_light_ = m_selected_light_type_[1];
	bool& is_spot_light_ = m_selected_light_type_[2];
	float m_ligth_position_[3];
	float m_ligth_color_[3];
	float m_fall_off_[2];
	float m_spot_power_;

public:
	float m_light_translation_[3];

public:
	vector<shared_ptr<LightConstantData>> lights;
	int selected_light_idx = -1;

public:
	void CreateLightSelector(const float& delta_time);
};

