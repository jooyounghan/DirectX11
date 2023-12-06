#include "SamplerState.h"
#include "ID3D11Helper.h"


SamplerState::SamplerState(ID3D11Device* pDevice) 
{
	ID3D11Helper::CreateSampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, NULL, pDevice, cpDrawerWrapSampler.GetAddressOf());
	ID3D11Helper::CreateSampler(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, NULL, pDevice, cpDrawerClampSampler.GetAddressOf());
}
