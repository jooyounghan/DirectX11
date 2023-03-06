#include "IRenderable.h"

IRenderable::IRenderable(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& device_context)
	: m_device_(device), m_device_context_(device_context)
{
}
