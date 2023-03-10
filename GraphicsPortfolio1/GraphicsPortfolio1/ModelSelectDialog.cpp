#include "ModelSelectDialog.h"
#include "ImGuiFileDialog.h"
#include "ImGuiFileDialogConfig.h"
#include "FileReader.h"

using namespace std;

ModelSelectDialog::ModelSelectDialog()
    : m_translation_{ 0.f, 0.f, 0.f }, m_rotation_{ 0.f, 0.f, 0.f }, m_scaling_{ 1.f, 1.f, 1.f }
{
}

void ModelSelectDialog::CreateModelSelector(const float& delta_time)
{
    ImGui::Begin("Modeling");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS) With ImGui GetIO",
        1000.0f * delta_time, 1.f / delta_time);

    if (ImGui::Button("Select Modeling File"))
        ImGuiFileDialog::Instance()->OpenDialog("SelectModel", "Select Model File", ".fbx", ".");

    if (ImGuiFileDialog::Instance()->Display("SelectModel"))
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk())
        {

            shared_ptr<ModelData> model_data = make_shared<ModelData>();
            std::string file_path_name = ImGuiFileDialog::Instance()->GetCurrentPath() + "\\";
            model_data->file_name = ImGuiFileDialog::Instance()->GetCurrentFileName();
            model_data->mesh_data = FileReader::GetMeshDataFromFile(file_path_name, model_data->file_name);
            m_model_files_.emplace_back(model_data);
            m_on_file_added_.Broadcast(*(m_model_files_.back()));

            ImGuiFileDialog::Instance()->Close();
        }
        else
        {
            ImGuiFileDialog::Instance()->Close();
        }
    }

    ImGui::SameLine();
    if (ImGui::Button("Remove Selected File"))
    {
        if (selected_model_idx >= 0)
        {
            m_model_files_.erase(m_model_files_.begin() + selected_model_idx);
            m_on_file_deleted_.Broadcast(selected_model_idx);
            selected_model_idx = -1;
        }
    }

    ImVec2 listbox_size = ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing());
    if (ImGui::BeginListBox("##file select listbox", listbox_size))
    {
        for (int idx = 0; idx < m_model_files_.size(); idx++)
        {
            bool item_checked = (idx == selected_model_idx);

            ImGui::PushID(idx);
            if (ImGui::Checkbox("##check", &item_checked))
            {
                if (item_checked)
                {
                    selected_model_idx = idx;
                }
                else
                {
                    selected_model_idx = -1;
                }
            }
            else
            {

            }

            ImGui::SameLine();
            ImGui::Text("%s", m_model_files_[idx]->file_name.c_str());
            ImGui::PopID();
        }
        ImGui::EndListBox();
    }

    const bool& is_checked = (selected_model_idx >= 0);

    if (is_checked)
    {
        memcpy(&m_translation_[0], &m_model_files_[selected_model_idx]->model_translation[0], sizeof(float) * 3);
        memcpy(&m_rotation_[0], &m_model_files_[selected_model_idx]->model_rotation[0], sizeof(float) * 3);
        memcpy(&m_scaling_[0], &m_model_files_[selected_model_idx]->model_scaling[0], sizeof(float) * 3);
    }

    bool is_transformed = false;
    bool material_changed = false;

    ImGui::BeginDisabled(selected_model_idx < 0);
    ImGui::Text("Translation");
    ImGui::PushItemWidth(-FLT_MIN);
    if (ImGui::SliderFloat3("Translation", &m_translation_[0], -10.0f, 10.0f, "%.3f"))
    {
        memcpy(&m_model_files_[selected_model_idx]->model_translation[0], &m_translation_[0], sizeof(float) * 3);
        is_transformed |= true;
    }
    ImGui::PopItemWidth();

    ImGui::Text("Rotation");
    ImGui::PushItemWidth(-FLT_MIN);
    if (ImGui::SliderFloat3("Rotation", &m_rotation_[0], -2.f * 3.141592f, 2.f * 3.141592f, "%.3f"))
    {
        memcpy(&m_model_files_[selected_model_idx]->model_rotation[0], &m_rotation_[0], sizeof(float) * 3);
        is_transformed |= true;
    }
    ImGui::PopItemWidth();

    ImGui::Text("Scaling");
    ImGui::PushItemWidth(-FLT_MIN);
    if (ImGui::SliderFloat3("Scaling", &m_scaling_[0], 1.0f, 5.0f, "%.3f"))
    {
        memcpy(&m_model_files_[selected_model_idx]->model_scaling[0], &m_scaling_[0], sizeof(float) * 3);
        is_transformed |= true;
    }
    ImGui::PopItemWidth();

    if (is_transformed)
    {
        m_on_model_transformed.Broadcast(selected_model_idx, *(m_model_files_[selected_model_idx]));
    }

    // ===========================================================================================

    ImGui::Text("Diffuse");
    ImGui::PushItemWidth(-FLT_MIN);
    if (ImGui::SliderFloat("Diffuse", &m_material[0], 0, 3.f, "%.3f"))
    {
        m_model_files_[selected_model_idx]->model_material[0] = m_material[0];
        material_changed |= true;
    }
    ImGui::PopItemWidth();

    ImGui::Text("Specular");
    ImGui::PushItemWidth(-FLT_MIN);
    if (ImGui::SliderFloat("Specular", &m_material[1], 0, 3.f, "%.3f"))
    {
        m_model_files_[selected_model_idx]->model_material[1] = m_material[1];
        material_changed |= true;
    }
    ImGui::PopItemWidth();

    ImGui::Text("Shininess");
    ImGui::PushItemWidth(-FLT_MIN);
    if (ImGui::SliderFloat("Shininess", &m_material[2], 0.f, 20.f, "%.3f"))
    {
        m_model_files_[selected_model_idx]->model_material[2] = m_material[2];
        material_changed |= true;
    }
    ImGui::PopItemWidth();
    
    if (material_changed)
    {
        m_on_material_changed.Broadcast(selected_model_idx, *(m_model_files_[selected_model_idx]));
    }

    ImGui::EndDisabled();

    ImGui::End();
}
