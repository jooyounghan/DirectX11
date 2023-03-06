#include <directxtk/SimpleMath.h>

#include "LightSelectDialog.h"

using namespace std;
using DirectX::SimpleMath::Vector3;

LightSelectDialog::LightSelectDialog()
    : m_selected_light_type_{ false, false, false }, m_ligth_position_{ 0.f, 0.f, 0.f },
    m_light_power_(0.f), m_ligth_direction_{0.f, 0.f, 0.f},
    m_ligth_color_{ 0.f, 0.f, 0.f }, m_fall_off_{ 0.f, 0.f }, m_spot_power_(0),
    m_light_translation_{ 0.f, 0.f, 0.f }, m_light_constant_data_(nullptr)
{
}

void LightSelectDialog::SwitchOffLightType()
{
    is_directional_light_ = false;
    is_point_light_ = false;
    is_spot_light_ = false;
}

void LightSelectDialog::SwitchOffSelectedLight()
{
    m_selected_light_idx_ = -1;
}

const bool LightSelectDialog::IsDirectionalLightReady(LightConstantData* selected_light_constant_data)
{
    return selected_light_constant_data ? selected_light_constant_data->light_type == 0 : is_directional_light_;
}

const bool LightSelectDialog::IsPointLightReady(LightConstantData* selected_light_constant_data)
{
    return selected_light_constant_data ? selected_light_constant_data->light_type == 1 : is_point_light_;
}

const bool LightSelectDialog::IsSpotLightReady(LightConstantData* selected_light_constant_data)
{
    return selected_light_constant_data ? selected_light_constant_data->light_type == 2 : is_spot_light_;
}

const bool LightSelectDialog::IsCreatingLight()
{
    return is_directional_light_ || is_point_light_ || is_spot_light_;
}

const bool LightSelectDialog::IsLightSelected()
{
    return m_selected_light_idx_ >= 0;
}

