#pragma once

#include "IVertexShader.h"

class IMesh;
class ATransformerable;
class Viewable;

class BasicVertexShader : public IVertexShader
{
	friend IVertexShader;

private:
	BasicVertexShader();
	BasicVertexShader(const BasicVertexShader&) = delete;
	BasicVertexShader& operator=(const BasicVertexShader&) = delete;
	virtual ~BasicVertexShader();

public:
	static BasicVertexShader* pPixelShader;

public:
	inline static BasicVertexShader* GetInstance() {
		if (pPixelShader == nullptr) {
			pPixelShader = new BasicVertexShader();
		}
		return pPixelShader;
	}

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

