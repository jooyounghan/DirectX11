#pragma once
#include "IModel.h"
class TestModel : public IModel
{
public:
	TestModel(
		ComPtr<ID3D11Device>& cpDeviceIn,
		ComPtr<ID3D11DeviceContext>& cpDeviceContextIn,
		const float& fCenterX,
		const float& fCenterY,
		const float& fCenterZ,
		const float& fLen
	);
	~TestModel() {};

public:
	virtual void Update() override;
	virtual void Render() override;
};

