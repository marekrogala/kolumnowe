/*
 * BlockSerializer.cpp
 *
 *  Created on: 23 May 2012
 *      Author: maciek
 */

#include "BlockSerializer.h"
#include "ScanOperation.h"
#include <vector>

using namespace std;

namespace Engine {


void printCols(vector<OperationTree::ScanOperation_Type> types, vector<void*> data, int rows) {
	cerr << "SCOLS: " << types.size() << " COLS: " <<data.size() <<" ROWS: "<< rows << "\n";
	for(int row = 0; row < rows; row++){
		for (int column = 0, columns = data.size(); column < columns; ++column) {
			cerr << "ROW: ";
			switch (types[column]) {
				case OperationTree::ScanOperation_Type_INT:
					cerr << * ((int*) data[column] + row) <<" ";
					break;
				case OperationTree::ScanOperation_Type_DOUBLE:
					cerr << * ((double*) data[column] + row) <<" ";
					break;
				case OperationTree::ScanOperation_Type_BOOL:
					cerr <<  * ((bool*) data[column] + row) <<" ";
					break;
				}
		}
		cerr << endl; 
	}
}

BlockSerializer::BlockSerializer() {
}
BlockSerializer::~BlockSerializer() {
}

int BlockSerializer::serializeBlock(
		vector<OperationTree::ScanOperation_Type> types, vector<void*> data,
		int rows, char ** resBuffer) {
	int size = 0;
	for (int i = 0; i < types.size(); i++) {
		switch (types[i]) {
		case SDOUBLE:
			size += sizeof(double) * rows;
			break;
		case SINT:
			size += sizeof(int32) * rows;
			break;
		case SBOOL:
			size += sizeof(bool) * rows;
			break;
		}

	}

	char* buffer = new char[size];
	char * act = buffer;
	for (int i = 0; i < types.size(); ++i) {
		switch (types[i]) {
		case SDOUBLE:
			memcpy(act, data[i], sizeof(double) * rows);
			act += sizeof(double) * rows;
			break;
		case SINT:
			memcpy(act, data[i], sizeof(int32) * rows);
			act += sizeof(int32) * rows;
			break;
		case SBOOL:
			memcpy(act, data[i], sizeof(bool) * rows);
			act += sizeof(bool) * rows;
			break;
		}

	}
	(*resBuffer) = buffer;
	return size;
}

int BlockSerializer::deserializeBlock(
		vector<OperationTree::ScanOperation_Type> types, int bufferSize,
		char * buffer, vector<void*> & result) {

	if(bufferSize == 0) {
		return 0;
	}
	
	int rows;
	int sum = 0;
	for (int i = 0; i < types.size(); i++) {
		switch (types[i]) {
		case SDOUBLE:
			sum += sizeof(double);
			break;
		case SINT:
			sum += sizeof(int32);
			break;
		case SBOOL:
			sum += sizeof(bool);
			break;
		}
	}
	rows = bufferSize / sum;
	for (int i = 0; i < types.size(); i++) {
		void *p;
		switch (types[i]) {
		case SDOUBLE:
			p = new char[sizeof(double) * rows];
			memcpy(p, buffer, sizeof(double) * rows);
			buffer += sizeof(double) * rows;
			result.push_back((void*) p);
			break;
		case SINT:
			p = new char[sizeof(int32) * rows];
			memcpy(p, buffer, sizeof(int32) * rows);
			buffer += sizeof(int32) * rows;
			result.push_back((void*) p);
			break;
		case SBOOL:
			p = new char[sizeof(bool) * rows];
			memcpy(p, buffer, sizeof(bool) * rows);
			buffer += sizeof(bool) * rows;
			result.push_back((void*) p);
			break;
		}

	}

return rows;
}

}
