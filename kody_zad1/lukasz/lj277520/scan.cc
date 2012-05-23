#include "scan.h"

Scan::Scan(Server* server, int outputSize, columnType* columnsTypes, int* sourceColumnIds) : DBOperation(outputSize, columnsTypes), server(server), sourceColumnIds(sourceColumnIds) {
}

//Scan::Scan(Server *server) : server(server) {
//}

Scan::~Scan() {
//	delete[] columnsTypes;
	delete[] sourceColumnIds;
}

int Scan::getOutput(int number, ColumnStruct** destination) {
	ColumnStruct* output = new ColumnStruct(outputSize, columnsTypes);
	int n;
	void* tmp;
	for(int i = 0; i < outputSize; i++) {
		switch(columnsTypes[i]) {
			case INT:
				tmp = new int32[number];
				n = server->GetInts(sourceColumnIds[i], number, (int32*)tmp);
				break;
			case DOUBLE:
				tmp = new double[number];
				n = server->GetDoubles(sourceColumnIds[i], number, (double*)tmp);
				break;
			case BOOL:
				tmp = new bool[number];
				n = server->GetByteBools(sourceColumnIds[i], number, (bool*)tmp);
				break;
		}
		output->setColumn(i, n, tmp);
	}
	*destination = output;
	return n;
}
