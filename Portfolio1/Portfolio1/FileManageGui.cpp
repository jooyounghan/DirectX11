#include "FileManageGui.h"
#include "ImGuiFileDialog\ImGuiFileDialog.h"

#include "FileLoader.h"
#include "FileInterface.h"

#include "UnknownFile.h"
#include "ModelTextureFile.h"
#include "DDSFile.h"
#include "StringHelper.h"

#include "ID3D11Helper.h"

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
            StringHelper::IsStrSame(&isImage, sExtensionName, ".jpg", ".png", ".exr", ".dds");
            if (isImage)
            {
                LoadImages(sExtensionName, entry.path().string());
            }
            else
            {

            }
        }
    }
}

void FileManageGui::LoadImages(const string& strExtention, const string& strFilePathIn)
{
    UINT uiWidth, uiHeight, uiChannel;
    uint8_t* ucImageRawData = nullptr;

    if (mapNameToFiles.find(strFilePathIn) == mapNameToFiles.end())
    {
        shared_ptr<FileInterface> spFileInterface;
        if (strExtention == ".exr")
        {
            ucImageRawData = FileLoader::LoadFileWithOpenEXR(strFilePathIn.c_str(), &uiWidth, &uiHeight, &uiChannel);
            spFileInterface = make_shared<ModelTextureFile>(cpDevice, cpDeviceContext, strFilePathIn, uiWidth, uiHeight, ucImageRawData, DXGI_FORMAT_R16G16B16A16_FLOAT);
        }
        else if (strExtention == ".dds")
        {
            spFileInterface = make_shared<DDSFile>(cpDevice, cpDeviceContext, strFilePathIn, strFilePathIn.find("Brdf") == string::npos);
        }
        else
        {
            ucImageRawData = FileLoader::LoadFileWithStbi(strFilePathIn.c_str(), &uiWidth, &uiHeight, &uiChannel);
            spFileInterface = make_shared<ModelTextureFile>(cpDevice, cpDeviceContext, strFilePathIn, uiWidth, uiHeight, ucImageRawData);
        }
        vLoadedFiles.emplace_back(spFileInterface);
        FileLoader::FreeLoadedFileData(ucImageRawData);
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

        if (pIndexedSRV != nullptr)
        {
            BeginGroup();
            Image(pIndexedSRV, ImVec2(60.f, 60.f));
            SameLine();
            TextEx(loadedFile->strFileName.c_str(), (const char*)0, ImGuiTextFlags_::ImGuiTextFlags_NoWidthForLargeClippedText);
            EndGroup();
    
            const FileType& eFileType = loadedFile->GetFileType();
            if (eFileType == FileType::ModelTextureFileType)
            {
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                {
                    ImGui::SetDragDropPayload("Texture2D", &loadedFile, sizeof(shared_ptr<ModelTextureFile>));
                    ImGui::EndDragDropSource();
                }
            }
            else if (eFileType == FileType::CubeMapTextureType)
            {
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                {
                    ImGui::SetDragDropPayload("CubeMap", &loadedFile, sizeof(shared_ptr<DDSFile>));
                    ImGui::EndDragDropSource();
                }
            }
            else;
        }
    }
    ImGui::EndChild();
}
