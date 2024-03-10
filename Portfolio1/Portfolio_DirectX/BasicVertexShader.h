#pragma once

#include "IVertexShader.h"

class IMesh;
class ATransformerable;
class Viewable;

class BasicVertexShader : public IVertexShader
{
public:
	BasicVertexShader();
	virtual ~BasicVertexShader();

private:
	static ID3D11Buffer* const pNullBuffers[3];
	static const UINT pNulls[3];
	static const UINT uiStrides[3];
	static const UINT uiOffsets[3];

public:
	virtual void ApplyShader() override;
	virtual void DisapplyShader() override;

public:
	void SetShader(ATransformerable& transformable, Viewable& viewable);
	void ResetShader();

public:
	void SetIAStage(IMesh& mesh);
	void ResetIAStage();
};

