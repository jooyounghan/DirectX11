#include "TextureSet.h"
#include "ID3D11Helper.h"

TextureSet::TextureSet() {};

TextureSet::~TextureSet() {}

void TextureSet::SetTextureToShaderResource(TextureType textureType, Texture* pTexture)
{
	Textures[textureType].TextureName = pTexture->TextureName;
	pTexture->TextureSRV.CopyTo(Textures[textureType].TextureSRV.GetAddressOf());
}