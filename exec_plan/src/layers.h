#ifndef LAYERS_H_
#define LAYERS_H_

#include "node_environment.h"
#include <vector>
#include <map>

using namespace std;


class Layers {
	private:
		static int get_layer(int n) {
			return n % 2;
		}

		static vector<int> t_[2];
		static map<int, int> map_;
		static map<int, int> map_inv_[2];
		static int node_number_;
		static int my_layer_;

	public:
		static void init(int layers, NodeEnvironmentInterface * nei) {
			map_.clear();
			map_inv_[0].clear();
			map_inv_[1].clear();
			t_[0].clear();
			t_[1].clear();

			node_number_ = nei -> my_node_number();
			if (layers == 1) {
				for(int i = 1; i < nei -> nodes_count(); ++i){
					t_[0].push_back(i);
					map_[i] = t_[0].size() - 1;
					map_inv_[0][t_[0].size() - 1] = i;
				}
				my_layer_ = 0;
			} else {
				for(int i = 1; i < nei -> nodes_count(); ++i) {
					t_[get_layer(i)].push_back(i);
					map_[i] = t_[get_layer(i)].size() - 1;
					map_inv_[get_layer(i)][t_[get_layer(i)].size() - 1] = i;
				}
				my_layer_ = get_layer(node_number_);
			}

		}

		static int get_real_node_number(int layer, int number) {
			return map_inv_[layer][number];
		}

		static int get_my_node_number() {
			return map_[node_number_];
		}

		static int count_nodes_in_my_layer() {
			return t_[my_layer_].size();
		}

		static int count_nodes_in_other_layer() {
			return t_[1 - my_layer_].size();
		}
		static int get_my_layer() {
			return my_layer_;
		}

};

/*
void inline SendPacket(NodeEnvironmentInterface* nei, uint32 target_node,
              const char* data, int  data_len){
  target_node++;
  if(GetMyLayer(nei) == 0) target_node += CountNodesInLayer(nei, 0);
  nei->SendPacket(target_node, data, data_len);
}*/

#endif
