#pragma once
#include "PickableModelInterface.h"
#include "CameraInterface.h"

class  MirrorModel : public PickableModelInterface, public CameraInterface
{
public:
	MirrorModel(
		ID3D11Device* pDeviceIn,
		ID3D11DeviceContext* pDeviceContextIn,
		const float& fMirrorWidthIn,
		const float& fMirrorHeightIn,
		DXGI_FORMAT eMirrorFormat,
		const float& fCenterXIn,
		const float& fCenterYIn,
		const float& fCenterZIn,
		const std::shared_ptr<CameraInterface>& cpCameraIn
	);
	virtual ~MirrorModel();

#pragma region Virtual
// Virtual Function ==============================================
public:
	virtual void Update(const float& fDelta) override;

public:
	virtual void SetIAProperties();

private:
	//virtual void SetConstantBuffers() override final {};
	//virtual void ResetConstantBuffers() override final {};
	//virtual void SetShaderResources() override final {};
	//virtual void ResetShaderResources() override final {};

public:
	virtual void SetConstantBuffers();
	virtual void SetShaderResources();
	virtual void ResetConstantBuffers();
	virtual void ResetShaderResources();

public:
	virtual void SetConstantBuffersAsCamera();
	virtual void ResetConstantBuffersAsCamera();

public:
	virtual void OMSetRenderTargets() override;
	virtual void ResetCamera() override;
// ==============================================================
#pragma endregion

protected:
	UINT uiMirrorTextureWidth;
	UINT uiMirrorTextureHeight;

protected:
	float fMirrorWidth;
	float fMirrorHeight;
	float fMirrorAspectRatio;

protected:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> cpMirrorResolvedTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cpMirrorResolvedSRV;

private:
	const std::shared_ptr<CameraInterface>& cpCamera;
};

