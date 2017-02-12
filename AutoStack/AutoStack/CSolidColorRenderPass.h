#pragma once

#include <iostream>

#include "CRenderPass.h"
#include "SColor.h"
#include "SPsConstantBuffer.h"

class CSolidColorRenderPass : public CRenderPass
{
public:
	CSolidColorRenderPass(ID3D11Device* device);
	~CSolidColorRenderPass();
	void setColor(int r, int g, int b);
	SPsConstantBuffer* getConstantBufferData();
private:
	SColor activeColor;
	SPsConstantBuffer cbData;
	void compileShaders();
	void createInputLayout();
	void createConstantBuffers();
};