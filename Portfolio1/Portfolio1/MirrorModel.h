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

public:
	virtual void	SetIAProperties();
	virtual void	SetVSConstantBuffers();
	virtual void	SetHSConstantBuffers();
	virtual void	SetDSConstantBuffers();
	virtual void	SetGSConstantBuffers();
	virtual void	SetPSConstantBuffers();
	virtual void	ResetConstantBuffers();

public:
	virtual void	SetVSShaderResources();
	virtual void	SetHSShaderResources();
	virtual void	SetDSShaderResources();
	virtual void	SetGSShaderResources();
	virtual void	SetPSShaderResources();
	virtual void	ResetShaderResources();

public:
	void			SetPSMirrorConstantBuffer();
	void			SetPSMirrorShaderResources();

	void			ResetPSMirrorConstantBuffer();
	void			ResetPSMirrorShaderResources();

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

