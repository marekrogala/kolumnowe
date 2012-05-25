/*
 * RealUniversalHashmap.h
 *
 *  Created on: 12 Apr 2012
 *      Author: maciek
 */

#ifndef REALUNIVERSALHASHMAP_H_
#define REALUNIVERSALHASHMAP_H_

#include "UniversalHashmap.h"
#include <tr1/unordered_map>
#include <iostream>
#include <vector>
#include <cassert>
#include <map>
#include <cstdlib>
#include "debug.h"
#include "operations.pb.h"
#include "Operation.h"
#include "MemoryManager.h"
#include "ExpressionNode.h"
#include <functional>

using namespace std;

namespace Engine {


class RealUniversalHashmap: public Engine::UniversalMap {
public:
	RealUniversalHashmap(vector<OperationTree::ScanOperation_Type> hash_signature, vector<OperationTree::ScanOperation_Type> value_signature);
	void insert(vector<void*>, vector<void*>, int rows);
	vector<void*> get_result(int&size);

	union Value{
		bool b;
		int32 i;
		double d;
	};


	struct HashStruct {
		vector<Value> t;
	};


	struct hash_name {
	    size_t operator()(const HashStruct&x ) const
	    {
	    	size_t res = 0;
	    	for(int i = 0; i < x.t.size(); ++i) {
	    		res ^= (x.t[i].i);
	    		res ^= static_cast<int32>(x.t[i].d);
	    		res ^= static_cast<int32>(x.t[i].b);
	    	}
	    	return res;
	    }
	};

private:

	vector<OperationTree::ScanOperation_Type> hash_signature_;
	vector<OperationTree::ScanOperation_Type> value_signature_;
	tr1::unordered_map<HashStruct, HashStruct, hash_name> map_;

};

}


#endif /* REALUNIVERSALHASHMAP_H_ */
