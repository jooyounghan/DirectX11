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

protected:
	const float& m_vmax_ = 10.f;
	const float& m_vzero_ = 0.f;
	const float& m_vmin_ = -10.f;

protected:
	enum GridProp
	{
		Selected,
		LightType,
		Color
	};

protected:
	bool m_selected_light_type_[3];
	bool& is_directional_light_ = m_selected_light_type_[0];
	bool& is_point_light_ = m_selected_light_type_[1];
	bool& is_spot_light_ = m_selected_light_type_[2];

protected:
	float m_ligth_color_[3];
	float m_ligth_position_[3];
	float m_ligth_direction_[3];
	float m_light_power_;

	float m_fall_off_[2];
	float m_spot_power_;

protected:
	float m_light_translation_[3];

protected:
	LightConstantData*	m_light_constant_data_;
	int					m_light_cnt_ = 0;
	int					m_selected_light_idx_ = -1;

protected:
	void SwitchOffLightType();
	void SwitchOffSelectedLight();

protected:
	const bool IsDirectionalLightReady(LightConstantData* selected_light_constant_data);
	const bool IsPointLightReady(LightConstantData* selected_light_constant_data);
	const bool IsSpotLightReady(LightConstantData* selected_light_constant_data);
	const bool IsCreatingLight();
	const bool IsLightSelected();
public:
	Delegator<void, const LightConstantData&, OUT LightConstantData*&>	m_on_light_added_;
	Delegator<void, const size_t&>				m_on_light_deleted_;

public:
	void CreateLightSelector(const float& delta_time);
};

