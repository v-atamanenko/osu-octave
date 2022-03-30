#pragma once

#include <cstdio>
#include <vector>

#define ARGS_PUSH(array, id, argument) array[id] = (void*)(&argument);
#define ARGS_POP(array, id, type) *((type*)array[id])

class ICallback
{
	public:
		virtual void DoCallback(void** args) = 0;
};
