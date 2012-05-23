#ifndef DBAGGREGATION_SUM_H
#define DBAGGREGATION_SUM_H

#include <stdint.h>

#include "dbaggregation.h"
#include "columnStruct.h"

class DBAggregation_sum : public DBAggregation {
	private:
		int colNum;
	public:
		DBAggregation_sum(int colNum);
		~DBAggregation_sum();
		virtual void aggregate(int rowNum, ColumnStruct* columns, void** destination);
};

#endif
