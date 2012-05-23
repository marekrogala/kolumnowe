#include "groupBy.h"

#include <cstring>
#include <unordered_map>
#include <utility>

int DBkey_size;


GroupBy::GroupBy(DBOperation* source, int groupBySize, int aggregationsSize, columnType* columnsTypes, int* groupByColumns, DBAggregation** aggregations) : DBOperation(groupBySize+aggregationsSize, columnsTypes), source(source), groupBySize(groupBySize), aggregationsSize(aggregationsSize), groupByColumns(groupByColumns), aggregations(aggregations), groupByReady(false) {
	//map.max_load_factor(0.5f);
	byteGroupBySize = 0;
	for(int i = 0; i < groupBySize; i++) {
		switch(columnsTypes[i]) {
			case INT:
				byteGroupBySize += sizeof(int32);
				break;
			case DOUBLE:
				byteGroupBySize += sizeof(double);
				break;
			case BOOL:
				byteGroupBySize += sizeof(bool);
				break;
		}
	}
	byteAggrSize = 0;
	for(int i = groupBySize; i < outputSize; i++) {
		switch(columnsTypes[i]) {
			case INT:
				byteAggrSize += sizeof(int32);
				break;
			case DOUBLE:
				byteAggrSize += sizeof(double);
				break;
			case BOOL:
				byteAggrSize += sizeof(bool);
				break;
		}
	}
}

GroupBy::~GroupBy() {
	delete source;
	delete[] groupByColumns;
	for(int i = 0; i < aggregationsSize; i++)
		delete aggregations[i];
	delete[] aggregations;
}

void GroupBy::getReady(int number) {
	ColumnStruct* sourceColumnStruct;
	int n = source->getOutput(number, &sourceColumnStruct);
	DBkey_size = byteGroupBySize;
	while(n > 0) {

	void** vgroupByColumns = new void*[groupBySize];

	for(int i = 0; i < groupBySize; i++) {
		sourceColumnStruct->getColumn(groupByColumns[i], &(vgroupByColumns[i]));
	}

	for(int i = 0; i < n; i++) {
		char* key = new char[byteGroupBySize];
		char* p = key;
		for(int j = 0; j < groupBySize; j++) {
			switch(columnsTypes[j]) {
				case INT:
					*(int32*)p = (((int32*)(vgroupByColumns[j]))[i]);
					p += sizeof(int32);
					break;
				case DOUBLE:
					*(double*)p = ((double*)(vgroupByColumns[j]))[i];
					p += sizeof(double);
					break;
				case BOOL:
					*(bool*)p = ((bool*)(vgroupByColumns[j]))[i];
					p += sizeof(bool);
					break;
			}
		}

		std::unordered_map<char*, char*, my_hash, my_eq>::iterator itt = map.find(key);
		char* val;

		if(itt == map.end()) {
			val = new char[byteAggrSize];
			memset(val, 0, byteAggrSize);
		}
		else {
			val = itt->second;
		}

		void* v = (void*)val;
		for(int j = 0; j < aggregationsSize; j++) {
			aggregations[j]->aggregate(i, sourceColumnStruct, &v);
		}
		if(itt == map.end()) {
			map[key] = val;
		}
		else {
			delete[] key;
		}
		//map.insert(std::make_pair(key, val));
//		map[key] = val;
//		delete sourceColumnStruct;
	}
	delete[] vgroupByColumns;
	delete sourceColumnStruct;
	n = source->getOutput(number, &sourceColumnStruct);
	DBkey_size = byteGroupBySize;
	} //while  end

	delete sourceColumnStruct;

	it = map.begin();
	groupByReady = true;
}

int GroupBy::getOutput(int number, ColumnStruct** destination) {
	DBkey_size = byteGroupBySize;

	if(!groupByReady)
		getReady(number);

	// zwracanie wyniku
	ColumnStruct* output = new ColumnStruct(outputSize, columnsTypes);
	int idx;
//	for(int i = 0; i < outputSize; i++) {
		int n;
		void* tmp;
		void** outColumns = new void*[outputSize];

		for(int i = 0; i < outputSize; i++) {
			switch(columnsTypes[i]) {
				case INT:
					outColumns[i] = new int32[number];
					break;
				case DOUBLE:
					outColumns[i] = new double[number];
					break;
				case BOOL:
					outColumns[i] = new bool[number];
					break;
			}
		}

		idx = 0;
		for(it = map.begin() ;it != map.end() && idx < number; ) {
			char* key = it->first;
			char* val = it->second;
			for(int i = 0; i < groupBySize; i++) {
				switch(columnsTypes[i]) {
					case INT:
						((int32*)outColumns[i])[idx] = *((int32*)key);
						key += sizeof(int32);
						break;
					case DOUBLE:
						((double*)outColumns[i])[idx] = *((double*)key);
						key += sizeof(double);
						break;
					case BOOL:
						((bool*)outColumns[i])[idx] = *((bool*)key);
						key += sizeof(bool);
						break;
				}
			}
			for(int i = groupBySize; i < outputSize; i++) {
				switch(columnsTypes[i]) {
					case INT:
						((int32*)outColumns[i])[idx] = *((int32*)val);
						val += sizeof(int32);
						break;
					case DOUBLE:
						((double*)outColumns[i])[idx] = *((double*)val);
						val += sizeof(double);
						break;
					case BOOL:
						((bool*)outColumns[i])[idx] = *((bool*)val);
						val += sizeof(bool);
						break;
				}
			}
			idx++;
			char* dkey = it->first;
			char* dval = it->second;
			it = map.erase(it);
			it = map.begin();
			delete[] dkey;
			delete[] dval;
		}

		for(int i = 0; i < outputSize; i++) {
			output->setColumn(i, idx, outColumns[i]); 
		}
		delete[] outColumns;
//	}
	*destination = output;
	return idx;

}
