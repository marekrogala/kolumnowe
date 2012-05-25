/*
 * MemoryManager.cpp
 *
 *  Created on: 9 Apr 2012
 *      Author: maciek
 */

#include "MemoryManager.h"

namespace Engine {

MemoryManager::MemoryManager(int max_rows) : max_rows_(max_rows) {
}

void* MemoryManager::allocate_normal(int size) {
	return malloc(size);
}

void* MemoryManager::allocate_normal() {
	allocate_normal(sizeof(double) * max_rows());
}

int MemoryManager::buffer_size_bytes() {
	return sizeof(double) * max_rows();
}

void MemoryManager::free_buffer(void* buff) {
	free(buff);
}

int MemoryManager::max_rows() {
	return max_rows_;
}

}
