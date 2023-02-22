#pragma once

#include "ICommand.h"
#include "Camera.h"


class CameraRotCom : public ICommand
{
public:
	CameraRotCom(shared_ptr<Camera> camera);

public:
	shared_ptr<Camera> m_camera_;

public:
	virtual void Command(HWND hwnd, const float& delta_time, WPARAM wParam, LPARAM lParam);

public:
	int m_x_pos_ = 0;
	int m_y_pos_ = 0;

public:
	bool m_init_flag = true;
};