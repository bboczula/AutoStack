#pragma once

#include "IShader.h"

class CRenderPass
{
protected:
	PixelShader* pixelShader;
	VertexShader* vertexShader;
	ID3D11Device* device;
	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* psConstantBuffer;
	D3D_PRIMITIVE_TOPOLOGY topology;
public:
	CRenderPass(ID3D11Device* device) : device(device), topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP), psConstantBuffer(nullptr)
	{
	}
	bool isPsCbPresent()
	{
		return (psConstantBuffer != nullptr);
	}
	PixelShader* getPixelShader()
	{
		return pixelShader;
	}
	VertexShader* getVertexShader()
	{
		return vertexShader;
	}
	D3D_PRIMITIVE_TOPOLOGY* getTopology()
	{
		return &topology;
	}
	ID3D11InputLayout* getInputLayout()
	{
		return inputLayout;
	}
	ID3D11Buffer* getPsConstantBuffer()
	{
		return psConstantBuffer;
	}
};