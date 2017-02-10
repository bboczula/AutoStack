#include "CSolidColorRenderPass.h"

CSolidColorRenderPass::CSolidColorRenderPass(ID3D11Device* device) : CRenderPass(device)
{
	std::cout << "Creating Solid Color Render Pass..." << std::endl;
	// Compile shaders
	pixelShader = new PixelShader();
	pixelShader->loadShaderFromFile("ps.hlsl");
	pixelShader->setEntryPoint("main");
	pixelShader->compile();
	pixelShader->create(device);

	vertexShader = new VertexShader();
	vertexShader->loadShaderFromFile("vs.hlsl");
	vertexShader->setEntryPoint("main");
	vertexShader->compile();
	vertexShader->create(device);
}

CSolidColorRenderPass::~CSolidColorRenderPass()
{
	if (pixelShader)
		delete pixelShader;

	if (vertexShader)
		delete vertexShader;
}