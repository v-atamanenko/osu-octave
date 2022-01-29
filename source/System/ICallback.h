#include <cstdio>
#include <vector>

#ifndef __ICALLBACK_H__
#define __ICALLBACK_H__

#define ARGS_PUSH(array, id, argument) array[id] = (void*)(&argument);
#define ARGS_POP(array, id, type) *((type*)array[id])

class ICallback
{
	public:
		virtual void DoCallback(void** args) = 0;
};

#endif

