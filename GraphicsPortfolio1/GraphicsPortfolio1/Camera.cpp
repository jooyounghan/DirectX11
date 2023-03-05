#include "Camera.h"
#include "D3D11Utilizer.h"

using namespace DirectX;

Camera::Camera(ComPtr<ID3D11Device>& device, int& buffer_width, int& buffer_height)
	: m_buffer_width_(buffer_width), m_buffer_height_(buffer_height), m_camera_move_flag_(false)
{
	m_total_rotation_ = Matrix::Identity;
	m_total_translation_ = Matrix::CreateTranslation(0.0f, 0.0f, 2.f);

	const float aspect = (float)m_buffer_width_ / (float)m_buffer_height_;
	float m_projFovAngleY = 70.0f;
	float m_nearZ = 0.01f;
	float m_farZ = 100.0f;

	Matrix projRow = XMMatrixPerspectiveFovLH(XMConvertToRadians(m_projFovAngleY), aspect, m_nearZ, m_farZ);
	m_stage_vertex_constant_.projection = projRow.Transpose();

	D3D11Utilizer::CreateConstantBuffer(device, m_stage_vertex_constant_, m_vertex_camera_cbuffer_);
}

const float Camera::GetAspectRatio()
{
	return (float)m_buffer_width_ / m_buffer_height_;
}

void Camera::SetOnCameraMoveFlag(const CameraMoveFlag& flag)
{
	m_camera_move_flag_ |= flag;
}

void Camera::SetOffCameraMoveFlag(const CameraMoveFlag& flag)
{
	m_camera_move_flag_ &= ~flag;
}


void Camera::UpdateCamera(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context)
{

	if (m_camera_move_flag_ & CAMERA_MOVE_FORWARD)
	{
		const Vector3& rightward_vector = m_total_rotation_.Invert().Forward() * m_translation_responsiveness_;
		m_total_translation_ *= Matrix::CreateTranslation(rightward_vector);
	}

	if (m_camera_move_flag_ & CAMERA_MOVE_BACKWARD)
	{
		const Vector3& rightward_vector = m_total_rotation_.Invert().Backward() * m_translation_responsiveness_;
		m_total_translation_ *= Matrix::CreateTranslation(rightward_vector);
	}

	if (m_camera_move_flag_ & CAMERA_MOVE_RIGHT)
	{
		const Vector3& rightward_vector = m_total_rotation_.Invert().Left() * m_translation_responsiveness_;
		m_total_translation_ *= Matrix::CreateTranslation(rightward_vector);
	}

	if (m_camera_move_flag_ & CAMERA_MOVE_LEFT)
	{
		const Vector3& rightward_vector = m_total_rotation_.Invert().Right() * m_translation_responsiveness_;
		m_total_translation_ *= Matrix::CreateTranslation(rightward_vector);
	}

	m_total_rotation_ = m_total_y_rotation_ * m_total_x_rotation_;
	m_stage_vertex_constant_.view = (m_total_translation_ * m_total_rotation_);
	m_stage_vertex_constant_.eye_world_pos = Vector3::Transform(Vector3(0.f, 0.f, 0.f), m_stage_vertex_constant_.view);
	m_stage_vertex_constant_.view = (m_total_translation_ * m_total_rotation_).Transpose();


	D3D11Utilizer::UpdateBuffer(device, context, m_stage_vertex_constant_, m_vertex_camera_cbuffer_);
}


