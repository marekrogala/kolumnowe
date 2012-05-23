#include "aggre_count.h"
#include "dbexpression.h"

DBAggregation_count::DBAggregation_count() {
}

DBAggregation_count::~DBAggregation_count() {
}

void DBAggregation_count::aggregate(int rowNum, ColumnStruct* columns, void** destination) {
	char* p = (char*)(*destination);
	*((int32*)p) = *((int32*)p) + 1;
	*destination = (void*)((char*)*destination + sizeof(int32));
}
