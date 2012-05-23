#include "columnStruct.h"
#include <cstring>

ColumnStruct::ColumnStruct(int size, columnType* ctypes) : size(size), nrows(0) {
	types = new columnType[size];
	memcpy(types, ctypes, size*sizeof(columnType));
	columns = new void*[size];
//	sizes = new int[size];
	for(int i = 0; i < size; i++) {
//		sizes[i] = 0;
		columns[i] = 0;
	}
}

ColumnStruct::~ColumnStruct() {
	for(int i = 0; i < size; i++) {
		deleteColumn(i);
	}
	delete[] columns;
	delete[] types;
//	delete[] sizes;
}

void ColumnStruct::deleteColumn(int columnId) {
	switch(types[columnId]) {
		case INT:
			delete[] (int32*)columns[columnId];
			break;
		case DOUBLE:
			delete[] (double*)columns[columnId];
			break;
		case BOOL:
			delete[] (bool*)columns[columnId];
			break;
	}
}

void ColumnStruct::setColumn(int columnId, int number, void* source) {
	deleteColumn(columnId);
	columns[columnId] = source;
	// assert?
	nrows = number;
	//sizes[columnId] = number;
}

int ColumnStruct::getColumn(int columnId, void** destination) {
	*destination = columns[columnId];
	return nrows;
//	return sizes[columnId];
}


int ColumnStruct::getColumnCopy(int columnId, void** destination) {
	//int n = sizes[columnId];
	int n = nrows;
	int size;
	void* tmp;
	switch(types[columnId]) {
		case INT:
			size = sizeof(int32) * n;
			tmp = new int32[n];
			break;
		case DOUBLE:
			size = sizeof(double) * n;
			tmp = new double[n];
			break;
		case BOOL:
			size = sizeof(bool) * n;
			tmp = new bool[n];
			break;
	}
	memcpy(tmp, columns[columnId], size);
	*destination = tmp;
	return n;
}
