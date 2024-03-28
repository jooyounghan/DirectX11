#include "Animation.h"
#include "DirectXDevice.h"
#include "ID3D11Helper.h"

Animation::Animation(const std::shared_ptr<AnimationFile>& spAnimationFileIn)
	: spAnimFile(spAnimationFileIn), dblAnimPlayTime(0.)
{
	AutoZeroMemory(sAnimData);
	sAnimData.dblAnimSpeed = 1.0;

	ID3D11Helper::CreateBuffer(
		DirectXDevice::pDevice, sAnimData, D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, NULL,
		cpAnimationBuffer.GetAddressOf()
	);
}

Animation::~Animation()
{
}

void Animation::UpdateAnimationBuffer()
{
	ID3D11Helper::UpdateBuffer(
		DirectXDevice::pDeviceContext, 
		sAnimData, D3D11_MAP_WRITE_DISCARD,
		cpAnimationBuffer.Get()
	);
}
