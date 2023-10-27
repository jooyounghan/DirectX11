#pragma once
class ManageGuiInterface
{
public:
	ManageGuiInterface() {};
	virtual ~ManageGuiInterface() {};

public:
	virtual void RenderGui() = 0;
};

