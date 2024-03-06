#pragma once
#include <vector>
#include <unordered_map>
#include <memory>

#include "IVertexShader.h"
#include "IHullShader.h"
#include "IDomainShader.h"
#include "IGeometryShader.h"
#include "IPixelShader.h"

class IRenderer
{
public:
	IRenderer();
	~IRenderer();

protected:
	class std::unique_ptr<IVertexShader> upVertexShader;
	class std::unique_ptr<IHullShader> upHullShader;
	class std::unique_ptr<IDomainShader> upDomainShader;
	class std::unique_ptr<IGeometryShader> upGeometryShader;
	class std::unique_ptr<IPixelShader> upPixelShader;
};
