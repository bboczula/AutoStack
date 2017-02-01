#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl.h>
#include <iostream>

class CEngine
{
private:
	Microsoft::WRL::ComPtr<ID3D11Device> pD3DDevice;
	Microsoft::WRL::ComPtr<IDXGIFactory2> pDxgiFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain1> pD3DSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pD3DImmediateContext;
public:
	CEngine(HWND* parentWindowHandler)
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

		HRESULT factoryResult = CreateDXGIFactory2(0, __uuidof(IDXGIFactory2), (void**)(&pDxgiFactory));
		if (SUCCEEDED(factoryResult))
		{
			std::cout << "DXGI Factory creation succeded." << std::endl;
		}
		else
		{
			std::cout << "ERROR: DXGI Factory creation failed!" << std::endl;
		}

		DXGI_SWAP_CHAIN_DESC1 swapChainDescription;
		ZeroMemory(&swapChainDescription, sizeof(DXGI_SWAP_CHAIN_DESC1));
		swapChainDescription.Width = 800;
		swapChainDescription.Height = 600;
		swapChainDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDescription.Stereo = FALSE;
		swapChainDescription.SampleDesc.Count = 0;
		swapChainDescription.SampleDesc.Quality = 0;
		swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDescription.BufferCount = 1;
		swapChainDescription.Scaling = DXGI_SCALING_STRETCH;
		swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDescription.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT swapChainResult = pDxgiFactory->CreateSwapChainForHwnd(pD3DDevice.Get(),
			*parentWindowHandler, &swapChainDescription, nullptr, nullptr, &pD3DSwapChain);
	}
};