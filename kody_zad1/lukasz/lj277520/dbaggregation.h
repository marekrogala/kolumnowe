#ifndef DBAGGREGATION_H
#define DBAGGREGATION_H

#include <stdint.h>

#include "columnStruct.h"

class DBAggregation {
	public:
		virtual void aggregate(int rowNum, ColumnStruct* columns, void** destination) = 0;
};

#endif
