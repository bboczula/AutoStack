#pragma once

#include <string>
#include <d3d11.h>

class CGeometry
{
private:
	void* data;
	int numOfVertices;
	int elementSize;
	ID3D11Device* device;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
public:
	CGeometry(ID3D11Device* device) : device(device), data(nullptr), numOfVertices(0), elementSize(0), vertexBuffer(nullptr), indexBuffer(nullptr)
	{
	}
	~CGeometry()
	{
		// Remember, data cannot be deleted if it was created somewhere else!
		if (vertexBuffer)
			vertexBuffer->Release();
		if (indexBuffer)
			indexBuffer->Release();
	}
	void setData(void* source, int numberOfItems, int sizeOfElement)
	{
		data = source;
		numOfVertices = numberOfItems;
		elementSize = sizeOfElement;
	}
	void compile()
	{
		D3D11_BUFFER_DESC vertexBufferDescription;
		ZeroMemory(&vertexBufferDescription, sizeof(D3D11_BUFFER_DESC));
		vertexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
		vertexBufferDescription.ByteWidth = elementSize * numOfVertices;
		vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDescription.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = data;
		initData.SysMemPitch = 0;
		initData.SysMemSlicePitch = 0;

		HRESULT result = device->CreateBuffer(&vertexBufferDescription, &initData, &vertexBuffer);
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
		return numOfVertices;
	}
	int getElementSize()
	{
		return elementSize;
	}
	ID3D11Buffer* getVertexBuffer()
	{
		return vertexBuffer;
	}
	ID3D11Buffer** getVertexBufferPtr()
	{
		return &vertexBuffer;
	}
};
