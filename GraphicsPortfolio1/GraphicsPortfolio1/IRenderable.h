#pragma once
#include <memory>
#include <vector>

#include <d3d11.h>
#include <wrl.h>

#include "Shader.h"

using Microsoft::WRL::ComPtr;

class IRenderable
{
public:
	IRenderable(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& device_context);

protected:
	ComPtr<ID3D11Device>& m_device_;
	ComPtr<ID3D11DeviceContext>& m_device_context_;

public:
	ComPtr<ID3D11Buffer> m_vertex_cbuffer_;
public:
	ComPtr<ID3D11Buffer> m_pixel_cbuffer_;

protected:
	std::shared_ptr<Shader> m_render_shader_;

public:
	virtual void Render() = 0;
	virtual void Update() = 0;
};

