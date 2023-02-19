#include "CameraRotCom.h"
#include <iostream>

using std::cout;
using std::endl;

CameraRotCom::CameraRotCom(shared_ptr<Camera> camera)
	: m_camera_(camera)
{
}

void CameraRotCom::Command(const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	UINT new_x_pos = (UINT)LOWORD(lParam);
	UINT new_y_pos = (UINT)HIWORD(lParam);

	const int& delta_yaw_factor	= static_cast<int>(new_x_pos - m_x_pos_);
	const int& delta_pitch_factor	= static_cast<int>(new_y_pos - m_y_pos_);

	m_x_pos_ = new_x_pos;
	m_y_pos_ = new_y_pos;

	if (m_init_flag)
	{
		m_init_flag = false;
		return;
	}

	const float& delta_yaw = delta_yaw_factor * m_camera_->m_rotation_responsiveness_;
	const float& delta_pitch = delta_pitch_factor * m_camera_->m_rotation_responsiveness_;

	m_camera_->m_total_rotation *= Matrix::CreateRotationZ(-delta_time * delta_yaw * 3.141592 / 360.f);
	m_camera_->m_total_rotation *= Matrix::CreateRotationY(-delta_time * delta_pitch * 3.141592 / 360.f);

}
