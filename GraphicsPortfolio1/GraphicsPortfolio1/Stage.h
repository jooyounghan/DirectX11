#pragma once
#include <memory>
#include <wrl.h>


#include "IMeshGroup.h"
#include "Light.h"

using std::shared_ptr;
using Microsoft::WRL::ComPtr;

class Stage
{
public:
	Stage(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& device_context);
	~Stage();

public:
	ComPtr<ID3D11Device>& m_device_;
	ComPtr<ID3D11DeviceContext>& m_device_context_;

public:
	vector<shared_ptr<IMeshGroup>>	m_mesh_group;
	vector<LightConstantData>		m_lights_group;

public:
	void AddMeshGroup(shared_ptr<IMeshGroup> mesh_group);
	void RemoveMeshGroup(const size_t& index);

public:
	void Render();
};

