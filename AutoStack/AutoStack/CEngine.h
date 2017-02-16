#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl.h>
#include <iostream>
#include <comdef.h>
#include <string>

#include "IShader.h"
#include "SVertex.h"
#include "SColorVertex.h"
#include "SPoint.h"
#include "SPsConstantBuffer.h"
#include "CSolidColorRenderPass.h"
#include "CColorRenderPass.h"
#include "CGeometry.h"

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
	HWND* parentWindowHandler;
	CRenderPass* currentRenderPass;
	CSolidColorRenderPass* solidColorRenderPass;
	CColorRenderPass* colorRenderPass;
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
	void createRenderPass()
	{
		solidColorRenderPass = new CSolidColorRenderPass(pD3DDevice.Get());
		colorRenderPass = new CColorRenderPass(pD3DDevice.Get());

		currentRenderPass = solidColorRenderPass;
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
	void setupInputAssemblerStage()
	{
		pD3DImmediateContext->IASetInputLayout(currentRenderPass->getInputLayout());
		pD3DImmediateContext->IASetPrimitiveTopology(*(currentRenderPass->getTopology()));
	}
	void setupShaders()
	{
		pD3DImmediateContext->VSSetShader(currentRenderPass->getVertexShader()->getDxShader(), nullptr, 0);
		pD3DImmediateContext->PSSetShader(currentRenderPass->getPixelShader()->getDxShader(), nullptr, 0);
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
		createRenderPass();
		createMainRenderTarget();
		setupDepthStencil();
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

		solidColorRenderPass->setColor(0, 255, 255);
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

		CGeometry geometry(pD3DDevice.Get());
		geometry.setData(quad, NUM_OF_VERTICES, sizeof(SVertex));
		geometry.compile();

		// Draw Quad
		pD3DImmediateContext->UpdateSubresource(currentRenderPass->getPsConstantBuffer(), 0, NULL, solidColorRenderPass->getConstantBufferData(), 0, 0);
		draw(&geometry, currentRenderPass);
	}
	void drawColorQuad(SPoint anchor, int width, int height)
	{
		float x = static_cast<float>(anchor.x);
		float y = static_cast<float>(anchor.y);

		const unsigned int NUM_OF_VERTICES{ 4 };

		SColorVertex quad[NUM_OF_VERTICES];
		quad[0] = { x, y, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f };
		quad[1] = { x, y - height, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f };
		quad[3] = { x + width, y - height, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f };
		quad[2] = { x + width, y, 0.5f, 1.0f, 1.0f, 0.0f, 1.0f };

		CGeometry geometry(pD3DDevice.Get());
		geometry.setData(quad, NUM_OF_VERTICES, sizeof(SColorVertex));

		draw(&geometry, colorRenderPass);
	}
	void draw(CGeometry* geometry, CRenderPass* renderPass)
	{
		const UINT stride = geometry->getElementSize();
		UINT offset = 0;
		pD3DImmediateContext->IASetVertexBuffers(0, 1, geometry->getVertexBufferPtr(), &stride, &offset);
		if (renderPass->isPsCbPresent())
		{
			std::cout << "PS CB Present" << std::endl;
			//renderPass->setColor(255, std::rand() % 255, 0);
			// Here you have to update constant buffer
			//pD3DImmediateContext->UpdateSubresource(renderPass->getPsConstantBuffer(), 0, NULL, renderPass->getConstantBufferData(), 0, 0);

			// ----------
			ID3D11Buffer* temp = renderPass->getPsConstantBuffer();
			pD3DImmediateContext->PSSetConstantBuffers(0, 1, &temp);
		}
		else
		{
			std::cout << "ERROR: PS CB NOT present!" << std::endl;
		}
		pD3DImmediateContext->OMSetRenderTargets(1, pD3DRenderTargetView.GetAddressOf(), pDepthStencilView.Get());
		pD3DImmediateContext->Draw(4, 0);
	}
};