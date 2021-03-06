#pragma once

#include <string>
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include <iostream>
#include <comdef.h>

#define DEFAULT_ENTRY_POINT "main"
#define DEFAULT_PS_SHADER_MODEL "ps_4_0"
#define DEFAULT_VS_SHADER_MODEL "vs_4_0"

class IShader
{
protected:
	std::string source;
	std::string entryPoint;
	std::string shaderModel;
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob;
	void addLine(std::string line)
	{
		source += line + "\n";
	}
public:
	void compile()
	{
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
		flags |= D3DCOMPILE_DEBUG;
#endif

		ID3DBlob* errorBlob = 0;
		HRESULT result = D3DCompile2(source.c_str(), source.length(), nullptr, nullptr,
			nullptr, entryPoint.c_str(), shaderModel.c_str(), flags, 0, 0, nullptr, 0, &shaderBlob, &errorBlob);

		if (FAILED(result))
		{
			if (errorBlob)
			{
				std::cout << "ERROR: Shader creation failed!" << std::endl;
				LPCSTR errMsg = _com_error(result).ErrorMessage();
				std::cout << " > " << errMsg << std::endl;

				MessageBoxA(nullptr, (char*)errorBlob->GetBufferPointer(), "Shader", MB_OK);
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}

			if (shaderBlob)
				shaderBlob.Get()->Release();
		}
	}
	void setEntryPoint(std::string entryPoint)
	{
		this->entryPoint = entryPoint;
	}
	void setShaderModel(std::string shaderModel)
	{
		this->shaderModel = shaderModel;
	}
	ID3DBlob* getBlobPtr()
	{
		return shaderBlob.Get();
	}
	void loadShaderFromFile(std::string filename)
	{
		std::string line;
		std::ifstream objFile(filename);
		if (objFile.is_open())
		{
			while (std::getline(objFile, line))
			{
				addLine(line);
			}
			objFile.close();
		}
		else
		{
			std::cout << "ERROR: cannot open a file" << std::endl;
		}
	}
};

class VertexShader : public IShader
{
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> dxVertexShader;
public:
	VertexShader()
	{
		setEntryPoint(DEFAULT_ENTRY_POINT);
		setShaderModel(DEFAULT_VS_SHADER_MODEL);
	}
	~VertexShader()
	{
		if (dxVertexShader)
		{
			dxVertexShader.Get()->Release();
			dxVertexShader.Reset();
		}
	}
	void create(ID3D11Device* device)
	{
		device->CreateVertexShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, dxVertexShader.GetAddressOf());
	}
	ID3D11VertexShader* getDxShader()
	{
		return dxVertexShader.Get();
	}
};

class PixelShader : public IShader
{
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> dxPixelShader;
public:
	PixelShader()
	{
		setEntryPoint(DEFAULT_ENTRY_POINT);
		setShaderModel(DEFAULT_PS_SHADER_MODEL);
	}
	~PixelShader()
	{
		if (dxPixelShader)
		{
			dxPixelShader.Get()->Release();
			dxPixelShader.Reset();
		}
	}
	void create(ID3D11Device* device)
	{
		device->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), NULL, dxPixelShader.GetAddressOf());
	}
	ID3D11PixelShader* getDxShader()
	{
		return dxPixelShader.Get();
	}
};