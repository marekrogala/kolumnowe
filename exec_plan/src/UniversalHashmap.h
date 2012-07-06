/*
 * UniversalHashmap.h
 *
 *  Created on: 9 Apr 2012
 *      Author: maciek
 */

#ifndef UNIVERSALHASHMAP_H_
#define UNIVERSALHASHMAP_H_

#include <iostream>
#include <vector>
#include <tr1/unordered_map>
#include <cassert>
#include <map>
#include <cstdlib>
#include "debug.h"

using namespace std;
namespace std
{
namespace tr1{
  template<typename S, typename T> struct hash<pair<S, T> >
  {
    inline size_t operator()(const pair<S, T> & v) const
    {
      return hash<S>()(v.first) ^ hash<T>()(v.second);
    }
  };
}
}

namespace Engine {

class UniversalMap {
public:
	virtual void insert(void*, void*, int rows) { assert(false); }
	virtual void insert(void*, void*, void*, int rows) { assert(false); }
	virtual void insert(vector<void*>, vector<void*>, int rows) { assert(false); }
	virtual vector<void*> get_result(int&size) = 0;
};

template<typename K, typename V>
class UniversalHashmap : public  UniversalMap{
public:
	UniversalHashmap():map2_(new tr1::unordered_map<K, V>()) { 
	}

	void insert(void* key, void* value, int rows) {
		K* key_buff = static_cast<K*>(key);
		V* value_buff = static_cast<V*>(value);
		for(int i = 0; i < rows; ++i) {
			(*map2_)[key_buff[i]] += value_buff[i];
		}
	}

	vector<void*> get_result(int&size) {
		size = map2_->size();
		vector<void*> res;
		K* res1 = static_cast<K*>(malloc(sizeof(K) * size));
		V* res2 = static_cast<V*>(malloc(sizeof(V) * size));
		int z = 0;
		for(typename tr1::unordered_map<K, V>::iterator i = map2_->begin(); i != map2_->end(); ++i) {
			res1[z] = i -> first;
			res2[z] = i -> second;
			++z;
		}
		res.push_back(res1);
		res.push_back(res2);
		delete map2_;
		return res;
	}

private:
	tr1::unordered_map<K, V> *map2_;
//	tr1::unordered_map<K, V> &map_;
};

template<typename V>
class BoolHashmap : public UniversalMap{
public:
	BoolHashmap() { map_[0] = map_[1] = 0; tak_[0] = tak_[1] = false;}

	void insert(void* key, void* value, int rows) {
		bool* key_buff = static_cast<bool*>(key);
		V* value_buff = static_cast<V*>(value);
		for(int i = 0; i < rows; ++i) {
			map_[key_buff[i]] += value_buff[i];
			tak_[key_buff[i]] = 1;
		}
	}

	vector<void*> get_result(int&size) {
		size = (int)(tak_[0]) + (int)(tak_[1]);
		vector<void*> res;
		bool* res1 = static_cast<bool*>(malloc(sizeof(bool) * size));
		V* res2 = static_cast<V*>(malloc(sizeof(V) * size));
		int z = 0;
		for(int i = 0; i < 2; ++i) {
			if (tak_[i]) {
			res1[z] = (bool)i;
			res2[z] = map_[i];
			++z;
			}
		}
		res.push_back(res1);
		res.push_back(res2);
		return res;
	}

private:
	V map_[2];
	bool tak_[2];
};

template<typename V, typename V2>
class BoolHashmap2 : public UniversalMap{
public:
	BoolHashmap2() { map_[0].first = map_[1].first = map_[0].second = map_[1].second = 0; tak_[0] = tak_[1] = false; }

	void insert(void* key, void* value, void* value2, int rows) {
		bool* key_buff = static_cast<bool*>(key);
		V* value_buff = static_cast<V*>(value);
		V2* value_buff2 = static_cast<V2*>(value2);
		for(int i = 0; i < rows; ++i) {
			map_[key_buff[i]].first += value_buff[i];
			map_[key_buff[i]].second += value_buff2[i];
			tak_[key_buff[i]] = 1;
		}
	}

