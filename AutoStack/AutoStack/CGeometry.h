#pragma once

#include <string>
#include <d3d11.h>

template<typename T>
class CGeometry
{
private:
	T* data;
	int numOfVertices;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
public:
	CGeometry(ID3D11Device* device) : data(nullptr), numOfVertices(0), vertexBuffer(nullptr), indexBuffer(nullptr)
	{
	}
	~CGeometry()
	{
		if (data)
			delete data;
		if (vertexBuffer)
			vertexBuffer->Release();
		if (indexBuffer)
			indexBuffer->Release();
	}
	void compile()
	{
		D3D11_BUFFER_DESC vertexBufferDescription;
		ZeroMemory(&vertexBufferDescription, sizeof(D3D11_BUFFER_DESC));
		vertexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDescription.ByteWidth = sizeof(SVertex) * numOfVertices;
		vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDescription.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = quad;
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

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
	}
	int size()
	{
		return numOfVertices
	}
};
