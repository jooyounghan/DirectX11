#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <directxtk/SimpleMath.h>


using Microsoft::WRL::ComPtr;
using DirectX::SimpleMath::Matrix;

typedef char CameraMoveFlag;

#define CAMERA_MOVE_FORWARD		0b00000001
#define CAMERA_MOVE_BACKWARD	0b00000010
#define CAMERA_MOVE_RIGHT		0b00000100
#define CAMERA_MOVE_LEFT		0b00001000

struct CameraVertexConstantData
{
	Matrix view;
	Matrix projection;
};

class Camera
{
	friend class CameraRotCom;

public:
	Camera(ComPtr<ID3D11Device>& device, int& buffer_width, int& buffer_height);

protected:
	int& m_buffer_width_;
	int& m_buffer_height_;

public:
	float m_rotation_responsiveness_ = 30.f;
	float m_translation_responsiveness_ = 0.1f;

public:
	Matrix m_total_rotation;
	Matrix m_total_translation;

public:
	CameraVertexConstantData m_stage_vertex_constant_;

public:
	ComPtr<ID3D11Buffer> m_vertex_stage_cbuffer_;
	ComPtr<ID3D11Buffer> m_pixel_stage_cbuffer_;

public:
	CameraMoveFlag m_camera_move_flag_;

public:
	void SetOnCameraMoveFlag(const CameraMoveFlag& flag);
	void SetOffCameraMoveFlag(const CameraMoveFlag& flag);

public:
	void UpdateCamera(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);


};

