
#include <Windows.h>

#include "ImGuiManager.h"
#include "ImGuiFileDialog.h"
#include "ImGuiFileDialogConfig.h"

using namespace std;

ImGuiManager::ImGuiManager()
    : m_translation_{0.f, 0.f, 0.f}, m_rotation_{ 0.f, 0.f, 0.f }, m_scaling_{ 0.f, 0.f, 0.f },
    m_model_transfrom_flag_(false)
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


            m_model_files_.emplace_back(make_shared<ModelData>(false, file_path_name, file_name));
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
            if (m_model_files_[index]->is_checked == true)
            {
                m_model_files_.erase(m_model_files_.begin() + index);
                m_on_file_deleted_.Broadcast(index);
                index--;
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
            bool item_checked = m_model_files_[idx] == m_selected_model_;
            ImGui::PushID(idx);
            if (ImGui::Checkbox("##check", &item_checked))
            {
                if (item_checked)
                {
                    m_selected_model_ = m_model_files_[idx];
                }
                else
                {
                    m_selected_model_ = nullptr;
                }
           }
            else
            {
                bool test = true;
            }

            ImGui::SameLine();
            ImGui::Text("%s", m_model_files_[idx]->file_name.c_str());
            ImGui::PopID();
        }
        ImGui::EndListBox();
    }

// Checkbox가 바뀌면 초기화 슬라이드 바 초기화 시키기

    ImGui::BeginDisabled(m_selected_model_ == nullptr);
    ImGui::Text("Translation");
    ImGui::PushItemWidth(-FLT_MIN);
    if (ImGui::SliderFloat3("Translation", &m_translation_[0], 0.0f, 10.0f, "%.3f"))
    {

    }
    ImGui::PopItemWidth();

    ImGui::Text("Rotation");
    ImGui::PushItemWidth(-FLT_MIN);
    if (ImGui::SliderFloat3("Rotation", &m_rotation_[0], 0.0f, 10.0f, "%.3f"))
    {

    }
    ImGui::PopItemWidth();

    ImGui::Text("Scaling");
    ImGui::PushItemWidth(-FLT_MIN);
    if (ImGui::SliderFloat3("Scaling", &m_scaling_[0], 0.0f, 10.0f, "%.3f"))
    {

    }
    float* test1 = m_translation_;
    float* test2 = m_rotation_;
    float* test3 = m_scaling_;
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
