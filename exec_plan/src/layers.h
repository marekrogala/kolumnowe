#ifndef LAYERS_H_
#define LAYERS_H_

#include "node_environment/node_environment.h"

int inline CountNodesInLayer(NodeEnvironmentInterface* nei, int parity){
  int nodes = nei->nodes_count();
  return nodes/2 + (!parity && (nodes%2));
}

int inline CountNodesInMyLayer(NodeEnvironmentInterface* nei){
  return CountNodesInLayer(nei, nei->my_node_number()%2);
}

int inline CountNodesInOtherLayer(NodeEnvironmentInterface* nei){
  return CountNodesInLayer(nei, (nei->my_node_number()+1)%2);
}

void inline SendPacket(NodeEnvironmentInterface* nei, uint32 target_node,
              const char* data, int  data_len){
  target_node *= 2;
  if(nei->my_node_number()%2==0) target_node++;
  nei->SendPacket(target_node, data, data_len);
}

#endif
