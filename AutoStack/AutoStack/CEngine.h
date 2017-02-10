#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl.h>
#include <iostream>
#include <comdef.h>
#include <string>

#include "IShader.h"
#include "SVertex.h"
#include "SPoint.h"
#include "SPsConstantBuffer.h"

class CEngine
{
private:
	Microsoft::WRL::ComPtr<ID3D11Device> pD3DDevice;
	Microsoft::WRL::ComPtr<IDXGIFactory2> pDxgiFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> pD3DSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pD3DImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pD3DRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthTexture;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthStencilState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pD3DInputLayout;
	HWND* parentWindowHandler;
	PixelShader* defaultPixelShader;
	VertexShader* defaultVertexShader;
private:
	void createDevice()
	{
		DWORD deviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined(_DEBUG)
		deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		std::cout << "Entering CEngine constructor." << std::endl;

		D3D_FEATURE_LEVEL featureLevels[]
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};

		D3D_FEATURE_LEVEL chosenFeatureLevel;

		HRESULT deviceResult = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, deviceFlags, featureLevels,
			ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &pD3DDevice, &chosenFeatureLevel, &pD3DImmediateContext);

		if (SUCCEEDED(deviceResult))
		{
			std::cout << "Device creation succeded." << std::endl;
		}
		else
		{
			std::cout << "ERROR: Device creation failed!" << std::endl;
		}
	}
	void createDxgiFactory()
	{
		HRESULT factoryResult = CreateDXGIFactory2(0, __uuidof(IDXGIFactory2), (void**)(&pDxgiFactory));
		if (SUCCEEDED(factoryResult))
		{
			std::cout << "DXGI Factory creation succeded." << std::endl;
		}
		else
		{
			std::cout << "ERROR: DXGI Factory creation failed!" << std::endl;
		}
	}
	void createSwapChain()
	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDescription;
		ZeroMemory(&swapChainDescription, sizeof(DXGI_SWAP_CHAIN_DESC1));
		swapChainDescription.Width = 800;
		swapChainDescription.Height = 600;
		swapChainDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDescription.Stereo = FALSE;
		swapChainDescription.SampleDesc.Count = 1;
		swapChainDescription.SampleDesc.Quality = 0;
		swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDescription.BufferCount = 1;
		swapChainDescription.Scaling = DXGI_SCALING_STRETCH;
		swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDescription.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT swapChainResult = pDxgiFactory->CreateSwapChainForHwnd(pD3DDevice.Get(),
			*parentWindowHandler, &swapChainDescription, nullptr, nullptr, &pD3DSwapChain);
		if (SUCCEEDED(swapChainResult))
		{
			std::cout << "Swap Chain creation succeded." << std::endl;
		}
		else
		{
			std::cout << "ERROR: Swap Chain creation failed!" << std::endl;
			LPCSTR errMsg = _com_error(swapChainResult).ErrorMessage();
			std::cout << " > " << errMsg << std::endl;
		}
	}
	void createMainRenderTarget()
	{
		ID3D11Texture2D* backBuffer;
		pD3DSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));

		pD3DRenderTargetView.Reset();
		HRESULT renderTargetResult = pD3DDevice->CreateRenderTargetView(backBuffer, NULL, &pD3DRenderTargetView);
		if (SUCCEEDED(renderTargetResult))
		{
			std::cout << "Main Render Target creation succeded." << std::endl;
		}
		else
		{
			std::cout << "ERROR: Main Render Target creation failed!" << std::endl;
			LPCSTR errMsg = _com_error(renderTargetResult).ErrorMessage();
			std::cout << " > " << errMsg << std::endl;
		}
	}
	void setupDepthStencil()
	{
		D3D11_TEXTURE2D_DESC depthStencilTexture;
		ZeroMemory(&depthStencilTexture, sizeof(D3D11_TEXTURE2D_DESC));
		depthStencilTexture.Width = 800;
		depthStencilTexture.Height = 600;
		depthStencilTexture.MipLevels = 1;
		depthStencilTexture.ArraySize = 1;
		depthStencilTexture.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilTexture.SampleDesc.Count = 1;
		depthStencilTexture.SampleDesc.Quality = 0;
		depthStencilTexture.Usage = D3D11_USAGE_DEFAULT;
		depthStencilTexture.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilTexture.CPUAccessFlags = 0;
		depthStencilTexture.MiscFlags = 0;

		HRESULT result = pD3DDevice->CreateTexture2D(&depthStencilTexture, NULL, &depthTexture);
		if (SUCCEEDED(result))
		{
			std::cout << "Depth Stencil creation succeded." << std::endl;
		}
		else
		{
			std::cout << "ERROR: Depth Stencil creation failed!" << std::endl;
			LPCSTR errMsg = _com_error(result).ErrorMessage();
			std::cout << " > " << errMsg << std::endl;
		}

		D3D11_DEPTH_STENCIL_DESC depthDescription;
		depthDescription.DepthEnable = true;
		depthDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthDescription.DepthFunc = D3D11_COMPARISON_LESS;

		depthDescription.StencilEnable = true;
		depthDescription.StencilReadMask = 0xFF;
		depthDescription.StencilWriteMask = 0xFF;

		depthDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		depthDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT stateResult = pD3DDevice->CreateDepthStencilState(&depthDescription, pDepthStencilState.GetAddressOf());
		if (SUCCEEDED(stateResult))
		{
			std::cout << "Depth Stencil State creation succeded." << std::endl;
		}
		else
		{
			std::cout << "ERROR: Depth Stencil State creation failed!" << std::endl;
			LPCSTR errMsg = _com_error(stateResult).ErrorMessage();
			std::cout << " > " << errMsg << std::endl;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		descDSV.Flags = 0;

		HRESULT depthViewResult = pD3DDevice->CreateDepthStencilView(depthTexture.Get(), 0, pDepthStencilView.GetAddressOf());
		if (SUCCEEDED(depthViewResult))
		{
			std::cout << "Depth Stencil View creation succeded." << std::endl;
		}
		else
		{
			std::cout << "ERROR: Depth Stencil View creation failed!" << std::endl;
			LPCSTR errMsg = _com_error(depthViewResult).ErrorMessage();
			std::cout << " > " << errMsg << std::endl;
		}
	}
	void createDefaultShaders()
	{
		defaultVertexShader = new VertexShader();
		defaultVertexShader->loadShaderFromFile("vs.hlsl");
		defaultVertexShader->setEntryPoint("main");
		defaultVertexShader->compile();
		defaultVertexShader->create(pD3DDevice.Get());

		defaultPixelShader = new PixelShader();
		defaultPixelShader->loadShaderFromFile("ps.hlsl");
		defaultPixelShader->setEntryPoint("main");
		defaultPixelShader->compile();
		defaultPixelShader->create(pD3DDevice.Get());
	}
	void setupInputAssemblerStage()
	{
		D3D11_INPUT_ELEMENT_DESC defaultInputLayout[] = 
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		HRESULT result = pD3DDevice->CreateInputLayout(defaultInputLayout, ARRAYSIZE(defaultInputLayout), defaultVertexShader->getBlobPtr()->GetBufferPointer(),
			defaultVertexShader->getBlobPtr()->GetBufferSize(), &pD3DInputLayout);
		if (SUCCEEDED(result))
		{
			std::cout << "Input Layout creation succeded." << std::endl;
		}
		else
		{
			std::cout << "ERROR: Input Layout creation failed!" << std::endl;
			LPCSTR errMsg = _com_error(result).ErrorMessage();
			std::cout << " > " << errMsg << std::endl;
		}

		pD3DImmediateContext->IASetInputLayout(pD3DInputLayout.Get());

		pD3DImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}
	void setupShaders()
	{
		pD3DImmediateContext->VSSetShader(defaultVertexShader->getDxShader(), nullptr, 0);
		pD3DImmediateContext->PSSetShader(defaultPixelShader->getDxShader(), nullptr, 0);
	}
	void setupRasterizerStage()
	{
		D3D11_RASTERIZER_DESC rasterizerDescription;
		ZeroMemory(&rasterizerDescription, sizeof(D3D11_RASTERIZER_DESC));
		rasterizerDescription.AntialiasedLineEnable = false;
		rasterizerDescription.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
		rasterizerDescription.DepthBias = 0;
		rasterizerDescription.DepthBiasClamp = 0.0f;
		rasterizerDescription.DepthClipEnable = true;
		rasterizerDescription.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rasterizerDescription.FrontCounterClockwise = false;
		rasterizerDescription.MultisampleEnable = false;
		rasterizerDescription.ScissorEnable = false;
		rasterizerDescription.SlopeScaledDepthBias = 0.0f;
		ID3D11RasterizerState* state;
		pD3DDevice->CreateRasterizerState(&rasterizerDescription, &state);
		pD3DImmediateContext->RSSetState(state);
	}
	void setupViewport()
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.Width = 800;
		viewport.Height = 600;
		viewport.MaxDepth = 1.0f;
		pD3DImmediateContext->RSSetViewports(1, &viewport);
	}
