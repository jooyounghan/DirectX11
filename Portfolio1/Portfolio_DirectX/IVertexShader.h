#pragma once
#include "IShader.h"
#include "IMesh.h"

class IVertexShader : public IShader
{
protected:
    IVertexShader() {};
    virtual ~IVertexShader() {};

protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> cpInputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> cpVertexShader;

public:
	virtual void ApplyShader() = 0;
	virtual void DisapplyShader() = 0;

public:
	virtual void SetIAStage(const size_t& meshIdx, IMesh& mesh) = 0;
	virtual void ResetIAStage() = 0;
};

