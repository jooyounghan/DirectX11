#pragma once

class PortfolioApp
{
public:
	PortfolioApp() {};
	~PortfolioApp() {};

public:
	void Update();
	void Render();
	void SwapChain();

public:
	struct ID3D11Device* GetDevice();
	struct ID3D11DeviceContext* GetDeviceContext();
};

