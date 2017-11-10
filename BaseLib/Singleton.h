#ifndef _SINGLETON
#define _SINGLETON

#pragma once

#include "Base.h"

template <class TYPE> class Singleton
{
public:
	Singleton()
	{
		_ASSERT(NULL == _Instance);
		_Instance = static_cast<TYPE*>(this);
	}
	virtual ~Singleton()
	{
		_ASSERT(NULL != _Instance);
		_Instance = NULL;
	}

	static TYPE* GetInstance(void)
	{
		return _Instance;
	}

private:
	static TYPE* _Instance;
};

template <class TYPE> TYPE* Singleton<TYPE>::_Instance = NULL;

#endif
