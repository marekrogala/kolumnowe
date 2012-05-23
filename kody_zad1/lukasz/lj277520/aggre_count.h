#ifndef DBAGGREGATION_COUNT_H
#define DBAGGREGATION_COUNT_H

#include <stdint.h>

#include "dbaggregation.h"
#include "columnStruct.h"

class DBAggregation_count : public DBAggregation {
	public:
		DBAggregation_count();
		~DBAggregation_count();
		virtual void aggregate(int rowNum, ColumnStruct* columns, void** destination);
};

#endif
