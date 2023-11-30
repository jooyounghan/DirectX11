#pragma once
#include "PickableModel.h"

class MirrorModel : public PickableModel
{
public:
	MirrorModel(
		Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		const float& fMirrorWidthIn,
		const float& fMirrorHeightIn,
		const float& fCenterXIn,
		const float& fCenterYIn,
		const float& fCenterZIn,
		const std::shared_ptr<class CameraInterface>& spCameraIn
	);
	virtual ~MirrorModel();

public:
	virtual void Update(const float& fDelta) override;
	virtual void Render() override;
public:
	virtual void SetIAProperties() = 0;
	virtual void SetVSConstantBuffers() = 0;
	virtual void SetHSConstantBuffers() = 0;
	virtual void SetDSConstantBuffers() = 0;
	virtual void SetGSConstantBuffers() = 0;
	virtual void SetPSConstantBuffers() = 0;
	virtual void ResetConstantBuffers() = 0;

public:
	virtual void SetVSShaderResources() = 0;
	virtual void SetHSShaderResources() = 0;
	virtual void SetDSShaderResources() = 0;
	virtual void SetGSShaderResources() = 0;
	virtual void SetPSShaderResources() = 0;
	virtual void ResetShaderResources() = 0;

protected:
	float fMirrorWidth;
	float fMirrorHeight;

protected:
	struct
	{
		DirectX::XMVECTOR fCenterPos;
		DirectX::XMMATRIX xmmMirrorViewProj;
		DirectX::XMMATRIX xmmMirrorViewProjInv;
	} sMirrorInfo;
	Microsoft::WRL::ComPtr<ID3D11Buffer> cpMirrorInfoBuffer;

protected:
	DirectX::XMVECTOR xmvDefaultDirection;
	DirectX::XMVECTOR xmvDefaultUp;

protected:
	const std::shared_ptr<class CameraInterface>& spCamera;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> cpMirrorTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cpMirrorSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> cpMirrorRTV;

};

