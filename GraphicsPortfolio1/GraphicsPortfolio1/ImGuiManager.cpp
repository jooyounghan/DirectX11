
#include <directxtk/SimpleMath.h>

#include <Windows.h>
#include <atomic>

#include "ImGuiManager.h"
#include "ImGuiFileDialog.h"
#include "ImGuiFileDialogConfig.h"

using namespace std;
using namespace DirectX::SimpleMath;

ImGuiManager::ImGuiManager()
    : m_translation_{0.f, 0.f, 0.f}, m_rotation_{ 0.f, 0.f, 0.f }, m_scaling_{ 1.f, 1.f, 1.f }
{
}

ImGuiManager::~ImGuiManager()
{

}


bool ImGuiManager::InitImGui(HWND main_window, ComPtr<ID3D11Device> m_device, ComPtr<ID3D11DeviceContext> m_context)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    if (!ImGui_ImplDX11_Init(m_device.Get(), m_context.Get())) {
        return false;
    }

    if (!ImGui_ImplWin32_Init(main_window)) {
        return false;
    }


    return true;
}

void ImGuiManager::ReSetImGui()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiManager::RecordRendering(IN OUT atomic<float>& delta_time)
{
    delta_time.store(1.f / ImGui::GetIO().Framerate);

    ImGui_ImplDX11_NewFrame(); // GUI 프레임 시작
    ImGui_ImplWin32_NewFrame();

    CreateModelSelectFrame(delta_time.load());

    ImGui::Render(); // 렌더링할 것들 기록 끝
}

void ImGuiManager::Render()
{
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // GUI 렌더링
}

void ImGuiManager::CreateModelSelectFrame(const float& delta_time)
{
    ImGui::NewFrame();
    ImGui::Begin("Mdoeling");

    ImGui::Text("Average %.3f ms/frame (%.1f FPS) With ImGui GetIO",
        1000.0f * delta_time, 1.f / delta_time);

    if (ImGui::Button("Select Modeling File"))
        ImGuiFileDialog::Instance()->OpenDialog("SelectModel", "Select Model File", ".fbx", ".");

    if (ImGuiFileDialog::Instance()->Display("SelectModel"))
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string file_path_name = ImGuiFileDialog::Instance()->GetCurrentPath() + "\\";
            std::string file_name = ImGuiFileDialog::Instance()->GetCurrentFileName();


            m_model_files_.emplace_back(make_shared<ModelData>(file_path_name, file_name));
            m_on_file_added_.Broadcast(file_path_name, file_name);

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
        for (size_t index = 0; index < m_model_files_.size(); ++index)
        {
            if (m_model_files_[index] == m_selected_model_)
            {
                m_model_files_.erase(m_model_files_.begin() + index);
                m_on_file_deleted_.Broadcast(index);
                index--;

                ResetSelection();
            }
        }
    }

    ImGui::SameLine();
    HelpMarker("How?",
        "You can add and delete Modeling File(.fbx) from this scene.\n"
        "Also, You can make model's transfromation by checking box and\n"
        "  manipulating the each scroll bar(Rotating, etc...)");



    ImVec2 listbox_size = ImVec2(-FLT_MIN, 10 * ImGui::GetTextLineHeightWithSpacing());
    if (ImGui::BeginListBox("##file select listbox", listbox_size))
    {
        for (int idx = 0; idx < m_model_files_.size(); idx++)
        {
            bool item_checked = (m_model_files_[idx] == m_selected_model_);

            ImGui::PushID(idx);
            if (ImGui::Checkbox("##check", &item_checked))
            {
                if (item_checked)
                {
                    SetSelection(idx);
                }
                else
                {
                    ResetSelection();
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

    ImGui::BeginDisabled(m_selected_model_ == nullptr);
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

    if (is_checked && is_transformed)
    {
        m_on_model_transformed.Broadcast(selected_model_idx, m_translation_, m_rotation_, m_scaling_);
    }

    ImGui::PopItemWidth();
    ImGui::EndDisabled();

    ImGui::End();
}

void ImGuiManager::HelpMarker(const char* marker_text, const char* desc)
{
    ImGui::TextDisabled(marker_text);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

inline void ImGuiManager::SetSelection(const int& idx)
{
    selected_model_idx = idx;
    m_selected_model_ = m_model_files_[idx];
}

inline void ImGuiManager::ResetSelection()
{
    selected_model_idx = -1;
    m_selected_model_ = nullptr;
}
