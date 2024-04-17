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
	for (size_t left_idx = 0; left_idx < vData.size() - 1; ++left_idx)
	{
		T& left_data = vData[left_idx];
		T& right_data = vData[left_idx + 1];

		if (dblPlayTimeIn < left_data.dblTime)
		{
			return DirectX::XMFLOAT3(
				left_data.xmElement.x,
				left_data.xmElement.y,
				left_data.xmElement.z
			);
		}
		else if (left_data.dblTime <= dblPlayTimeIn && dblPlayTimeIn < right_data.dblTime)
		{
			const double totalTime = right_data.dblTime - left_data.dblTime;
			const double spentTime = dblPlayTimeIn - left_data.dblTime;
			const double rightWeight = spentTime / totalTime;
			return DirectX::XMFLOAT3(
				(1 - rightWeight) * left_data.xmElement.x + rightWeight * right_data.xmElement.x,
				(1 - rightWeight) * left_data.xmElement.y + rightWeight * right_data.xmElement.y,
				(1 - rightWeight) * left_data.xmElement.z + rightWeight * right_data.xmElement.z			);
		}
		else;
	}

	if (!vData.empty() && vData[vData.size() - 1].dblTime <= dblPlayTimeIn)
	{
		T& last_data = vData[vData.size() - 1];
		return DirectX::XMFLOAT3(
			last_data.xmElement.x,
			last_data.xmElement.y,
			last_data.xmElement.z
		);
	}
	return defaultResult;
}

template<typename T>
DirectX::XMFLOAT4 AnimChannel::GetInterpolatedData(const double& dblPlayTimeIn, std::vector<T>& vData, const DirectX::XMFLOAT4& defaultResult)
{
	for (size_t left_idx = 0; left_idx < vData.size() - 1; ++left_idx)
	{
		T& left_data = vData[left_idx];
		T& right_data = vData[left_idx + 1];


		if (dblPlayTimeIn < left_data.dblTime)
		{
			return DirectX::XMFLOAT4(
				left_data.xmElement.x,
				left_data.xmElement.y,
				left_data.xmElement.z,
				left_data.xmElement.w
			);
		}
		else if (left_data.dblTime <= dblPlayTimeIn && dblPlayTimeIn < right_data.dblTime)
		{
			const double totalTime = right_data.dblTime - left_data.dblTime;
			const double spentTime = dblPlayTimeIn - left_data.dblTime;
			const double rightWeight = spentTime / totalTime;
			DirectX::XMVECTOR result = DirectX::XMQuaternionSlerp(
				DirectX::XMVectorSet(left_data.xmElement.x, left_data.xmElement.y, left_data.xmElement.z, left_data.xmElement.w),
				DirectX::XMVectorSet(right_data.xmElement.x, right_data.xmElement.y, right_data.xmElement.z, right_data.xmElement.w),
				rightWeight
			);

			return DirectX::XMFLOAT4(result.m128_f32);
		}
		else;
	}

	if (!vData.empty() && vData[vData.size() - 1].dblTime <= dblPlayTimeIn)
	{
		T& last_data = vData[vData.size() - 1];
		return DirectX::XMFLOAT4(
			last_data.xmElement.x,
			last_data.xmElement.y,
			last_data.xmElement.z,
			last_data.xmElement.w
		);
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

