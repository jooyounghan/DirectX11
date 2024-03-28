#pragma once
#include "IFile.h"

#include <vector>
#include <DirectXMath.h>

class AnimChannelFile
{
public:
	AnimChannelFile(
		const std::string& strChannleNameIn,
		const unsigned int& uiNumPosition,
		void* pPositionIn,
		const unsigned int& uiNumRotation,
		void* pRotationIn,
		const unsigned int& uiNumScale,
		void* pScaleIn
	);
	~AnimChannelFile();

public:
	typedef struct AnimTransformation
	{
		double dblTime;
		DirectX::XMFLOAT3 xmTransformation;
	} AnimScale;

	struct AnimRotation
	{
		double dblTime;
		DirectX::XMFLOAT4 xmQuaternion;
	};

private:
	std::string						strAnimChannelData;
	std::vector<AnimTransformation> vTransformation;
	std::vector<AnimRotation>		vRotation;
	std::vector<AnimScale>			vScale;
};

class AnimationFile : public IFile, public std::enable_shared_from_this<AnimationFile>
{
public:
	AnimationFile(
		const std::string& strFileLabelIn,
		const double& dblDurationIn,
		const double& dblTicksPerSecondIn
	);
	~AnimationFile();

private:
	double dblDuration;
	double dblTicksPerSecond;
	std::vector<AnimChannelFile> vAnimChannels;

public:
	inline void AddAnimChannel(AnimChannelFile&& spAnimChannel) { vAnimChannels.emplace_back(spAnimChannel); }

public:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator);
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() { return nullptr; };
};

