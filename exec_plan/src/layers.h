#ifndef LAYERS_H_
#define LAYERS_H_

#include "node_environment/node_environment.h"

/* nodes 0..floor(n/2)-1   --> Layer number 0
   nodes floor(n/2)..n-1   --> Layer number 1
   
   Initially Layer 0 are receivers, Layer 1 senders
   */


int inline CountNodesInLayer(NodeEnvironmentInterface* nei, int layerNum){
  int nodes = nei->nodes_count();
  if(layerNum == 0){
    return nodes/2;
  } else {
    return nodes - nodes/2;
  }
}

int inline GetMyLayer(NodeEnvironmentInterface* nei){
  return nei->my_node_number() >= CountNodesInLayer(nei, 0);
}

int inline CountNodesInMyLayer(NodeEnvironmentInterface* nei){
  return CountNodesInLayer(nei, GetMyLayer(nei));
}

int inline CountNodesInOtherLayer(NodeEnvironmentInterface* nei){
  return CountNodesInLayer(nei, !GetMyLayer(nei));
}

void inline SendPacket(NodeEnvironmentInterface* nei, uint32 target_node,
              const char* data, int  data_len){
  if(GetMyLayer(nei) == 0) target_node += CountNodesInLayer(0);
  nei->SendPacket(target_node, data, data_len);
}

#endif
