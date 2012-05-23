/*
 * MemoryManager.h
 *
 *  Created on: 9 Apr 2012
 *      Author: maciek
 */

#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_

#include <iostream>
#include <cstdlib>
#include "debug.h"

namespace Engine {

class MemoryManager {
public:
	MemoryManager(int max_rows);
	void* allocate_normal(int size);
	void* allocate_normal();
	void free_buffer(void* buff);
	int max_rows();
	int buffer_size_bytes();
private:
	const int max_rows_;
};

}

#endif /* MEMORYMANAGER_H_ */
