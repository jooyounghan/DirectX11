#pragma once
#include "IGuiMenu.h"
#include "FileLoader.h"
#include "DefineVar.h"
#include <string>
#include <vector>
#include <memory>

class FileManipulator : public IGuiMenu
{
public:
	FileManipulator();
	virtual ~FileManipulator();

public:
	virtual void PopAsDialog() override;
	virtual bool IsGuiAvailable() override;

protected:
	void ChooseFiles();
	void DisplayFiles();

protected:
	void LoadFiles(const std::wstring& wstrFilePathIn);

public:
	template<typename T>
	void ShowAsList(T& fileIn, const char* keyIn);

public:
	void ShowAsList(class NormalImageFile& imageFile);
	void ShowAsList(class DDSImageFile& imageFile);

private:
	std::string strCurrentPath;
	std::vector<std::shared_ptr<class IFile>> vLoadedFiles;
};

template<typename T>
inline void FileManipulator::ShowAsList(T& fileIn, const char* keyIn)
{
	struct ID3D11ShaderResourceView* pIndexedSRV = nullptr;

	ImGui::BeginGroup();
	ImGui::Image(pIndexedSRV, ImVec2(60.f, 60.f));
	ImGui::SameLine();
	ImGui::TextEx(fileIn.GetFileLabel().c_str(), (const char*)0, ImGuiTextFlags_::ImGuiTextFlags_NoWidthForLargeClippedText);
	ImGui::EndGroup();

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
	{
		ImGui::SetDragDropPayload(keyIn, &fileIn, sizeof(T));
		ImGui::EndDragDropSource();
	}
}
