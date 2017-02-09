#pragma once

#include "SListItem.h"

template<class T>
class CList
{
public:
	SListItem<T>* begin = { nullptr };
	SListItem<T>* end = { nullptr };
	int numOfItems{ 0 };
	void push_back(T* item)
	{
		if (begin == nullptr)
		{
			begin = new SListItem<T>();
			begin->data = item;
			begin->next = nullptr;
			end = begin;
		}
		else
		{
			end->next = new SListItem<T>();
			end->next->data = item;
			end->next->next = nullptr;
			end = end->next;
		}
		numOfItems++;
	}
	T* at(int j)
	{
		SListItem<T>* temp = begin;
		for (int i = 0; i < j; i++)
		{
			temp = temp->next;
		}
		return temp->data;
	}
	~CList()
	{
		SListItem<T>* current = begin;
		SListItem<T>* next = begin->next;
		for (int i = 0; i < numOfItems; i++)
		{
			delete current;
			current = next;
			if (next)
			{
				next = current->next;
			}
		}
	}
};