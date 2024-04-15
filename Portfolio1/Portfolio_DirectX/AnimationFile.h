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

public:
	inline void ReserveTranslation(const size_t& uiNumData) { vTranslation.reserve(uiNumData); }
	inline void ReserveRotationQuat(const size_t& uiNumData) { vRotation.reserve(uiNumData); }
	inline void ReserveScale(const size_t& uiNumData) { vScale.reserve(uiNumData); }

	void AddTranslation(const double& dblTimeIn, const float& x, const float& y, const float& z);
	void AddRotationQuat(const double& dblTimeIn, const float& x, const float& y, const float& z, const float& w);
	void AddScale(const double& dblTimeIn, const float& x, const float& y, const float& z);

public:
	DirectX::XMFLOAT3 GetTranslation(const double& dblPlayTimeIn);

public:
	DirectX::XMMATRIX GetTransformation (const double& dblPlayTimeIn);
	DirectX::XMMATRIX GetTransformationWithoutTranslation(const double& dblPlayTimeIn);

private:
	template<typename T>
	DirectX::XMFLOAT3 GetInterpolatedData(const double& dblPlayTimeIn, std::vector<T>& vData, const DirectX::XMFLOAT3& defaultResult);

	template<typename T>
	DirectX::XMFLOAT4 GetInterpolatedData(const double& dblPlayTimeIn, std::vector<T>& vData, const DirectX::XMFLOAT4& defaultResult);
};

template<typename T>
DirectX::XMFLOAT3 AnimChannel::GetInterpolatedData(const double& dblPlayTimeIn, std::vector<T>& vData, const DirectX::XMFLOAT3& defaultResult)
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
			return DirectX::XMFLOAT3(
				(1 - rightWeight) * left_data.xmElement.x + rightWeight * right_data.xmElement.x,
				(1 - rightWeight) * left_data.xmElement.y + rightWeight * right_data.xmElement.y,
				(1 - rightWeight) * left_data.xmElement.z + rightWeight * right_data.xmElement.z
			);
		}
	}
	return defaultResult;
}

template<typename T>
DirectX::XMFLOAT4 AnimChannel::GetInterpolatedData(const double& dblPlayTimeIn, std::vector<T>& vData, const DirectX::XMFLOAT4& defaultResult)
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
			return DirectX::XMFLOAT4(
				(1 - rightWeight) * left_data.xmElement.x + rightWeight * right_data.xmElement.x,
				(1 - rightWeight) * left_data.xmElement.y + rightWeight * right_data.xmElement.y,
				(1 - rightWeight) * left_data.xmElement.z + rightWeight * right_data.xmElement.z,
				(1 - rightWeight) * left_data.xmElement.w + rightWeight * right_data.xmElement.w
			);
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

