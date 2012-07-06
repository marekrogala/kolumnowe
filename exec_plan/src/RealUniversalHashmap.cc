/*
 * RealUniversalHashmap.cpp
 *
 *  Created on: 12 Apr 2012
 *      Author: maciek
 */

#include "RealUniversalHashmap.h"

namespace Engine {

RealUniversalHashmap::RealUniversalHashmap(vector<OperationTree::ScanOperation_Type> hash_signature,
		vector<OperationTree::ScanOperation_Type> value_signature) :
			hash_signature_(hash_signature), value_signature_(value_signature), map2_(new tr1::unordered_map<HashStruct, HashStruct, hash_name>()) {
//	cerr << "Creating RealUniversal!!!!!!!!!!" << endl;
}

bool operator==(const RealUniversalHashmap::HashStruct&l, const RealUniversalHashmap::HashStruct&r) {
	for(int i = 0; i < l.t.size(); ++i) {
		if (l.t[i].i != r.t[i].i) return false;
		if (l.t[i].b != r.t[i].b) return false;
		if (l.t[i].d != r.t[i].d) return false;
	}
	return true;
}


void RealUniversalHashmap::insert(vector<void*> hash, vector<void*> value, int rows) {
	HashStruct h;
	h.t.resize(hash.size());

	HashStruct v;
	v.t.resize(value.size());


	for(int i = 0; i < rows; ++i) {
		for(int j = 0; j < hash_signature_.size(); ++j) {
			if (hash_signature_[j] == SINT) {
				h.t[j].i = (static_cast<int32*>(hash[j]))[i];
			} else {
				h.t[j].b = (static_cast<bool*>(hash[j]))[i];
			}
		}

		tr1::unordered_map<HashStruct, HashStruct>::iterator x = map2_->find(h);
		if (x == map2_->end()) {
			for(int j = 0; j < value_signature_.size(); ++j) {
				if (value_signature_[j] == SINT) {
					v.t[j].i = (static_cast<int32*>(value[j]))[i];
				} else {
					v.t[j].d = (static_cast<double*>(value[j]))[i];
				}
			}
			(*map2_)[h] = v;
		} else {
			for(int j = 0; j  < value_signature_.size(); ++j) {
				if (value_signature_[j] == SINT) {
					x -> second.t[j].i += (static_cast<int32*>(value[j]))[i];
				} else {
					x -> second.t[j].d += (static_cast<double*>(value[j]))[i];
				}
			}
		}
	}
}

vector<void*> RealUniversalHashmap::get_result(int & size) {
	size = map2_->size();
	vector<void*> res;
	for(int i = 0; i < hash_signature_.size(); ++i) {
		if (hash_signature_[i] == SINT) {
			res.push_back(malloc(sizeof(int32) * size));
		} else {
			res.push_back(malloc(sizeof(bool) * size));
		}
	}

	for(int i = 0; i < value_signature_.size(); ++i) {
		if (value_signature_[i] == SINT) {
			res.push_back(malloc(sizeof(int32) * size));
		} else {
			res.push_back(malloc(sizeof(double) * size));
		}
	}

	int z = 0;

	for(tr1::unordered_map<HashStruct, HashStruct>::iterator x = map2_->begin(); x != map2_->end(); ++x) {
		// XXX maybe change the order of the loops to optimize?

	for(int i = 0; i < hash_signature_.size(); ++i) {
		if (hash_signature_[i] == SINT) {
			int32* ptr = static_cast<int32*>(res[i]);
			ptr[z] = x -> first.t[i].i;
		} else {
			bool* ptr = static_cast<bool*>(res[i]);
			ptr[z] = x -> first.t[i].b;
		}
	}

	for(int i = 0; i < value_signature_.size(); ++i) {
		if (value_signature_[i] == SINT) {
			int32* ptr = static_cast<int32*>(res[i + hash_signature_.size()]);
			ptr[z] = x -> second.t[i].i;
		} else {
			double* ptr = static_cast<double*>(res[i + hash_signature_.size()]);
			ptr[z] = x -> second.t[i].d;
		}
	}
	++z;
	}

	delete map2_;
	return res;
}


}



