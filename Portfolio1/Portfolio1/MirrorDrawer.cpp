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
	// Mirror ����� Normal ���⿡ ���� ViewProj�� ���Ͽ� ��ü�� �׸���. �ſ��� Render Target�� ��ü�� �׸���.
	
	// ���� �𵨸� �� ��ü�� �׸���.
	BaseModelDrawer::Draw(pCamera, pLightManager, vSpModels, pEnvironmentCubeMap);

	// �ſ￡ ���� �𵨸��� �Ѵ�.

}
