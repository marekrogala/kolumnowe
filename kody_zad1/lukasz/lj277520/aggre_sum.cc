#include "aggre_sum.h"
#include "dbexpression.h"

DBAggregation_sum::DBAggregation_sum(int colNum) : colNum(colNum) {
}

DBAggregation_sum::~DBAggregation_sum() {
}

void DBAggregation_sum::aggregate(int rowNum, ColumnStruct* columns, void** destination) {
	char* p = (char*)(*destination);
	void* col;
	columns->getColumn(colNum, &col);
	switch(columns->types[colNum]) {
		case INT:
			*((int32*)p) = *((int32*)p) + ((int32*)col)[rowNum];
			*destination = (void*)((char*)*destination + sizeof(int32));
			break;
		case DOUBLE:
			*((double*)p) = *((double*)p) + ((double*)col)[rowNum];
			*destination = (void*)((char*)*destination + sizeof(double));
			break;
		case BOOL:
			*((int32*)p) = *((int32*)p) + ((bool*)col)[rowNum];
			*destination = (void*)((char*)*destination + sizeof(int32));
			break;

	}
}
