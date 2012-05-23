#ifndef COLUMNSTRUCT_H
#define COLUMNSTRUCT_H

#include <stdint.h>

typedef int32_t int32;

enum columnType {INT, DOUBLE, BOOL};

class ColumnStruct {
	private:

		void deleteColumn(int columnId);
	public:
		ColumnStruct(int size, columnType* ctypes);
		~ColumnStruct();

		void setColumn(int columnId, int number, void* source);
		int getColumn(int columnId, void** destination);
		int getColumnCopy(int columnId, void** destination);

		int nrows;
		//int* sizes;
		columnType* types;
		void** columns;
		int size;
};

#endif
