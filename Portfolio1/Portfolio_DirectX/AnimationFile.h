#pragma once
#include "IFile.h"

#include <unordered_map>
#include <DirectXMath.h>
#include <cmath>
#include <algorithm>

class AnimChannelFile
{
public:
	AnimChannelFile(
		const std::string& strChannleNameIn,
		const unsigned int& uiNumPosition,
		const unsigned int& uiNumRotation,
		const unsigned int& uiNumScale,
		void* pPositionIn,
		void* pRotationIn,
		void* pScaleIn
	);
	~AnimChannelFile();

public:
	typedef struct AnimTranslation
	{
		double dblTime;
		DirectX::XMFLOAT3 xmElement;
	} AnimScale;

	struct AnimRotation
	{
		double dblTime;
		DirectX::XMFLOAT4 xmElement;
	};

private:
	std::string						strAnimChannelData;

private:
	std::vector<AnimTranslation>	vTranslation;
	std::vector<AnimRotation>		vRotation;
	std::vector<AnimScale>			vScale;

public:
	inline const std::string& GetAnimChannelName() const { return strAnimChannelData; }
	DirectX::XMMATRIX GetChannelTransformation(const double& dblPlayTimeIn);

private:
	static DirectX::XMVECTOR SetXMVectorFromXMFloat(const DirectX::XMFLOAT3& xmFloat3, const float& w) { return DirectX::XMVectorSet(xmFloat3.x, xmFloat3.y, xmFloat3.z, w); };
	static DirectX::XMVECTOR SetXMVectorFromXMFloat(const DirectX::XMFLOAT4& xmFloat4, const float& dummy) {return DirectX::XMVectorSet(xmFloat4.x, xmFloat4.y, xmFloat4.z, xmFloat4.w); };

private:
	template<typename T>
	DirectX::XMVECTOR GetInterpolatedData(const double& dblPlayTimeIn, std::vector<T>& vData, const float& w, const DirectX::XMVECTOR& defaultResult);
};

template<typename T>
DirectX::XMVECTOR AnimChannelFile::GetInterpolatedData(const double& dblPlayTimeIn, std::vector<T>& vData, const float& w, const DirectX::XMVECTOR& defaultResult)
{
	for (size_t back_idx = 0; back_idx < vData.size(); ++back_idx)
	{
		size_t front_idx = back_idx != 0 ? back_idx - 1 : back_idx;
		T& front_data = vData[front_idx];
		T& back_data = vData[back_idx];

		if (front_data.dblTime < dblPlayTimeIn && dblPlayTimeIn < back_data.dblTime)
		{
			const double totalTime = back_data.dblTime - front_data.dblTime;
			const double frontTimePortion = dblPlayTimeIn - front_data.dblTime;
			return (frontTimePortion / totalTime) * SetXMVectorFromXMFloat(front_data.xmElement, w) + (1.0 - frontTimePortion / totalTime) * SetXMVectorFromXMFloat(back_data.xmElement, w);
		}
	}

	return defaultResult;
}


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
	std::unordered_map<std::string, AnimChannelFile> unmapAnimChannels;

public:
	inline const double& GetDuration() const { return dblDuration; }
	inline const double& GetTicksPerSecond() const { return dblTicksPerSecond; }

public:
	inline std::unordered_map<std::string, AnimChannelFile>& GetAnimationChannels() { return unmapAnimChannels; }
	inline void AddAnimChannel(AnimChannelFile&& spAnimChannel) { 
		unmapAnimChannels.emplace(spAnimChannel.GetAnimChannelName(), spAnimChannel);
	}


public:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator);
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() { return nullptr; };
};

