#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>
#include "FileManageGui.h"
#include "FileManager.h"

#include <codecvt>

using namespace std;
using namespace ImGui;

FileManageGui::FileManageGui(std::unique_ptr<FileManager>& upFileManagerIn)
	: upFileManager(upFileManagerIn)
{
}

FileManageGui::~FileManageGui()
{
}

void FileManageGui::RenderGui()
{
	Begin("File Manager");
    if (CollapsingHeader("Set Textures"))
    {
        SetTextureMenu();
    }
	End();
}

void FileManageGui::SetTextureMenu()
{
    static int iSelectedIdx = 0;
    if (iSelectedIdx < upFileManager->vTexturesWithDirectory.size())
    {
        string pSelectedStr = FileManager::ConvertWCharToChar(upFileManager->vTexturesWithDirectory[iSelectedIdx].wstrDirectoryName);
        if (ImGui::BeginCombo("Set Directories", pSelectedStr.c_str(), NULL))
        {
            for (size_t idx = 0; idx < upFileManager->vTexturesWithDirectory.size(); ++idx)
            {
                const bool is_selected = (iSelectedIdx == idx);
                string pIndexedStr = FileManager::ConvertWCharToChar(upFileManager->vTexturesWithDirectory[idx].wstrDirectoryName);
                if (ImGui::Selectable(pIndexedStr.c_str(), is_selected))
                    iSelectedIdx = idx;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        for (size_t idx = 0; idx < upFileManager->vTexturesWithDirectory[iSelectedIdx].sTextures.size(); ++idx)
        {
            BeginGroup();
            ID3D11ShaderResourceView* pIndexedSRV = upFileManager->vTexturesWithDirectory[iSelectedIdx].sTextures[idx].TextureSRV.Get();
            Image(pIndexedSRV, ImVec2(60.f, 60.f));

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
            {
                ImGui::SetDragDropPayload("DND_DEMO_CELL", &(upFileManager->vTexturesWithDirectory[iSelectedIdx].sTextures[idx]), sizeof(Texture));
                ImGui::EndDragDropSource();
            }
            EndGroup();
            SameLine();
        }
    }
}
