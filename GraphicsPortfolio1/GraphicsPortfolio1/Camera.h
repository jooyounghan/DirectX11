#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <directxtk/SimpleMath.h>

using Microsoft::WRL::ComPtr;
using DirectX::SimpleMath::Matrix;


struct CameraVertexConstantData
{
	Matrix view;
	Matrix projection;
};

class Camera
{
public:
	Camera(ComPtr<ID3D11Device>& device, UINT& buffer_width, UINT& buffer_height);

protected:
	UINT& m_buffer_width_;
	UINT& m_buffer_height_;

public:
	float m_rotation_responsiveness_ = 1000.f;

public:
	Matrix m_total_rotation;
	Matrix m_total_translation;

public:
	CameraVertexConstantData m_stage_vertex_constant_;

public:
	ComPtr<ID3D11Buffer> m_vertex_stage_cbuffer_;
	ComPtr<ID3D11Buffer> m_pixel_stage_cbuffer_;

public:
	void UpdateCamera(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);


};

