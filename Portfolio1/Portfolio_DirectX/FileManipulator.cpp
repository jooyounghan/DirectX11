#include "FileManipulator.h"
#include "ImGuiFileDialog.h"
#include "FileLoader.h"
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
        loadedFile->AcceptFileManipulator(this, loadedFile);
    }
    ImGui::EndChild();
}

void FileManipulator::LoadFiles(const std::wstring& wstrFilePathIn)
{
    filesystem::path texturePath(wstrFilePathIn);
    if (filesystem::exists(texturePath) && filesystem::is_directory(texturePath))
    {
        for (auto& loadedFile : vLoadedFiles)
        {
            if (loadedFile.use_count() > 1)
            {
                const string& fileName = loadedFile->GetFileName();
                if (mapNameToFiles.find(fileName) == mapNameToFiles.end())
                {
                    mapNameToFiles.emplace(fileName, loadedFile);
                }
            }
        }
        vLoadedFiles.clear();

        filesystem::directory_iterator dirIter(texturePath);
        for (auto& entry : dirIter)
        {
            string sExtensionName = entry.path().extension().string();

            UINT uiWidth, uiHeight, uiChannel;

            const string strFileName = entry.path().filename().string();
            const string strExtention = entry.path().extension().string();
            const string strFilePathIn = entry.path().string();

            bool isImage = false;
            FileLoader::IsStrSame(&isImage, sExtensionName, ".jpg", ".png", ".exr", ".dds");

            if (isImage)
            {
                bool isNeedLoad = false;
                uint8_t* ucImageRawData = nullptr;
                shared_ptr<IFile> spFileInterface;

                if (mapNameToFiles.find(strFileName) == mapNameToFiles.end())
                {
                    isNeedLoad = true;
                }
                else
                {
                    if (!mapNameToFiles[strFileName].expired())
                    {
                        spFileInterface = mapNameToFiles[strFileName].lock();
                    }
                    else
                    {
                        mapNameToFiles.erase(strFileName);
                        isNeedLoad = true;
                    }
                }

                if (isNeedLoad)
                {
                    if (strExtention == ".exr")
                    {
                        ucImageRawData = FileLoader::LoadFileWithOpenEXR(strFilePathIn.c_str(), &uiWidth, &uiHeight, &uiChannel);
                        spFileInterface = make_shared<NormalImageFile>(uiWidth, uiHeight, DXGI_FORMAT_R16G16B16A16_FLOAT, ucImageRawData, strFilePathIn, strFileName);
                    }
                    else if (strExtention == ".dds")
                    {
                        spFileInterface = make_shared<DDSImageFile>(strFilePathIn, strFileName, strFilePathIn.find("Brdf") == string::npos);
                    }
                    else
                    {
                        ucImageRawData = FileLoader::LoadFileWithStbi(strFilePathIn.c_str(), &uiWidth, &uiHeight, &uiChannel);
                        spFileInterface = make_shared<NormalImageFile>(uiWidth, uiHeight, DXGI_FORMAT_R8G8B8A8_UNORM, ucImageRawData, strFilePathIn, strFileName);
                    }
                    FileLoader::FreeLoadedFileData(ucImageRawData);
                }
 
                vLoadedFiles.emplace_back(spFileInterface);
            }
        }
    }
}

void FileManipulator::VisitFile(NormalImageFile& imageFile, shared_ptr<IFile>& spFile)
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

void FileManipulator::VisitFile(DDSImageFile& imageFile, shared_ptr<IFile>& spFile)
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