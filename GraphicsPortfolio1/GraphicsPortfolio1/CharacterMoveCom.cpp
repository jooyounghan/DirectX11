#include "CharacterMoveCom.h"
#include <iostream>

using std::cout;
using std::endl;

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;

MoveForwardCom::MoveForwardCom(shared_ptr<Camera> camera)
	: m_camera_(camera)
{
}

void MoveForwardCom::Command(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	Vector3 forward_vector = m_camera_->m_total_rotation.Invert().Forward();
	forward_vector *= m_camera_->m_translation_responsiveness_;

	m_camera_->m_total_translation *= Matrix::CreateTranslation(forward_vector);
}

MoveBackWardCom::MoveBackWardCom(shared_ptr<Camera> camera)
	: m_camera_(camera)
{
}

void MoveBackWardCom::Command(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	Vector3 backward_vector = m_camera_->m_total_rotation.Backward();
	backward_vector *= m_camera_->m_translation_responsiveness_;

	m_camera_->m_total_translation *= Matrix::CreateTranslation(backward_vector);
}

MoveRightCom::MoveRightCom(shared_ptr<Camera> camera)
	: m_camera_(camera)
{
}

void MoveRightCom::Command(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	Vector3 rightward_vector = m_camera_->m_total_rotation.Right();
	rightward_vector *= m_camera_->m_translation_responsiveness_;

	m_camera_->m_total_translation *= Matrix::CreateTranslation(rightward_vector);
}

MoveLeftCom::MoveLeftCom(shared_ptr<Camera> camera)
	: m_camera_(camera)
{
}

void MoveLeftCom::Command(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam)
{
	Vector3 leftward_vector = m_camera_->m_total_rotation.Left();
	leftward_vector *= m_camera_->m_translation_responsiveness_;

	m_camera_->m_total_translation *= Matrix::CreateTranslation(leftward_vector);
}
