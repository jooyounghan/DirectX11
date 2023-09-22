#include "TextureSet.h"
#include "ID3D11Helper.h"

TextureSet::TextureSet() {};
TextureSet::~TextureSet() {};

void TextureSet::SetTextureToShaderResource(TextureType textureType, const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& cpTextureShaderResource)
{
	cpTextureShaderResource.CopyTo(ShaderResources[textureType].GetAddressOf());
}
