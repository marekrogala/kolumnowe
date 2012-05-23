#ifndef LAYERS_H_
#define LAYERS_H_

int CountNodesInLayer(NodeEnvironmentInterface* nei, int parity){
  int nodes = nei->nodes_count();
  return nodes/2 + (!parity && (nodes%2));
}

int CountNodesInMyLayer(NodeEnvironmentInterface* nei, int myNumber){
  return CountNodesInLayer(nei, myNumber%2);
}

int CountNodesInOtherLayer(NodeEnvironmentInterface* nei, int myNumber){
  return CountNodesInLayer(nei, (myNumber+1)%2);
}

#endif
