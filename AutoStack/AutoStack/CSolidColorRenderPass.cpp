#include "CSolidColorRenderPass.h"

CSolidColorRenderPass::CSolidColorRenderPass(ID3D11Device* device) : CRenderPass(device)
{
	std::cout << "Creating Solid Color Render Pass..." << std::endl;
	
	setColor(0, 255, 0);
	cbData.r = static_cast<float>(activeColor.r / 255.0f);
	cbData.g = static_cast<float>(activeColor.g / 255.0f);
	cbData.b = static_cast<float>(activeColor.b / 255.0f);
	compileShaders();
	createInputLayout();
	createConstantBuffers();
}

CSolidColorRenderPass::~CSolidColorRenderPass()
{
	if (pixelShader)
		delete pixelShader;

	if (vertexShader)
		delete vertexShader;
}

void CSolidColorRenderPass::setColor(int r, int g, int b)
{
	activeColor.r = r;
	activeColor.g = g;
	activeColor.b = b;
	cbData.r = static_cast<float>(activeColor.r / 255.0f);
	cbData.g = static_cast<float>(activeColor.g / 255.0f);
	cbData.b = static_cast<float>(activeColor.b / 255.0f);
}

SPsConstantBuffer * CSolidColorRenderPass::getConstantBufferData()
{
	return &cbData;
}

void CSolidColorRenderPass::compileShaders()
{
	pixelShader = new PixelShader();
	pixelShader->loadShaderFromFile("ps.hlsl");
	pixelShader->compile();
	pixelShader->create(device);

	vertexShader = new VertexShader();
	vertexShader->loadShaderFromFile("vs.hlsl");
	vertexShader->compile();
	vertexShader->create(device);
}

void CSolidColorRenderPass::createInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC defaultInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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

void CSolidColorRenderPass::createConstantBuffers()
{
	// Create PS Constant Buffer
	D3D11_BUFFER_DESC constantBufferDescription;
	ZeroMemory(&constantBufferDescription, sizeof(D3D11_BUFFER_DESC));

	constantBufferDescription.ByteWidth = sizeof(SPsConstantBuffer);
	constantBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//constantBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constantBufferDescription.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA initData2;
	initData2.pSysMem = &cbData;
	initData2.SysMemPitch = 0;
	initData2.SysMemSlicePitch = 0;

	HRESULT pscbResult = device->CreateBuffer(&constantBufferDescription, &initData2, &psConstantBuffer);
	if (SUCCEEDED(pscbResult))
	{
#if defined _DEBUG
		std::cout << "PS Context Buffer creation succeded." << std::endl;
#endif
	}
	else
	{
		std::cout << "ERROR: PS Context Buffer creation failed!" << std::endl;
		LPCSTR errMsg = _com_error(pscbResult).ErrorMessage();
		std::cout << " > " << errMsg << std::endl;
	}
}
