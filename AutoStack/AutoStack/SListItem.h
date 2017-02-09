#pragma once

template<class T>
class SListItem
{
public:
	T* data{ nullptr };
	SListItem* next{ nullptr };
	SListItem()
	{
		data = new T();
	}
	~SListItem()
	{
		delete data;
	}
};