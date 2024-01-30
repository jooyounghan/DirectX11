#include "FileManipulator.h"
#include "ImGuiFileDialog.h"
#include "FileLoader.h"
#include "IFile.h"

#include "DDSImageFile.h"
#include "NormalImageFile.h"

#include "DirectXDevice.h"

#include <filesystem>

using namespace std;
using namespace ImGui;

FileManipulator::FileManipulator()
{
}

FileManipulator::~FileManipulator()
{
}

void FileManipulator::PopAsDialog()
{
    Begin("File Loader");
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
    End();
}

bool FileManipulator::IsGuiAvailable()
{
    return false;
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

            UINT uiWidth, uiHeight, uiChannel;
            uint8_t* ucImageRawData = nullptr;


            const string strExtention = entry.path().extension().string();
            const string strFilePathIn = entry.path().string();

            if (mapNameToFiles.find(strFilePathIn) == mapNameToFiles.end())
            {
                shared_ptr<IFile> spFileInterface;
                if (strExtention == ".exr")
                {
                    ucImageRawData = FileLoader::LoadFileWithOpenEXR(strFilePathIn.c_str(), &uiWidth, &uiHeight, &uiChannel);
                    spFileInterface = make_shared<NormalImageFile>(uiWidth, uiHeight, DXGI_FORMAT_R16G16B16A16_FLOAT, ucImageRawData, strFilePathIn);
                }
                else if (strExtention == ".dds")
                {
                    spFileInterface = make_shared<DDSFile>(cpDevice, cpDeviceContext, strFilePathIn, strFilePathIn.find("Brdf") == string::npos);
                }
                else
                {
                    ucImageRawData = FileLoader::LoadFileWithStbi(strFilePathIn.c_str(), &uiWidth, &uiHeight, &uiChannel);
                    spFileInterface = make_shared<NormalImageFile>(uiWidth, uiHeight, NULL, ucImageRawData, strFilePathIn);
                }
                vLoadedFiles.emplace_back(spFileInterface);
                FileLoader::FreeLoadedFileData(ucImageRawData);
            }
            else
            {
                vLoadedFiles.emplace_back(mapNameToFiles[strFilePathIn]);
            }
        }
    }
}
