#include "Camera.h"
#include "D3D11Utilizer.h"

using namespace DirectX;

Camera::Camera(ComPtr<ID3D11Device>& device, UINT& buffer_width, UINT& buffer_height)
	: m_buffer_width_(buffer_width), m_buffer_height_(buffer_height)
{
	m_total_translation = Matrix::CreateTranslation(0.0f, 50.0f, 50).Transpose();

	const float aspect = (float)m_buffer_width_ / (float)m_buffer_height_;
	float m_projFovAngleY = 70.0f;
	float m_nearZ = 0.01f;
	float m_farZ = 100.0f;

	Matrix projRow = XMMatrixPerspectiveFovLH(XMConvertToRadians(m_projFovAngleY), aspect, m_nearZ, m_farZ);
	m_stage_vertex_constant_.projection = projRow.Transpose();

	D3D11Utilizer::CreateConstantBuffer(device, m_stage_vertex_constant_, m_vertex_stage_cbuffer_);
}

void Camera::UpdateCamera(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context)
{
	m_stage_vertex_constant_.view = (m_total_rotation * m_total_translation).Transpose();
	D3D11Utilizer::UpdateBuffer(device, context, m_stage_vertex_constant_, m_vertex_stage_cbuffer_);
}

