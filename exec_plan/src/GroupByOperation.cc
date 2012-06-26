/*
 * GroupByOperation.cpp
 *
 *  Created on: 9 Apr 2012
 *      Author: maciek
 */

#include "GroupByOperation.h"
#include "UniversalHashmap.h"

#define init_hashmap_1(mapka, gtype1, aim_gtype1, buff_gtype1, atype1, aim_atype1, buff_atype1) \
	if (gtype1 == aim_gtype1 && aim_gtype1 == SBOOL && atype1 == aim_atype1) mapka = new BoolHashmap<buff_atype1>(); \
	else \
	if (gtype1 == aim_gtype1 && atype1 == aim_atype1) mapka = new UniversalHashmap<buff_gtype1, buff_atype1>();

#define init_hashmap_2(mapka, gtype1, aim_gtype1, buff_gtype1, atype1, aim_atype1, buff_atype1, atype2, aim_atype2, buff_atype2) \
	if (gtype1 == aim_gtype1 && aim_gtype1 == SBOOL && atype1 == aim_atype1 && atype2 == aim_atype2) mapka = new BoolHashmap2<buff_atype1, buff_atype2>(); \
	else \
	if (gtype1 == aim_gtype1 && atype1 == aim_atype1 && atype2 == aim_atype2) mapka = new UniversalHashmap2<buff_gtype1, buff_atype1, buff_atype2>();

#define init_hashmap_3(mapka, gtype1, aim_gtype1, buff_gtype1, gtype2, aim_gtype2, buff_gtype2, atype1, aim_atype1, buff_atype1) \
	if (gtype1 == aim_gtype1 && gtype2 == aim_gtype2 && atype1 == aim_atype1) mapka = new UniversalHashmap3<buff_gtype1, buff_gtype2, buff_atype1>();


