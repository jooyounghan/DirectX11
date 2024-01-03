#include "ViewPort.h"
#include "DefineVar.h"
ViewPort::ViewPort(ID3D11DeviceContext* pDeviceContextIn)
	: pDeviceContext(nullptr)
{
	AutoZeroMemory(sViewPort);
}

void ViewPort::SetViewPort(
	const float& fTopLeftXIn,
	const float& fTopLeftYIn,
	const float& fWidthIn,
	const float& fHeightIn,
	const float& fMinDepthIn,
	const float& fMaxDepthIn
)
{
	sViewPort.TopLeftX = fTopLeftXIn;
	sViewPort.TopLeftY = fTopLeftYIn;
	sViewPort.Width = fWidthIn;
	sViewPort.Height = fHeightIn;
	sViewPort.MinDepth = fMinDepthIn;
	sViewPort.MaxDepth = fMaxDepthIn;
	if (pDeviceContext != nullptr)
	{
		pDeviceContext->RSSetViewports(1, &sViewPort);
	}
}

