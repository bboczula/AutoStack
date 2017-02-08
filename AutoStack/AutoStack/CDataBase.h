#pragma once

#include "SStock.h"

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

template<class T>
class CList
{
public:
	SListItem<T>* begin = { nullptr };
	SListItem<T>* end = { nullptr };
};