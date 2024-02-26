#include "FileManipulator.h"
#include "ImGuiFileDialog.h"
#include "IFile.h"

#include "DDSImageFile.h"
#include "NormalImageFile.h"

#include "DirectXDevice.h"

#include <filesystem>
#include <locale.h>

using namespace std;
using namespace ImGui;

FileManipulator::FileManipulator()
{
    std::setlocale(LC_ALL, ".UTF8");
}

FileManipulator::~FileManipulator()
{
}

void FileManipulator::PopAsDialog()
{
    Begin("File Loader");
 
    ChooseFiles();
    DisplayFiles();

    End();
}

bool FileManipulator::IsGuiAvailable()
{
    return true;
}

void FileManipulator::ChooseFiles()
{
    Text("Load Files");
    SameLine();

    if (ImGui::Button("Choose Directory"))
    {
        ImGuiFileDialog::Instance()->OpenDialog("FileDescriptor", "Choose File", nullptr, ".");
    }

    if (ImGuiFileDialog::Instance()->Display("FileDescriptor"))
    {
        // action if OK
        if (ImGuiFileDialog::Instance()->IsOk())
        {

            std::string strUTF8FilePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            if (strCurrentPath != strUTF8FilePath)
            {
                strCurrentPath = strUTF8FilePath;
                LoadFiles(FileLoader::ConvertUTF8ToUniCode(strCurrentPath));
            }
        }
        // close
        ImGuiFileDialog::Instance()->Close();
    }
}

void FileManipulator::DisplayFiles()
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysVerticalScrollbar;
    ImVec2 imvec2ContentRegionAvail = GetContentRegionAvail();
    ImGui::BeginChild("LoadedFiles", imvec2ContentRegionAvail, false, window_flags);

    for (auto& loadedFile : vLoadedFiles)
    {
        Separator();
        loadedFile->AcceptFileAsList(this, loadedFile);
    }
    ImGui::EndChild();
}

void FileManipulator::LoadFiles(const std::wstring& wstrFilePathIn)
{
    filesystem::path texturePath(wstrFilePathIn);
    if (filesystem::exists(texturePath) && filesystem::is_directory(texturePath))
    {
        vLoadedFiles.clear();

        filesystem::directory_iterator dirIter(texturePath);
        for (auto& entry : dirIter)
        {
            UINT uiWidth, uiHeight, uiChannel;

            const string strFileName = entry.path().filename().string();
            const string strExtention = entry.path().extension().string();
            const string strFilePathIn = entry.path().parent_path().string();
            
            bool isImage = false;
            FileLoader::IsStrSame(&isImage, strExtention, ".jpg", ".png", ".exr", ".dds");
            shared_ptr<IFile> spFileInterface;
            if (isImage)
            {
                bool isNeedLoad = false;
                uint8_t* ucImageRawData = nullptr;
                spFileInterface = fileLoader.LoadImageFile(
                    strExtention, strFilePathIn, strFileName, &uiWidth, &uiHeight, &uiChannel
                );

            }
            bool isObject = false;
            FileLoader::IsStrSame(&isObject, strExtention, ".fbx", ".gltf");
            if(isObject)
            {
                spFileInterface = fileLoader.LoadModelFile(strExtention, strFilePathIn, strFileName);
            }

            if (spFileInterface.get() != nullptr)
                vLoadedFiles.emplace_back(spFileInterface);
        }
    }
}

void FileManipulator::ShowAsList(NormalImageFile& imageFile, shared_ptr<IFile>& spFile)
{
    ID3D11ShaderResourceView* pIndexedSRV = imageFile.cpThumbnailSRV.Get();

    if (pIndexedSRV != nullptr)
    {
        BeginGroup();
        Image(pIndexedSRV, ImVec2(60.f, 60.f));
        SameLine();
        TextEx(imageFile.GetFileName().c_str(), (const char*)0, ImGuiTextFlags_::ImGuiTextFlags_NoWidthForLargeClippedText);
        EndGroup();

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            ImGui::SetDragDropPayload("Texture2D", &spFile, sizeof(shared_ptr<NormalImageFile>));
            ImGui::EndDragDropSource();
        }

    }
}

void FileManipulator::ShowAsList(DDSImageFile& imageFile, shared_ptr<IFile>& spFile)
{
    ID3D11ShaderResourceView* pIndexedSRV = imageFile.cpThumbnailSRV.Get();

    if (pIndexedSRV != nullptr)
    {
        BeginGroup();
        Image(pIndexedSRV, ImVec2(60.f, 60.f));
        SameLine();
        TextEx(imageFile.GetFileName().c_str(), (const char*)0, ImGuiTextFlags_::ImGuiTextFlags_NoWidthForLargeClippedText);
        EndGroup();

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            D3D11_TEXTURE2D_DESC desc;
            imageFile.GetTexture()->GetDesc(&desc);

            if (desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE)
            {
                ImGui::SetDragDropPayload("CubeMap", &spFile, sizeof(shared_ptr<DDSImageFile>));
                ImGui::EndDragDropSource();
            }
            else
            {
                ImGui::SetDragDropPayload("Texture2D", &spFile, sizeof(shared_ptr<DDSImageFile>));
                ImGui::EndDragDropSource();
            }
        }
    }
}

void FileManipulator::ShowAsList(ModelFile& modelFile, std::shared_ptr<class IFile>& spFile)
{
}
