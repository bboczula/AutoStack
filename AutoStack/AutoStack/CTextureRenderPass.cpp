#include "CTextureRenderPass.h"

CTextureRenderPass::CTextureRenderPass(ID3D11Device* device) : CRenderPass(device), texture(nullptr), samplerState(nullptr)
{
	std::cout << "Creating Texture Render Pass..." << std::endl;

	compileShaders();
	createInputLayout();
	createTexture();
}

CTextureRenderPass::~CTextureRenderPass()
{
	if (pixelShader)
	{
		delete pixelShader;
	}

	if (vertexShader)
	{
		delete vertexShader;
	}
}

void CTextureRenderPass::compileShaders()
{
	pixelShader = new PixelShader();
	pixelShader->loadShaderFromFile("ps_texture.hlsl");
	pixelShader->compile();
	pixelShader->create(device);

	vertexShader = new VertexShader();
	vertexShader->loadShaderFromFile("vs_texture.hlsl");
	vertexShader->compile();
	vertexShader->create(device);
}

void CTextureRenderPass::createInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC defaultInputLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
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

void CTextureRenderPass::createTexture()
{
	const int width = 32;
	const int height = 32;
	const int bytesPerPixel = 4;
	int count = 0;

	unsigned char texArray[width * height * bytesPerPixel];
	for (int i = 0; i < width * height * bytesPerPixel; i += bytesPerPixel)
	{
		texArray[i + 0] = count % 2 ? 0 : 255;
		texArray[i + 1] = count++ % 2 ? 255 : 0;
		texArray[i + 2] = 0;
		//texArray[i + 3] = 0;
	}

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = texArray;
	data.SysMemPitch = width * bytesPerPixel;

	HRESULT result = device->CreateTexture2D(&desc, &data, &texture);
	if (SUCCEEDED(result))
	{
		std::cout << "Texture creation succeded." << std::endl;
	}
	else
	{
		std::cout << "ERROR: Texture creation failed!" << std::endl;
	}

	HRESULT srvResult = device->CreateShaderResourceView(texture, NULL, &srv);
	if (SUCCEEDED(srvResult))
	{
		std::cout << "SRV creation succeded." << std::endl;
	}
	else
	{
		std::cout << "ERROR: SRV creation failed!" << std::endl;
	}

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	device->CreateSamplerState(&samplerDesc, &samplerState);
}

void CTextureRenderPass::bind(ID3D11DeviceContext * immediateContext)
{
	std::cout << "BINDING!" << std::endl;
	immediateContext->PSSetShaderResources(0, 1, &srv);
	immediateContext->PSSetSamplers(0, 1, &samplerState);
}
