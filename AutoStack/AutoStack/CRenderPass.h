#pragma once

#include "IShader.h"

class CRenderPass
{
protected:
	PixelShader* pixelShader;
	VertexShader* vertexShader;
	ID3D11Device* device;
public:
	CRenderPass(ID3D11Device* device)
	{
		this->device = device;
	}
	PixelShader* getPixelShader()
	{
		return pixelShader;
	}
	VertexShader* getVertexShader()
	{
		return vertexShader;
	}
};