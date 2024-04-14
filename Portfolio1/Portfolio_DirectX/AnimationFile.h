#pragma once
#include "IFile.h"
#include "BoneFile.h"
#include <DirectXMath.h>

class AnimChannel
{
public:
	AnimChannel();
	~AnimChannel();

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
	std::vector<AnimTranslation>	vTranslation;
	std::vector<AnimRotation>		vRotation;
	std::vector<AnimScale>			vScale;

private:
	template<typename T>
	inline void SetChannelData(const size_t& uiNumData, void* pSrc, std::vector<T>& channelData);

public:
	inline void SetTranslation(const size_t& uiNumData, void* pSrc) { SetChannelData(uiNumData, pSrc, vTranslation); }
	inline void SetRotation(const size_t& uiNumData, void* pSrc) { SetChannelData(uiNumData, pSrc, vRotation); }
	inline void SetScale(const size_t& uiNumData, void* pSrc) { SetChannelData(uiNumData, pSrc, vScale); }

public:
	DirectX::XMVECTOR GetTranslation(const double& dblPlayTimeIn);

public:
	DirectX::XMMATRIX GetTransformation (const double& dblPlayTimeIn);
	DirectX::XMMATRIX GetTransformationWithoutTranslation(const double& dblPlayTimeIn);

private:
	static DirectX::XMVECTOR SetXMVectorFromXMFloat(const DirectX::XMFLOAT3& xmFloat3, const float& w) { return DirectX::XMVectorSet(xmFloat3.x, xmFloat3.y, xmFloat3.z, w); };
	static DirectX::XMVECTOR SetXMVectorFromXMFloat(const DirectX::XMFLOAT4& xmFloat4, const float& dummy) {return DirectX::XMVectorSet(xmFloat4.x, xmFloat4.y, xmFloat4.z, xmFloat4.w); };

private:
	template<typename T>
	DirectX::XMVECTOR GetInterpolatedData(const double& dblPlayTimeIn, std::vector<T>& vData, const float& w, const DirectX::XMVECTOR& defaultResult);
};

template<typename T>
DirectX::XMVECTOR AnimChannel::GetInterpolatedData(const double& dblPlayTimeIn, std::vector<T>& vData, const float& w, const DirectX::XMVECTOR& defaultResult)
{
	for (size_t right_idx = 0; right_idx < vData.size(); ++right_idx)
	{
		size_t left_idx = right_idx != 0 ? right_idx - 1 : right_idx;
		T& left_data = vData[left_idx];
		T& right_data = vData[right_idx];

		if (left_data.dblTime <= dblPlayTimeIn && dblPlayTimeIn < right_data.dblTime)
		{
			const double totalTime = right_data.dblTime - left_data.dblTime;
			const double spentTime = dblPlayTimeIn - left_data.dblTime;
			const double rightWeight = spentTime / totalTime;
			return (1 - rightWeight) * SetXMVectorFromXMFloat(left_data.xmElement, w) + (rightWeight) * SetXMVectorFromXMFloat(right_data.xmElement, w);
		}
	}

	return defaultResult;
}

template<typename T>
inline void AnimChannel::SetChannelData(const size_t& uiNumData, void* pSrc, std::vector<T>& channelData)
{
	channelData.resize(uiNumData);
	memcpy(channelData.data(), pSrc, sizeof(T) * uiNumData);
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
	std::unordered_map<std::string, AnimChannel> unmapAnimChannels;

public:
	inline const double& GetDuration() const { return dblDuration; }
	inline const double& GetTicksPerSecond() const { return dblTicksPerSecond; }

public:
	inline AnimChannel* AddAnimChannel(const std::string& strChannelName)
	{
		return &unmapAnimChannels[strChannelName];
	}
	inline AnimChannel* GetAnimChannel(const std::string& strChannelName) 
	{ 
		return unmapAnimChannels.find(strChannelName) != unmapAnimChannels.end() ? &unmapAnimChannels[strChannelName] : nullptr; 
	}

public:
	virtual void AcceptFileAsList(class FileManipulator* pFileManipulator);
	virtual ID3D11ShaderResourceView* GetThumbNailSRV() { return nullptr; };
};

