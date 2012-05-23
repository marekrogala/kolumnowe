#include "filter.h"

#include <cstring>

Filter::Filter(DBOperation* source, int outputSize, columnType* columnsTypes, DBExpression* expr) : DBOperation(outputSize, columnsTypes), source(source), expr(expr) { 
}

Filter::~Filter() {
	delete source;
	delete expr;
}

int Filter::getOutput(int number, ColumnStruct** destination) {
	int outputRows = 0;
	ColumnStruct* output = new ColumnStruct(outputSize, columnsTypes);
	for(int i = 0; i < outputSize; i++) {
		switch(columnsTypes[i]) {
			case INT: {
				int32* col = new int32[number];
				output->setColumn(i, number, (void*)col);
			}
				break;
			case DOUBLE: {
				double* col = new double[number];
				output->setColumn(i, number, (void*)col);
			}
				break;
			case BOOL: {
				bool* col = new bool[number];
				output->setColumn(i, number, (void*)col);
			}
				break;
		}
	}
	while(2*outputRows < number) {
		ColumnStruct* sourceColumnStruct;
		int n = source->getOutput((number - outputRows), &sourceColumnStruct);
		if(n == 0) {
			delete sourceColumnStruct;
			break;
		}
		int outputSize  = sourceColumnStruct->size;

		columnType* columnsTypes = sourceColumnStruct->types;
	//	if(output == NULL) {
	//		output = new ColumnStruct(outputSize, columnsTypes);
	//		for(int i = 0; i < outputSize; i++) {
	//			switch(columnsTypes[i]) {
	//				case INT: {
	//					int32* col = new int32[number];
	//					output->setColumn(i, number, (void*)col);
	//				}
	//					break;
	//				case DOUBLE: {
	//					double* col = new double[number];
	//					output->setColumn(i, number, (void*)col);
	//				}
	//					break;
	//				case BOOL: {
	//					bool* col = new bool[number];
	//					output->setColumn(i, number, (void*)col);
	//				}
	//					break;
	//			}
	//		}
	//	}

		int j;

		void* result;
		expr->getResult(sourceColumnStruct, &result);
		bool* bresult = (bool*)result;

		for(int i = 0; i < outputSize; i++) {
			void* in;
			sourceColumnStruct->getColumn(i, &in);

			j = outputRows;

			void* vcolOut;
			output->getColumn(i, &vcolOut);

			switch(columnsTypes[i]) {
				case INT: {
					int32* colOut = (int32*)vcolOut;
					int32* colIn = (int32*)in;
					for(int i = 0; i < n; i++) {
						if(bresult[i]) {
							colOut[j] = colIn[i];
							j++;
						}
					}
				  }
					break;
				case DOUBLE: {
					double* colOut = (double*)vcolOut;
					double* colIn = (double*)in;
					for(int i = 0; i < n; i++) {
						if(bresult[i]) {
							colOut[j] = colIn[i];
							j++;
						}
					}
				 }
					break;
				case BOOL: {
					bool* colOut = (bool*)vcolOut;
					bool* colIn = (bool*)in;
					for(int i = 0; i < n; i++) {
						if(bresult[i]) {
							colOut[j] = colIn[i];
							j++;
						}
					}
			   }
				break;
			}
		}
		outputRows = j;
		output->nrows = outputRows;
		delete[] bresult;
		delete sourceColumnStruct;
	}
	output->nrows = outputRows;
	*destination = output;
	return outputRows;
}
