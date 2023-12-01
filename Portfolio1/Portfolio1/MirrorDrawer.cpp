#include "MirrorDrawer.h"
#include "ID3D11Helper.h"
MirrorDrawer::MirrorDrawer(
	Microsoft::WRL::ComPtr<ID3D11Device>& cpDeviceIn, 
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& cpDeviceContextIn
)
	: BaseModelDrawer(cpDevice, cpDeviceContext)
{
	ID3D11Helper::CreatePS(cpDevice.Get(), L"MirrorModelPS.hlsl", cpMirrorPixelShader.GetAddressOf());
}

MirrorDrawer::~MirrorDrawer()
{
}

void MirrorDrawer::Draw(
	CameraInterface* pCamera,
	LightManager* pLightManager,
	const std::vector<std::shared_ptr<PickableModel>>& vSpModels,
	CubeMapModel* pEnvironmentCubeMap,
	const std::vector<std::shared_ptr<MirrorModel>>& vMirrorModel
)
{
	// Mirror 평면의 Normal 방향에 대한 ViewProj를 통하여 물체를 그린다. 거울의 Render Target에 물체를 그린다.
	
	// 기존 모델링 할 물체를 그린다.
	BaseModelDrawer::Draw(pCamera, pLightManager, vSpModels, pEnvironmentCubeMap);

	// 거울에 대한 모델링을 한다.

}
