#include <directxtk/SimpleMath.h>

#include "LightSelectDialog.h"

using namespace std;
using DirectX::SimpleMath::Vector3;

LightSelectDialog::LightSelectDialog()
    : m_selected_light_type_{ false, false, false }, m_ligth_position_{ 0.f, 0.f, 0.f },
    m_light_power_(0.f), m_ligth_direction_{0.f, 0.f, 0.f},
    m_ligth_color_{ 0.f, 0.f, 0.f }, m_fall_off_{ 0.f, 0.f }, m_spot_power_(0),
    m_light_translation_{ 0.f, 0.f, 0.f }
{
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

    const bool& is_light_on = is_directional_light_ || is_point_light_ || is_spot_light_;

    ImGui::BeginDisabled(!is_light_on);
    ImGui::ColorEdit3("Light Color", &m_ligth_color_[0]);
    ImGui::SliderFloat3("Light Position", &m_ligth_position_[0], m_vmin_, m_vmax_);
    ImGui::EndDisabled();

    ImGui::BeginDisabled(!is_directional_light_);
    ImGui::SliderFloat3("Light Direction", &m_ligth_direction_[0], -1.f, 1.f);
    ImGui::EndDisabled();

    ImGui::BeginDisabled(!is_light_on);
    ImGui::SliderFloat("Light Power", &m_light_power_, 0.f, 1.f);
    ImGui::EndDisabled();

    ImGui::BeginDisabled(!(is_point_light_ || is_spot_light_));
    ImGui::SliderScalar("Fall Off Start", ImGuiDataType_Float, &m_fall_off_[0], &m_vzero_, &m_vmax_, "%.3f");
    ImGui::SliderScalar("Fall Off End", ImGuiDataType_Float, &m_fall_off_[1], &m_vzero_, &m_vmax_, "%.3f");
    ImGui::EndDisabled();

    ImGui::BeginDisabled(!is_spot_light_);
    ImGui::SliderScalar("Spot Power", ImGuiDataType_Float, &m_spot_power_, &m_vzero_, &m_vmax_, "%.3f");
    ImGui::EndDisabled();

    ImGui::Separator();

    ImGui::BeginDisabled(!is_light_on);
    const float& add_light_btn_pos = (ImGui::GetWindowWidth() - 0.4f * window_width * 2.f) / 3.f;
    ImGui::SetCursorPosX(add_light_btn_pos);
    if (ImGui::Button("Add Light", ImVec2(0.4f * window_width, 0)))
    {
        LightConstantData light_data;
        if (is_directional_light_)
        {
            light_data = Light::CreateDriectionalLightData(
                Vector3(m_ligth_color_[0], m_ligth_color_[1], m_ligth_color_[2]),
                Vector3(m_ligth_position_[0], m_ligth_position_[1], m_ligth_position_[2]),
                Vector3(m_ligth_direction_[0], m_ligth_direction_[1], m_ligth_direction_[2]),
                m_light_power_);
        }
        else if (is_point_light_)
        {
            light_data = Light::CreatePointLightData(
                Vector3(m_ligth_color_[0], m_ligth_color_[1], m_ligth_color_[2]),
                Vector3(m_ligth_position_[0], m_ligth_position_[1], m_ligth_position_[2]),
                m_light_power_, m_fall_off_[0], m_fall_off_[1]);
        }
        else if (is_spot_light_)
        {
            light_data = Light::CreateSpotLightData(
                Vector3(m_ligth_color_[0], m_ligth_color_[1], m_ligth_color_[2]),
                Vector3(m_ligth_position_[0], m_ligth_position_[1], m_ligth_position_[2]),
                m_light_power_, m_fall_off_[0], m_fall_off_[1],
                m_spot_power_);
        }
        else;

        LightConstantData* inserted_light_constant_data;
        m_on_light_added_.Broadcast(light_data, inserted_light_constant_data);
        m_light_constant_data_.push_back(inserted_light_constant_data);

    }
    ImGui::EndDisabled();

    ImGui::BeginDisabled(!is_light_on);
    ImGui::SameLine();
    ImGui::SetCursorPosX(add_light_btn_pos * 2.f + 0.4f * window_width);
    if (ImGui::Button("Delete Light", ImVec2(0.4f * window_width, 0)))
    {
        if (m_selected_light_idx_ >= 0)
        {
            m_light_constant_data_.erase(m_light_constant_data_.begin() + m_selected_light_idx_);
            m_on_light_deleted_.Broadcast(m_selected_light_idx_);
            m_selected_light_idx_ = -1;
        }
    }
    ImGui::EndDisabled();

    static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable;

    const int& row_num = 8;
    if (ImGui::BeginTable("Ligths Status", row_num, flags))
    {
        ImGui::TableSetupColumn("Select");
        ImGui::TableSetupColumn("Light Type");
        ImGui::TableSetupColumn("Color");
        ImGui::TableSetupColumn("Position");
        ImGui::TableSetupColumn("Direction");
        ImGui::TableSetupColumn("Light Power");
        ImGui::TableSetupColumn("Fall Off Start/End");
        ImGui::TableSetupColumn("Spot Power");

        ImGui::TableHeadersRow();
        for (int row = 0; row < m_light_constant_data_.size(); row++)
        {
            ImGui::TableNextRow();
            bool light_checked = (row == m_selected_light_idx_);
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
                    switch (m_light_constant_data_[row]->light_type)
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
                case GridProp::Position:
                    ImGui::Text("X : %.3f\tY : %.3f\tZ : %.3f", m_light_constant_data_[row]->position.x, m_light_constant_data_[row]->position.y, m_light_constant_data_[row]->position.z);
                    break;
                case GridProp::Direction:
                    switch (m_light_constant_data_[row]->light_type)
                    {
                    case LightType::Directional:
                        ImGui::Text("X : %.3f\tY : %.3f\tZ : %.3f", m_light_constant_data_[row]->direction.x, m_light_constant_data_[row]->direction.y, m_light_constant_data_[row]->direction.z);
                        break;
                    case LightType::Point:
                    case LightType::Spot:
                        ImGui::TextUnformatted("-");
                        break;
                    }
                    break;
                case GridProp::Color:
                    ImGui::Text("X : %.3f\tY : %.3f\tZ : %.3f", m_light_constant_data_[row]->light_color.x, m_light_constant_data_[row]->light_color.y, m_light_constant_data_[row]->light_color.z);
                    break;
                case GridProp::LightPower:
                    ImGui::Text("%.3f", m_light_constant_data_[row]->light_power);
                    break;
                case GridProp::FallOff:
                    switch (m_light_constant_data_[row]->light_type)
                    {
                    case LightType::Directional:
                        ImGui::TextUnformatted("-");
                        break;
                    case LightType::Point:
                    case LightType::Spot:
                        ImGui::Text("Start : %.3f\tEnd : %.3f", m_light_constant_data_[row]->fall_off_start, m_light_constant_data_[row]->fall_off_end);
                        break;
                    }
                    break;
                case GridProp::SpotPower:
                    switch (m_light_constant_data_[row]->light_type)
                    {
                    case LightType::Directional:
                    case LightType::Point:
                        ImGui::TextUnformatted("-");
                        break;
                    case LightType::Spot:
                        ImGui::Text("%.3f", m_light_constant_data_[row]->spot_power);
                        break;
                    }
                    break;
                }
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();
}