public:
	CEngine(HWND* hwnd) : parentWindowHandler(hwnd)
	{
		createDevice();
		createDxgiFactory();
		createSwapChain();
		createMainRenderTarget();
		setupDepthStencil();
		createDefaultShaders();
		setupInputAssemblerStage();
		setupRasterizerStage();
		setupViewport();
		setupShaders();
		clearMainRenderTarget();
		present();
	}
	void clearMainRenderTarget()
	{
		float color[4]{ 0.2431f, 0.5843f, 0.7568f, 1.0f };
		pD3DImmediateContext->ClearRenderTargetView(pD3DRenderTargetView.Get(), color);
		pD3DImmediateContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	void present()
	{
		pD3DSwapChain->Present(0, 0);
	}
	void drawFrame()
	{
		// Draw Frame
		const int THICKNESS{ 3 };
		const int CANVAS_WIDTH{ 800 };
		const int CANVAS_HEIGHT{ 600 };
		const int LEFT_MARGIN{ 5 };
		const int RIGHT_MARGIN{ 5 };
		const int TOP_MARGIN{ 5 };
		const int BOTTOM_MARGIN{ 5 };

		drawQuad(SPoint{ LEFT_MARGIN, CANVAS_HEIGHT - RIGHT_MARGIN, 10 }, THICKNESS, CANVAS_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN));
		drawQuad(SPoint{ CANVAS_WIDTH - TOP_MARGIN, CANVAS_HEIGHT - RIGHT_MARGIN, 10 }, THICKNESS, CANVAS_HEIGHT - (TOP_MARGIN + BOTTOM_MARGIN));
		drawQuad(SPoint{ LEFT_MARGIN, CANVAS_HEIGHT - RIGHT_MARGIN, 10 }, CANVAS_WIDTH - (LEFT_MARGIN + RIGHT_MARGIN), THICKNESS);
		drawQuad(SPoint{ LEFT_MARGIN, TOP_MARGIN, 10 }, CANVAS_WIDTH - (LEFT_MARGIN + RIGHT_MARGIN), THICKNESS);

		// Draw lines
		int numOfSteps = 10;
		int step = static_cast<int>(595 / numOfSteps);
		for (int i = 0; i < numOfSteps; i++)
		{
			drawQuad(SPoint{ LEFT_MARGIN, 595 - (i * step), 10 }, CANVAS_WIDTH - (LEFT_MARGIN + RIGHT_MARGIN), 1);
		}
	}
	void drawQuad(SPoint anchor, int width, int height)
	{
		float x = static_cast<float>(anchor.x);
		float y = static_cast<float>(anchor.y);

		const unsigned int NUM_OF_VERTICES{ 4 };

		SVertex quad[NUM_OF_VERTICES];
		quad[0] = { x, y, 0.5f };
		quad[1] = { x, y - height, 0.5f };
		quad[3] = { x + width, y - height, 0.5f };
		quad[2] = { x+width, y, 0.5f };

		D3D11_BUFFER_DESC vertexBufferDescription;
		ZeroMemory(&vertexBufferDescription, sizeof(D3D11_BUFFER_DESC));
		vertexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDescription.ByteWidth = sizeof(SVertex) * NUM_OF_VERTICES;
		vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDescription.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = quad;
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		ID3D11Buffer* vertexBuffer;
		HRESULT result = pD3DDevice->CreateBuffer(&vertexBufferDescription, &initData, &vertexBuffer);
		if (SUCCEEDED(result))
		{
#if defined _DEBUG
			std::cout << "Vertex Buffer creation succeded." << std::endl;
#endif
		}
		else
		{
			std::cout << "ERROR: Vertex Buffer creation failed!" << std::endl;
			LPCSTR errMsg = _com_error(result).ErrorMessage();
			std::cout << " > " << errMsg << std::endl;
		}

		// Create Constant Buffers
		D3D11_BUFFER_DESC constantBufferDescription;
		ZeroMemory(&constantBufferDescription, sizeof(D3D11_BUFFER_DESC));

		constantBufferDescription.ByteWidth = sizeof(SPsConstantBuffer);
		constantBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		SPsConstantBuffer psData;
		psData.r = 0.3019f;
		psData.g = 0.7019f;
		psData.b = 0.3450f;

		D3D11_SUBRESOURCE_DATA initData2;
		initData2.pSysMem = &psData;
		initData2.SysMemPitch = 0;
		initData2.SysMemSlicePitch = 0;

		ID3D11Buffer* constantBuffer;
		HRESULT pscbResult = pD3DDevice->CreateBuffer(&constantBufferDescription, &initData2, &constantBuffer);
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

		const UINT stride = sizeof(SVertex);
		UINT offset = 0;
		pD3DImmediateContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		pD3DImmediateContext->PSSetConstantBuffers(0, 1, &constantBuffer);
		pD3DImmediateContext->OMSetRenderTargets(1, pD3DRenderTargetView.GetAddressOf(), pDepthStencilView.Get());
		pD3DImmediateContext->Draw(4, 0);

		// Release the buffers
		vertexBuffer->Release();
		constantBuffer->Release();
	}
};