	vector<void*> get_result(int&size) {
		size = (int)(tak_[0]) + (int)(tak_[1]);
		vector<void*> res;
		bool* res1 = static_cast<bool*>(malloc(sizeof(bool) * size));
		V* res2 = static_cast<V*>(malloc(sizeof(V) * size));
		V2* res3 = static_cast<V2*>(malloc(sizeof(V2) * size));
		int z = 0;
		for(int i = 0; i < 2; ++i) {
			if (tak_[i]) {
			res1[z] = (bool)i;
			res2[z] = map_[i].first;
			res3[z] = map_[i].second;
			++z;
			}
		}
		res.push_back(res1);
		res.push_back(res2);
		res.push_back(res3);
		return res;
	}

private:
	pair<V, V2> map_[2];
	bool tak_[2];
};


template<typename K, typename V, typename V2>
class UniversalHashmap2 : public  UniversalMap{
public:
	UniversalHashmap2():map2_(new tr1::unordered_map<K, pair<V, V2> >()) { }
	void insert(void* key, void* value1, void* value2, int rows) {
		K* key_buff = static_cast<K*>(key);
		V* value_buff = static_cast<V*>(value1);
		V2* value_buff2 = static_cast<V2*>(value2);
		for(int i = 0; i < rows; ++i) {
			typename tr1::unordered_map<K, pair<V, V2> > :: iterator x = map2_->find(key_buff[i]);
			if (x == map2_->end()) {
				(*map2_)[key_buff[i]] = make_pair(value_buff[i], value_buff2[i]);
			} else {
				x -> second.first += value_buff[i];
				x -> second.second += value_buff2[i];
			}
		}
	}

	vector<void*> get_result(int&size) {
		size = map2_->size();
		vector<void*> res;
		K* res1 = static_cast<K*>(malloc(sizeof(K) * size));
		V* res2 = static_cast<V*>(malloc(sizeof(V) * size));
		V2* res3 = static_cast<V2*>(malloc(sizeof(V2) * size));
		int z = 0;
		for(typename tr1::unordered_map<K, pair<V, V2> >::iterator i = map2_->begin(); i != map2_->end(); ++i) {
			res1[z] = i -> first;
			res2[z] = i -> second.first;
			res3[z] = i -> second.second;
			++z;
		}
		res.push_back(res1);
		res.push_back(res2);
		res.push_back(res3);
		delete map2_;
		return res;
	}

private:
	tr1::unordered_map<K, pair<V, V2> > *map2_;
//	tr1::unordered_map<K, pair<V, V2> > &map_;
};


template<typename K, typename K2, typename V>
class UniversalHashmap3 : public  UniversalMap{
public:

	UniversalHashmap3() : map2_(new tr1::unordered_map<pair<K, K2>, V>()) {}
	void insert(void* key, void* key2, void* value, int rows) {
		K* key_buff = static_cast<K*>(key);
		K2* key_buff2 = static_cast<K2*>(key2);
		V* value_buff = static_cast<V*>(value);
		for(int i = 0; i < rows; ++i) {
			(*map2_)[make_pair(key_buff[i], key_buff2[i])] += value_buff[i];
		}
	}

	vector<void*> get_result(int&size) {
		size = map2_->size();
		vector<void*> res;
		K* res1 = static_cast<K*>(malloc(sizeof(K) * size));
		K2* res2 = static_cast<K2*>(malloc(sizeof(K2) * size));
		V* res3 = static_cast<V*>(malloc(sizeof(V) * size));
		int z = 0;

		for(typename tr1::unordered_map<pair<K, K2>, V>::iterator i = map2_->begin(); i != map2_->end(); ++i) {
			res1[z] = i -> first.first;
			res2[z] = i -> first.second;
			res3[z] = i -> second;
			++z;
		}
		res.push_back(res1);
		res.push_back(res2);
		res.push_back(res3);
		delete map2_;
		return res;
	}

private:
	tr1::unordered_map<pair<K, K2>, V> *map2_;
//	tr1::unordered_map<pair<K, K2>, V> &map_;
};

}

#endif /* UNIVERSALHASHMAP_H_ */