void LightSelectDialog::CreateLightSelector(const float& delta_time)
{
    ImGui::Begin("Lighting");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS) With ImGui GetIO",
        1000.0f * delta_time, 1.f / delta_time);

    float window_width = ImGui::GetWindowWidth();

    if (ImGui::Checkbox("Directional Light", &is_directional_light_))
    {
        is_point_light_ = false;
        is_spot_light_ = false;
    }
    ImGui::SameLine();
    if (ImGui::Checkbox("Point Light", &is_point_light_))
    {
        is_directional_light_ = false;
        is_spot_light_ = false;
    }
    ImGui::SameLine();
    if (ImGui::Checkbox("Spot Light", &is_spot_light_))
    {
        is_directional_light_ = false;
        is_point_light_ = false;
    }

    const bool& is_creating_light = IsCreatingLight();
    const bool& is_light_selected = IsLightSelected();
    if (is_creating_light) SwitchOffSelectedLight();
    LightConstantData* selected_light_constant_data = m_selected_light_idx_ < 0 ? nullptr : m_light_constant_data_ + m_selected_light_idx_;

    ImGui::BeginDisabled(!(is_creating_light || is_light_selected));
    if (selected_light_constant_data) ImGui::ColorEdit3("Light Color", &selected_light_constant_data->light_color.x);
    else ImGui::ColorEdit3("Light Color", &m_ligth_color_[0]);
    ImGui::EndDisabled();

    ImGui::BeginDisabled(!(IsPointLightReady(selected_light_constant_data) || IsSpotLightReady(selected_light_constant_data)));
    if (selected_light_constant_data) ImGui::SliderFloat3("Light Position", &selected_light_constant_data->position.x, m_vmin_, m_vmax_);
    else ImGui::SliderFloat3("Light Position", &m_ligth_position_[0], m_vmin_, m_vmax_);
    ImGui::EndDisabled();

    ImGui::BeginDisabled(!(IsDirectionalLightReady(selected_light_constant_data) || IsSpotLightReady(selected_light_constant_data)));
    if (selected_light_constant_data) ImGui::SliderFloat3("Light Direction", &selected_light_constant_data->direction.x, -1.f, 1.f);
    else ImGui::SliderFloat3("Light Direction", &m_ligth_direction_[0], -1.f, 1.f);
    ImGui::EndDisabled();

    ImGui::BeginDisabled(!(is_creating_light || is_light_selected));
    if (selected_light_constant_data) ImGui::SliderFloat("Light Power", &selected_light_constant_data->light_power, 0.f, 1.f);
    else ImGui::SliderFloat("Light Power", &m_light_power_, 0.f, 1.f);
    ImGui::EndDisabled();

    ImGui::BeginDisabled(!(IsPointLightReady(selected_light_constant_data) || IsSpotLightReady(selected_light_constant_data)));
    if (selected_light_constant_data)
    {
        ImGui::SliderScalar("Fall Off Start", ImGuiDataType_Float, &selected_light_constant_data->fall_off_start, &m_vzero_, &m_vmax_, "%.3f");
        ImGui::SliderScalar("Fall Off End", ImGuiDataType_Float, &selected_light_constant_data->fall_off_end, &m_vzero_, &m_vmax_, "%.3f");
    }
    else
    {
        ImGui::SliderScalar("Fall Off Start", ImGuiDataType_Float, &m_fall_off_[0], &m_vzero_, &m_vmax_, "%.3f");
        ImGui::SliderScalar("Fall Off End", ImGuiDataType_Float, &m_fall_off_[1], &m_vzero_, &m_vmax_, "%.3f");
    }
    ImGui::EndDisabled();

    ImGui::BeginDisabled(!IsSpotLightReady(selected_light_constant_data));
    if (selected_light_constant_data) ImGui::SliderScalar("Spot Power", ImGuiDataType_Float, &selected_light_constant_data->spot_power, &m_vzero_, &m_vmax_, "%.3f");
    else ImGui::SliderScalar("Spot Power", ImGuiDataType_Float, &m_spot_power_, &m_vzero_, &m_vmax_, "%.3f");
    ImGui::EndDisabled();

    ImGui::Separator();

    ImGui::BeginDisabled(!(is_creating_light));
    const float& add_light_btn_pos = (ImGui::GetWindowWidth() - 0.4f * window_width * 2.f) / 3.f;
    ImGui::SetCursorPosX(add_light_btn_pos);
    if (ImGui::Button("Add Light", ImVec2(0.4f * window_width, 0)) && (m_light_cnt_ < MAX_LIGHT_NUM))
    {
        LightConstantData light_data;
        if (is_directional_light_)
        {
            light_data = Light::CreateDriectionalLightData(
                Vector4(m_ligth_color_[0], m_ligth_color_[1], m_ligth_color_[2], 1.f),
                Vector3(m_ligth_position_[0], m_ligth_position_[1], m_ligth_position_[2]),
                Vector3(m_ligth_direction_[0], m_ligth_direction_[1], m_ligth_direction_[2]),
                m_light_power_);
        }
        else if (is_point_light_)
        {
            light_data = Light::CreatePointLightData(
                Vector4(m_ligth_color_[0], m_ligth_color_[1], m_ligth_color_[2], 1.f),
                Vector3(m_ligth_position_[0], m_ligth_position_[1], m_ligth_position_[2]),
                m_light_power_, m_fall_off_[0], m_fall_off_[1]);
        }
        else if (is_spot_light_)
        {
            light_data = Light::CreateSpotLightData(
                Vector4(m_ligth_color_[0], m_ligth_color_[1], m_ligth_color_[2], 1.f),
                Vector3(m_ligth_position_[0], m_ligth_position_[1], m_ligth_position_[2]),
                m_light_power_, m_fall_off_[0], m_fall_off_[1],
                m_spot_power_);
        }
        else;

        m_light_cnt_++;
        m_on_light_added_.Broadcast(light_data, m_light_constant_data_);
    }
    ImGui::EndDisabled();

    ImGui::BeginDisabled(m_selected_light_idx_ < 0);
    ImGui::SameLine();
    ImGui::SetCursorPosX(add_light_btn_pos * 2.f + 0.4f * window_width);
    if (ImGui::Button("Delete Light", ImVec2(0.4f * window_width, 0)))
    {
        if (m_selected_light_idx_ >= 0)
        {
            m_light_cnt_--;
            m_on_light_deleted_.Broadcast(m_selected_light_idx_);
            m_selected_light_idx_ = -1;
        }
    }
    ImGui::EndDisabled();

    static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable;

    const int& row_num = 3;
    if (ImGui::BeginTable("Ligths Status", row_num, flags))
    {
        ImGui::TableSetupColumn("Select");
        ImGui::TableSetupColumn("Light Type");
        ImGui::TableSetupColumn("Color");

        ImGui::TableHeadersRow();
        for (int row = 0; row < m_light_cnt_; row++)
        {
            ImGui::TableNextRow();

            bool light_checked = (row == m_selected_light_idx_);

            LightConstantData* row_light_data = m_light_constant_data_ + row;

            for (int column = 0; column < row_num; column++)
            {
                ImGui::TableSetColumnIndex(column);

                switch (column)
                {
                case GridProp::Selected:
                    ImGui::PushID(row);
                    if (ImGui::Checkbox("##check", &light_checked))
                    {
                        if (light_checked)
                        {
                            m_selected_light_idx_ = row;
                        }
                        else
                        {
                            m_selected_light_idx_ = -1;
                        }
                    }
                    ImGui::PopID();
                    break;
                case GridProp::LightType:
                    switch (row_light_data->light_type)
                    {
                    case LightType::Directional:
                        ImGui::TextUnformatted("Directional");
                        break;
                    case LightType::Point:
                        ImGui::TextUnformatted("Point");
                        break;
                    case LightType::Spot:
                        ImGui::TextUnformatted("Spot");
                        break;
                    }
                    break;
                case GridProp::Color:
                    ImGui::ColorButton("SelectedColor", ImVec4(row_light_data->light_color.x, row_light_data->light_color.y, row_light_data->light_color.z, row_light_data->light_color.w));
                    break;
                }
            }
        }
        ImGui::EndTable();

        if (m_selected_light_idx_ >= 0) SwitchOffLightType();
    }
    ImGui::End();
}
