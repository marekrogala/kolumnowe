#ifndef GROUPBY_H
#define GROUPBY_H

#include <unordered_map>
#include <cstring>

#include "dboperation.h"
#include "dbaggregation.h"
#include "columnStruct.h"

extern int DBkey_size;

struct my_hash {
	inline size_t operator()(char* val) const {
		const uint32_t p = 16777619;
		uint32_t hash = 2166136261u;

		for (int i = 0; i < DBkey_size; i++)
			hash = (hash ^ ((unsigned char*)val)[i]) * p;

		hash += hash << 13;
		hash ^= hash >> 7;
		hash += hash << 3;
		hash ^= hash >> 17;
		hash += hash << 5;
		return hash;
	}
};

struct my_eq {
	inline bool operator()(const char* s1, char* s2) const {
		return memcmp((void*)s1, (void*)s2, DBkey_size) == 0;
	}
};

class GroupBy : public DBOperation {
	private:
		bool groupByReady;
		DBOperation* source;
		int* groupByColumns;
		int groupBySize;
		DBAggregation** aggregations;
		int aggregationsSize;

		int byteGroupBySize;
		int byteAggrSize;

		std::unordered_map<char*, char*, my_hash, my_eq> map;
		std::unordered_map<char*, char*, my_hash, my_eq>::iterator it;

		void getReady(int number);
	public:
		GroupBy(DBOperation* source, int groupBySize, int aggregationsSize, columnType* columnsTypes, int* groupByColumns, DBAggregation** aggregations);
		~GroupBy();
		virtual int getOutput(int number, ColumnStruct** destination);
};

#endif
