#pragma once

#include "CRenderPass.h"

class CColorRenderPass : public CRenderPass
{
public:
	CColorRenderPass(ID3D11Device* device);
	~CColorRenderPass();

private:
	void compileShaders();
	void createInputLayout();
	void bind(ID3D11DeviceContext* deviceContext);
};