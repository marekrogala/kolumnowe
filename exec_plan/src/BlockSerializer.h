/*
 * BlockSerializer.h
 *
 *  Created on: 23 May 2012
 *      Author: maciek
 */

#ifndef BLOCKSERIALIZER_H_
#define BLOCKSERIALIZER_H_

#include "operations.pb.h"

namespace Engine {
class BlockSerializer {
public:
	BlockSerializer();
	virtual ~BlockSerializer();
	int serializeBlock(vector<OperationTree::ScanOperation_Type> types, vector<void*> data, int rows, char ** resBuffer);
	int deserializeBlock(vector<OperationTree::ScanOperation_Type> types, int bufferSize, char * buffer, vector<void*> & result);
};
}

#endif /* BLOCKSERIALIZER_H_ */
