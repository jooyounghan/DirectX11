#include "CameraRotCom.h"
#include <iostream>

using namespace std;

CameraRotCom::CameraRotCom(shared_ptr<Camera> camera)
	: m_camera_(camera)
{
}

void CameraRotCom::Command(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	/*
		If rotating the mouse in a circular motion causes unexpected rotation along the Z - axis,
		it is likely that the Z - axis rotation is unintentionally changing due to an accumulation of
		floating - point errors.

		To avoid this issue, you should ensure that the view matrix is always orthonormal,
		meaning that the rowsand columns are perpendicular to each otherand have a length of 1.0.
		If the view matrix becomes non - orthonormal due to accumulation of floating - point errors,
		it can cause unexpected rotations.
	*/

	RECT window_rect;
	GetWindowRect(hwnd, &window_rect);

	int new_x_pos = (int)LOWORD(lParam);
	int new_y_pos = (int)HIWORD(lParam);
	const int& delta_yaw_factor = new_x_pos - m_x_pos_;
	const int& delta_pitch_factor = new_y_pos - m_y_pos_;


	if (new_x_pos >= window_rect.left && new_x_pos < window_rect.right &&
		new_y_pos >= window_rect.top && new_y_pos < window_rect.bottom)
	{

		m_x_pos_ = new_x_pos;
		m_y_pos_ = new_y_pos;

		if (GET_KEYSTATE_WPARAM(wParam) & MK_MBUTTON)
		{
			if (m_init_flag)
			{
				m_init_flag = false;
				return;
			}

			const float& delta_yaw = delta_yaw_factor * m_camera_->m_rotation_responsiveness_;
			const float& delta_pitch = delta_pitch_factor * m_camera_->m_rotation_responsiveness_;

			m_camera_->m_total_rotation *= Matrix::CreateRotationY(-delta_time * delta_yaw * 3.141592 / 360.f);
			m_camera_->m_total_rotation *= Matrix::CreateRotationX(-delta_time * delta_pitch * 3.141592 / 360.f);
		}
		else
		{
			m_init_flag = true;
		}
	}
}
