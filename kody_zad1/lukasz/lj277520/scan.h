#ifndef SCAN_H
#define SCAN_H

#include "dboperation.h"
#include "server.h"
#include "columnStruct.h"

class Scan : public DBOperation {
	private:
		Server* server;
//		int outputSize;
//		columnType* columnsTypes;
		int* sourceColumnIds;
	public:
		Scan(Server* server, int outputSize, columnType* columnsTypes, int* sourceColumnIds);
		~Scan();
		virtual int getOutput(int number, ColumnStruct** destination);
};

#endif
