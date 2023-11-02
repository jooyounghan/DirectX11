#pragma once
#include "ManageGuiInterface.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <wrl/client.h>
#include <string>
#include <vector>
#include <map>
#include <memory>

class FileManageGui : public ManageGuiInterface
{
public:
	FileManageGui(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
	);
	virtual ~FileManageGui() override;

public:
	virtual void RenderGui() override;

private:
	void UpdateLoadedFiles(const std::wstring& wstrFilePathIn);

private:
	void SetFileDialog();
	void SetLoadedFiles();

private:
	void LoadImages(const std::string& strFilePathIn);

private:
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContext;

private:
	std::string strCurrentPath;
	std::vector<std::shared_ptr<class FileInterface>> vLoadedFiles;
	std::map<std::string, std::shared_ptr<class FileInterface>> mapNameToFiles;
};