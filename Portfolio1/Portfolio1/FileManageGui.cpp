#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>
#include "FileManageGui.h"
#include "ImGuiFileDialog\ImGuiFileDialog.h"

#include "FileLoader.h"

using namespace std;
using namespace ImGui;

FileManageGui::FileManageGui(
    Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
)
    : cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn), strCurrentPath("")
{
}

FileManageGui::~FileManageGui()
{
}

void FileManageGui::RenderGui()
{
	Begin("File Manager");
    SetFileDialog();
    SetLoadedFiles();
	End();
}

void FileManageGui::UpdateLoadedFiles(const string& strFilePathIn)
{
    //    static int iSelectedIdx = 0;
//    if (iSelectedIdx < upFileManager->vTexturesWithDirectory.size())
//    {
//        string pSelectedStr = FileManager::ConvertWCharToChar(upFileManager->vTexturesWithDirectory[iSelectedIdx].wstrDirectoryName);
//        if (ImGui::BeginCombo("Set Directories", pSelectedStr.c_str(), NULL))
//        {
//            for (size_t idx = 0; idx < upFileManager->vTexturesWithDirectory.size(); ++idx)
//            {
//                const bool is_selected = (iSelectedIdx == idx);
//                string pIndexedStr = FileManager::ConvertWCharToChar(upFileManager->vTexturesWithDirectory[idx].wstrDirectoryName);
//                if (ImGui::Selectable(pIndexedStr.c_str(), is_selected))
//                    iSelectedIdx = idx;
//
//                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
//                if (is_selected)
//                    ImGui::SetItemDefaultFocus();
//            }
//            ImGui::EndCombo();
//        }
//
//
//        ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
//        ImGui::BeginChild("FileTextureSet", ImVec2(ImGui::GetContentRegionAvail().x, 75.f), false, window_flags);
//        for (size_t idx = 0; idx < upFileManager->vTexturesWithDirectory[iSelectedIdx].sTextures.size(); ++idx)
//        {
//            Separator();
//            ID3D11ShaderResourceView* pIndexedSRV = upFileManager->vTexturesWithDirectory[iSelectedIdx].sTextures[idx].TextureSRV.Get();
//            Image(pIndexedSRV, ImVec2(60.f, 60.f));
//
//            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
//            {
//                ImGui::SetDragDropPayload("DND_DEMO_CELL", &(upFileManager->vTexturesWithDirectory[iSelectedIdx].sTextures[idx]), sizeof(Texture));
//                ImGui::EndDragDropSource();
//            }
//            SameLine();
//            TextEx(FileManager::ConvertWCharToChar(upFileManager->vTexturesWithDirectory[iSelectedIdx].sTextures[idx].TextureName).c_str(), (const char*)0, ImGuiTextFlags_::ImGuiTextFlags_NoWidthForLargeClippedText);
//        }
//        ImGui::EndChild();
//    }
}

void FileManageGui::SetFileDialog()
{
    if (ImGui::Button("Choose Directory"))
    {
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", nullptr, "."); ImGui::SameLine();
    }

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImGui::Text(strCurrentPath.c_str(), ImGui::GetContentRegionAvail().x);

    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            string strTempFilePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            if (strCurrentPath != strTempFilePath)
            {
                strCurrentPath = strTempFilePath;
                UpdateLoadedFiles(strTempFilePath);
            }
        }

        // close
        ImGuiFileDialog::Instance()->Close();
    }
}

void FileManageGui::SetLoadedFiles()
{
    //ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
    //ImVec2 imvec2ContentRegionAvail = GetContentRegionAvail();
    //ImGui::BeginChild("FileTextureSet", imvec2ContentRegionAvail, false, window_flags);
    //for (size_t idx = 0; idx < upFileManager->vTexturesWithDirectory[iSelectedIdx].sTextures.size(); ++idx)
    //{
    //    Separator();
    //    ID3D11ShaderResourceView* pIndexedSRV = upFileManager->vTexturesWithDirectory[iSelectedIdx].sTextures[idx].TextureSRV.Get();
    //    Image(pIndexedSRV, ImVec2(60.f, 60.f));

    //    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
    //    {
    //        ImGui::SetDragDropPayload("DND_DEMO_CELL", &(upFileManager->vTexturesWithDirectory[iSelectedIdx].sTextures[idx]), sizeof(Texture));
    //        ImGui::EndDragDropSource();
    //    }
    //    SameLine();
    //    TextEx(FileManager::ConvertWCharToChar(upFileManager->vTexturesWithDirectory[iSelectedIdx].sTextures[idx].TextureName).c_str(), (const char*)0, ImGuiTextFlags_::ImGuiTextFlags_NoWidthForLargeClippedText);
    //}
    //ImGui::EndChild();
}
