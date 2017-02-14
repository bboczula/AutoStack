#include "CColorRenderPass.h"

CColorRenderPass::CColorRenderPass(ID3D11Device* device) :CRenderPass(device)
{
	std::cout << "Creating Color Render Pass..." << std::endl;

	compileShaders();
	createInputLayout();
}

CColorRenderPass::~CColorRenderPass()
{
}

void CColorRenderPass::compileShaders()
{
	pixelShader = new PixelShader();
	pixelShader->loadShaderFromFile("ps_color.hlsl");
	pixelShader->setEntryPoint("main");
	pixelShader->setShaderModel("ps_4_0");
	pixelShader->compile();
	pixelShader->create(device);

	vertexShader = new VertexShader();
	vertexShader->loadShaderFromFile("vs_color.hlsl");
	vertexShader->setEntryPoint("main");
	vertexShader->setShaderModel("vs_4_0");
	vertexShader->compile();
	vertexShader->create(device);
}

void CColorRenderPass::createInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC defaultInputLayout[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	HRESULT result = device->CreateInputLayout(defaultInputLayout, ARRAYSIZE(defaultInputLayout),
		vertexShader->getBlobPtr()->GetBufferPointer(), vertexShader->getBlobPtr()->GetBufferSize(), &inputLayout);
	if (SUCCEEDED(result))
	{
#if defined _DEBUG
		std::cout << "Input Layout creation succeded." << std::endl;
#endif
	}
	else
	{
		std::cout << "ERROR: Input Layout creation failed!" << std::endl;
		LPCSTR errMsg = _com_error(result).ErrorMessage();
		std::cout << " > " << errMsg << std::endl;
	}
}