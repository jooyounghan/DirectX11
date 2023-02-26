#include "LightSelectDialog.h"

LightSelectDialog::LightSelectDialog()
    : m_selected_light_type_{ false, false, false }, m_ligth_position_{ 0.f, 0.f, 0.f },
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
    ImGui::SliderFloat3("Light Position", (float*)&m_ligth_position_, m_vmin_, m_vmax_);
    ImGui::ColorEdit3("Light Color", (float*)&m_ligth_color_);
    ImGui::EndDisabled();

    ImGui::BeginDisabled(!(is_point_light_ || is_spot_light_));
    ImGui::SliderScalar("Fall Off Start", ImGuiDataType_Float, &m_fall_off_[0], &m_vmin_, &m_vmax_, "%.3f");
    ImGui::SliderScalar("Fall Off End", ImGuiDataType_Float, &m_fall_off_[1], &m_vmin_, &m_vmax_, "%.3f");
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

    }
    ImGui::EndDisabled();

    ImGui::BeginDisabled(!is_light_on);
    ImGui::SameLine();
    ImGui::SetCursorPosX(add_light_btn_pos * 2.f + 0.4f * window_width);
    if (ImGui::Button("Delete Light", ImVec2(0.4f * window_width, 0)))
    {

    }
    ImGui::EndDisabled();

    static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable;

    if (ImGui::BeginTable("Ligths Status", 5, flags))
    {
        ImGui::TableSetupColumn("Light Type");
        ImGui::TableSetupColumn("Position");
        ImGui::TableSetupColumn("Direction");
        ImGui::TableSetupColumn("Fall Off Start/End");
        ImGui::TableSetupColumn("Spot Power");
        ImGui::TableHeadersRow();

        for (int row = 0; row < lights.size(); row++)
        {
            ImGui::TableNextRow();
            for (int column = 0; column < 5; column++)
            {
                ImGui::TableSetColumnIndex(column);
                switch (column)
                {
                case GridProp::LightType: // Light Type
                    switch (lights[row]->light_type)
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

                    break;
                case GridProp::Direction:

                    break;
                case GridProp::FallOff: 

                    break;
                case GridProp::SpotPower:

                    break;
                }
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();
}
