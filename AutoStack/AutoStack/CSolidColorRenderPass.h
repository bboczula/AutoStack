#pragma once

#include <iostream>

#include "CRenderPass.h"
#include "SColor.h"

class CSolidColorRenderPass : public CRenderPass
{
public:
	CSolidColorRenderPass(ID3D11Device* device);
	~CSolidColorRenderPass();
private:
	SColor activeColor;
};