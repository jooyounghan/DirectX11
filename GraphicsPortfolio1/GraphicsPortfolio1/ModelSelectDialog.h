#pragma once

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <vector>
#include <string>
#include <memory>

#include "MeshData.h"
#include "Delegator.h"

using std::string;
using std::vector;
using std::shared_ptr;

class ModelSelectDialog
{
public:
	ModelSelectDialog();

protected:
	std::vector<shared_ptr<ModelData>>	m_model_files_;
	int selected_model_idx = -1;

protected:
	float m_translation_[3];
	float m_rotation_[3];
	float m_scaling_[3];

protected:
	float m_material[3];

public:
	Delegator<void, const ModelData&>					m_on_file_added_;
	Delegator<void, const size_t&>						m_on_file_deleted_;
	Delegator<void, const size_t&, const ModelData&>	m_on_model_transformed;
	Delegator<void, const size_t&, const ModelData&>	m_on_material_changed;
public:
	void CreateModelSelector(const float& delta_time);
};

