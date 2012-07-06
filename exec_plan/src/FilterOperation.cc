/*
 * FilterOperation.cpp
 *
 *  Created on: 9 Apr 2012
 *      Author: maciek
 */

#include "FilterOperation.h"
#include <iostream>
using namespace std;

// TODO tutaj możesz walnąć tą fajną optymalizacje że najpierw przypisujesz a pozniej patrzysz czy się nie udało ale to chyba w IF jednak a nie tu

#define do_filter(typ, aim_typ, buff_type, y, warunek, res_, n_ile_mam, data_) if ((typ) == (aim_typ)) { for(int j = 0; j < (y); ++j) { \
				if (warunek[j]) {((buff_type*)(res_))[(n_ile_mam)++] = ((buff_type*)(data_))[j]; } } }

namespace Engine {

OperationTree::ScanOperation_Type FilterOperation::get_source_type(int idx) {
	return source_types_[idx];
}

FilterOperation::FilterOperation(NodeEnvironmentInterface * nei, const OperationTree::FilterOperation & node, MemoryManager * mem_manager):
		Operation(nei), node_(node), mem_manager_(mem_manager) {
}

InitRes FilterOperation::init(bool &group_flag) {
//	if (debug) cerr << "FilterOperation::init()" << endl;

    source_ = OperationBuilder::build(nei_, node_.source(), mem_manager_);
    InitRes r = source_ -> init(group_flag);
		source_types_ = r.first;

    for(int i = 0; i < source_types_.size(); ++i) {
    	buffered_.push_back(mem_manager_ -> allocate_normal());
        res_.push_back(mem_manager_ -> allocate_normal(2 * mem_manager_ -> buffer_size_bytes()));
    }

    expr_ = new ExpressionNode(&(node_.expression()), this, mem_manager_);
    expr_ -> init();

    buffered_size_ = 0;
		if (group_flag){
			source_ = r.second;
			return make_pair(source_types_, this);
		} else return make_pair(source_types_, r.second);
}

vector<void*> FilterOperation::pull(int &rows) {
//	if (debug) cerr << "FilterOperation::pull " << rows << endl;
	int x = rows;
	int y;
	int ile_mam = 0;

	if (buffered_size_ > 0) {
		for(int i = 0; i < source_types_.size(); ++i) {
			memcpy(res_[i], buffered_[i], mem_manager_ -> buffer_size_bytes());
		}
	}

	ile_mam = buffered_size_;
	buffered_size_ = 0;

	while (true) {
		y = x;
		data_ = source_ -> pull(y);
		bool* warunek = (bool*)expr_ -> pull(y);

		int n_ile_mam;

		for(int i = 0; i < source_types_.size(); ++i) {
			n_ile_mam = ile_mam;
			void *a, *b;
			a = res_[i];
			b = data_[i];
			do_filter(source_types_[i], SINT, int32, y, warunek, a, n_ile_mam, b);
			do_filter(source_types_[i], SDOUBLE, double, y, warunek, a, n_ile_mam, b);
			do_filter(source_types_[i], SBOOL, bool, y, warunek, a, n_ile_mam, b);

			if (n_ile_mam > rows) {
				buffered_size_ = n_ile_mam - rows;
				if (source_types_[i] == SINT) {
					int32 *a, *b;
					a = (int32*)buffered_[i];
					b = (int32*)res_[i];
					for(int j = rows; j < n_ile_mam; ++j) a[j - rows] = b[j];
				}
				if (source_types_[i] == SDOUBLE) {
					double *a, *b;
					a = (double*)buffered_[i];
					b = (double*)res_[i];
					for(int j = rows; j < n_ile_mam; ++j) a[j - rows] = b[j];
				}

				if (source_types_[i] == SBOOL) {
					bool *a, *b;
					a = (bool*)buffered_[i];
					b = (bool*)res_[i];
					for(int j = rows; j < n_ile_mam; ++j) a[j - rows] = b[j];
				}
			}

//			if (debug) cerr << "FILTER OPER " << i << " " << n_ile_mam << endl;
			if (i + 1 == source_types_.size()) { ile_mam = n_ile_mam; }
		}

		if (ile_mam >= rows) {
			break;
		}

		if (y == 0) {
			break;
		}
	}

	rows = min(rows, ile_mam);

	return res_;
}

void* FilterOperation::get_data(int idx) {
	return data_[idx];
}

}