namespace Engine {

GroupByOperation::GroupByOperation(NodeEnvironmentInterface * nei, const OperationTree::GroupByOperation & node, MemoryManager * mem_manager):
		Operation(nei), node_(node), mem_manager_(mem_manager), first_time_(true) {
}

InitRes GroupByOperation::init(bool &group_flag) {
	source_ = OperationBuilder::build(nei_, node_.source(), mem_manager_);
	InitRes r = source_ -> init(group_flag);
	source_types_ = r.first;


	count_buffer_ = static_cast<int32*>(mem_manager_ -> allocate_normal());
	int max_rows = mem_manager_ -> max_rows();
	for(int i = 0; i < max_rows; ++i) { count_buffer_[i] = 1; }
	map_ = NULL;
	init_hashmap();


	if (group_flag){
		GroupSender *sender = new GroupSender(nei_, source_, source_types_, hash_signature_, node_);
		group_flag = false;
		return make_pair(res_types_, sender);
	} else {
		GroupReceiver *receiver = new GroupReceiver(nei_, source_types_);
		this -> source_ = receiver;
		group_flag = true;
		return make_pair(res_types_, this);
	}

}

void GroupByOperation::init_hashmap() {
	if (node_.group_by_column_size() == 2 && node_.aggregations_size() == 1) {
		OperationTree::ScanOperation_Type gtype1 = source_types_[node_.group_by_column(0)];
		OperationTree::ScanOperation_Type gtype2 = source_types_[node_.group_by_column(0)];
		OperationTree::ScanOperation_Type atype1 = SINT;
		if (node_.aggregations(0).type() == OperationTree::Aggregation_Type_SUM)
			atype1 = source_types_[node_.aggregations(0).aggregated_column()];

		init_hashmap_3(map_, gtype1, SINT, int32, gtype2, SINT, int32, atype1, SINT, int32);
		init_hashmap_3(map_, gtype1, SINT, int32, gtype2, SINT, int32, atype1, SDOUBLE, double);

		init_hashmap_3(map_, gtype1, SINT, int32, gtype2, SBOOL, bool, atype1, SINT, int32);
		init_hashmap_3(map_, gtype1, SINT, int32, gtype2, SBOOL, bool, atype1, SDOUBLE, double);

		init_hashmap_3(map_, gtype1, SBOOL, bool, gtype2, SINT, int32, atype1, SINT, int32);
		init_hashmap_3(map_, gtype1, SBOOL, bool, gtype2, SINT, int32, atype1, SDOUBLE, double);

		init_hashmap_3(map_, gtype1, SBOOL, bool, gtype2, SBOOL, bool, atype1, SINT, int32);
		init_hashmap_3(map_, gtype1, SBOOL, bool, gtype2, SBOOL, bool, atype1, SDOUBLE, double);

	}

	if (node_.group_by_column_size() == 1 && node_.aggregations_size() == 1) {
		OperationTree::ScanOperation_Type gtype1 = source_types_[node_.group_by_column(0)];
		OperationTree::ScanOperation_Type atype1 = SINT;
		if (node_.aggregations(0).type() == OperationTree::Aggregation_Type_SUM)
			atype1 = source_types_[node_.aggregations(0).aggregated_column()];
		init_hashmap_1(map_, gtype1, SINT, int32, atype1, SINT, int32);
		init_hashmap_1(map_, gtype1, SINT, int32, atype1, SDOUBLE, double);
		init_hashmap_1(map_, gtype1, SBOOL, bool, atype1, SINT, int32);
		init_hashmap_1(map_, gtype1, SBOOL, bool, atype1, SDOUBLE, double);
	}

	if (node_.group_by_column_size() == 1 && node_.aggregations_size() == 2) {
				OperationTree::ScanOperation_Type gtype1 = source_types_[node_.group_by_column(0)];
		OperationTree::ScanOperation_Type atype1 = SINT;
		if (node_.aggregations(0).type() == OperationTree::Aggregation_Type_SUM)
			atype1 = source_types_[node_.aggregations(0).aggregated_column()];

			OperationTree::ScanOperation_Type atype2 = SINT;
		if (node_.aggregations(1).type() == OperationTree::Aggregation_Type_SUM)
			atype2 = source_types_[node_.aggregations(1).aggregated_column()];

		init_hashmap_2(map_, gtype1, SINT, int32, atype1, SINT, int32, atype2, SINT, int32);
		init_hashmap_2(map_, gtype1, SINT, int32, atype1, SINT, int32, atype2, SDOUBLE, double);

		init_hashmap_2(map_, gtype1, SINT, int32, atype1, SDOUBLE, double, atype2, SINT, int32);
		init_hashmap_2(map_, gtype1, SINT, int32, atype1, SDOUBLE, double, atype2, SDOUBLE, double);

		init_hashmap_2(map_, gtype1, SBOOL, bool, atype1, SINT, int32, atype2, SINT, int32);
		init_hashmap_2(map_, gtype1, SBOOL, bool, atype1, SINT, int32, atype2, SDOUBLE, double);

		init_hashmap_2(map_, gtype1, SBOOL, bool, atype1, SDOUBLE, double, atype2, SINT, int32);
		init_hashmap_2(map_, gtype1, SBOOL, bool, atype1, SDOUBLE, double, atype2, SDOUBLE, double);
	}

	vector<OperationTree::ScanOperation_Type> hash_signature;
	vector<OperationTree::ScanOperation_Type> value_signature;

	for(int i = 0; i < node_.group_by_column_size(); ++i) {
		res_types_.push_back(source_types_[node_.group_by_column(i)]);
		hash_signature.push_back(source_types_[node_.group_by_column(i)]);
	}

	hash_signature_ = hash_signature;

	for(int i = 0; i < node_.aggregations_size(); ++i) {
		if (node_.aggregations(i).type() == OperationTree::Aggregation_Type_COUNT) res_types_.push_back(SINT);
		else res_types_.push_back(source_types_[node_.aggregations(i).aggregated_column()]);

		if (node_.aggregations(i).type() == OperationTree::Aggregation_Type_COUNT) value_signature.push_back(SINT);
		else value_signature.push_back(source_types_[node_.aggregations(i).aggregated_column()]);
	}

	if (map_ == NULL) {
		cerr << "YEEEEEEEEEEEEEEEEEEEEES !!!" << hash_signature.size() << " " << value_signature.size() << endl;
		for(int i = 0; i < value_signature.size(); ++i) {
			cerr << hash_signature[i] << " ";
		} cerr << endl;
		map_ = new RealUniversalHashmap(hash_signature, value_signature);
	}
}

void GroupByOperation::first_pull() {
	if (debug) cerr << "GroupByOperation::first_pull" << endl;
while (true) {
		int x = mem_manager_ -> max_rows();
		vector<void*> data = source_ -> pull(x);
		if (x == 0) break;


		if (node_.aggregations_size() == 1 && node_.group_by_column_size() <= 2) {
			void * buff = count_buffer_;

			if (node_.aggregations(0).type() == OperationTree::Aggregation_Type_SUM)
				buff = data[node_.aggregations(0).aggregated_column()];
			if (node_.group_by_column_size() == 2)
				map_ -> insert(data[node_.group_by_column(0)], data[node_.group_by_column(1)], buff, x);
			else map_ -> insert(data[node_.group_by_column(0)], buff, x);
		} else if (node_.aggregations_size() == 2 && node_.group_by_column_size() == 1) {
			void * buff1 = count_buffer_;
			void * buff2 = count_buffer_;

			if (node_.aggregations(0).type() == OperationTree::Aggregation_Type_SUM)
				buff1 = data[node_.aggregations(0).aggregated_column()];
			if (node_.aggregations(1).type() == OperationTree::Aggregation_Type_SUM)
				buff2 = data[node_.aggregations(1).aggregated_column()];

			map_ -> insert(data[node_.group_by_column(0)], buff1, buff2, x);
		} else {
			vector<void*> hash_buff;
			vector<void*> value_buff;
			for(int i = 0; i < node_.group_by_column_size(); ++i) {
				hash_buff.push_back(data[node_.group_by_column(i)]);
			}

			for(int i = 0; i < node_.aggregations_size(); ++i) {
				if (node_.aggregations(i).type() == OperationTree::Aggregation_Type_SUM)
					value_buff.push_back(data[node_.aggregations(i).aggregated_column()]);
				else value_buff.push_back(count_buffer_);
			}

			map_ -> insert(hash_buff, value_buff, x);

		}
}
	res_ = map_ -> get_result(res_size_);

}

vector<void*> GroupByOperation::pull(int &rows) {
	if (debug) cerr << "GroupByOperation::pull" << endl;
	if (first_time_) {
		first_time_ = false;
		first_pull();
	}

	vector<void*> resx = res_;
	rows = min(rows, res_size_);
	res_size_ -= rows;
	for(int i = 0; i < res_types_.size(); ++i) {
		switch(res_types_[i]) {
		case SINT:
			{
			int32 *buff = static_cast<int32*>(res_[i]);
			buff += rows;
			res_[i] = buff;
			}
			break;
		case SBOOL:
			{
			bool *buff = static_cast<bool*>(res_[i]);
			buff += rows;
			res_[i] = buff;
			}
			break;
		case SDOUBLE:
			{
			double *buff = static_cast<double*>(res_[i]);
			buff += rows;
			res_[i] = buff;
			}
			break;
		}
	}
	return resx;
}


}
