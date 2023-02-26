#pragma once

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <vector>
#include <string>
#include <memory>

#include "Delegator.h"

using std::string;
using std::vector;
using std::shared_ptr;

struct ModelData
{
	string base_path;
	string file_name;
	float model_translation[3]{ 0.f, 0.f, 0.f };
	float model_rotation[3]{ 0.f, 0.f, 0.f };
	float model_scaling[3]{ 1.f, 1.f, 1.f };
};

class ModelSelectDialog
{
public:
	ModelSelectDialog();

public:
	std::vector<shared_ptr<ModelData>>	m_model_files_;
	int selected_model_idx = -1;

public:
	float m_translation_[3];
	float m_rotation_[3];
	float m_scaling_[3];

public:
	Delegator<void, const string&, const string&>	m_on_file_added_;
	Delegator<void, const size_t&>					m_on_file_deleted_;
	Delegator<void, const size_t&, float*, float*, float*>	m_on_model_transformed;

public:
	void CreateModelSelector(const float& delta_time);
};

