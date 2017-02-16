#pragma once

#include "CRenderPass.h"

class CTextureRenderPass : public CRenderPass
{
private:
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* srv;
	ID3D11SamplerState* samplerState;
public:
	CTextureRenderPass(ID3D11Device* device);
	~CTextureRenderPass();
private:
	void compileShaders();
	void createInputLayout();
	void createTexture();
	void bind(ID3D11DeviceContext* immediateContext);
};