#include "compute.h"

#include <cstring>


Compute::Compute(DBOperation* source, int outputSize, columnType* columnsTypes, DBExpression** expr) : DBOperation(outputSize, columnsTypes), source(source), expr(expr) {
}

Compute::~Compute() {
	delete source;
//	delete[] columnsTypes;
	for(int i = 0; i < outputSize; i++)
		delete expr[i];
	delete[] expr;
}

int Compute::getOutput(int number, ColumnStruct** destination) {
	ColumnStruct* output = new ColumnStruct(outputSize, columnsTypes);
	ColumnStruct* sourceColumnStruct;
	int n = source->getOutput(number, &sourceColumnStruct);
	if(n == 0) {
		delete sourceColumnStruct;
		*destination = output;
		return 0;
	}
	for(int i = 0; i < outputSize; i++) {
		void* tmp;
		expr[i]->getResult(sourceColumnStruct, &tmp);
		output->setColumn(i, n, tmp); 
	}
	delete sourceColumnStruct;
	*destination = output;
	return n;
}
