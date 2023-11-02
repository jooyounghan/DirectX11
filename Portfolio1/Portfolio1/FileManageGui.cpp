#include "FileManageGui.h"
#include "ImGuiFileDialog\ImGuiFileDialog.h"

#include "FileLoader.h"
#include "FileInterface.h"
#include "UnknownFile.h"
#include "ModelTexture.h"
#include "Filter.h"

#include "ID3D11Helper.h"

#include "FileLoader.h"

#include <filesystem>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h>
#include <locale.h>


using namespace std;
using namespace ImGui;

FileManageGui::FileManageGui( 
    Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
)
    : cpDevice(cpDeviceIn), cpDeviceContext(cpDeviceContextIn), strCurrentPath("")
{
    std::setlocale(LC_ALL, ".UTF8");
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

void FileManageGui::UpdateLoadedFiles(const wstring& wstrFilePathIn)
{
    filesystem::path texturePath(wstrFilePathIn);
    if (filesystem::exists(texturePath) && filesystem::is_directory(texturePath))
    {
        for (auto& loadedFile : vLoadedFiles)
        {
            if (loadedFile.use_count() > 1)
            {
                if (mapNameToFiles.find(loadedFile->strFileName) == mapNameToFiles.end())
                {
                    mapNameToFiles.emplace(loadedFile->strFileName, loadedFile);
                }
            }
        }
        vLoadedFiles.clear();

        filesystem::directory_iterator dirIter(texturePath);
        for (auto& entry : dirIter)
        {
            string sExtensionName = entry.path().extension().string();
            
            bool isImage = false;
            Filter::IsStrSame(&isImage, sExtensionName, ".jpg", ".png");
            if (isImage)
            {
                LoadImages(entry.path().string());
            }
            else
            {
                // LoadUnknownFIle
            }
        }
    }
}

void FileManageGui::LoadImages(const string& strFilePathIn)
{
    int iWidth, iHeight, iChannel;
    uint8_t* ucImageRawData;

    if (mapNameToFiles.find(strFilePathIn) == mapNameToFiles.end())
    {
        ucImageRawData = FileLoader::stbi_load(strFilePathIn.c_str(), &iWidth, &iHeight, &iChannel, 0);
        FileLoader::ExtendChannel(ucImageRawData, iWidth, iHeight, iChannel, 4);
        shared_ptr<ModelTexture> spModelTexture = make_shared<ModelTexture>(cpDevice, cpDeviceContext, strFilePathIn, (UINT)iWidth, (UINT)iHeight, ucImageRawData);
        vLoadedFiles.emplace_back(spModelTexture);
        FileLoader::stbi_free(ucImageRawData);
    }
    else
    {
        vLoadedFiles.emplace_back(mapNameToFiles[strFilePathIn]);
    }
}

void FileManageGui::SetFileDialog()
{
    if (ImGui::Button("Choose Directory"))
    {
        ImGuiFileDialog::Instance()->OpenDialog("FileDescriptor", "Choose File", nullptr, ".");
    }
    ImGui::SameLine();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImGui::Text(strCurrentPath.c_str(), ImGui::GetContentRegionAvail().x);

    if (ImGuiFileDialog::Instance()->Display("FileDescriptor"))
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            string strUTF8FilePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            if (strCurrentPath != strUTF8FilePath)
            {
                strCurrentPath = strUTF8FilePath;
                UpdateLoadedFiles(FileLoader::ConvertUTF8ToUniCode(strCurrentPath));
            }
        }
        // close
        ImGuiFileDialog::Instance()->Close();
    }
}

void FileManageGui::SetLoadedFiles()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar;
    ImVec2 imvec2ContentRegionAvail = GetContentRegionAvail();
    ImGui::BeginChild("LoadedFiles", imvec2ContentRegionAvail, false, window_flags);

    for (auto& loadedFile : vLoadedFiles)
    {
        Separator();
        ID3D11ShaderResourceView* pIndexedSRV = loadedFile->cpFileThumbNailSRV.Get();
        Image(pIndexedSRV, ImVec2(60.f, 60.f));
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            ImGui::SetDragDropPayload("DND_DEMO_CELL", &loadedFile, sizeof(shared_ptr<ModelTexture>));
            ImGui::EndDragDropSource();
        }
        SameLine();
        TextEx(loadedFile->strFileName.c_str(), (const char*)0, ImGuiTextFlags_::ImGuiTextFlags_NoWidthForLargeClippedText);
    }
    ImGui::EndChild();
}
