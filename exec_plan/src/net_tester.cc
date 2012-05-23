#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<iostream>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/thread.hpp>

#include "GroupSender.h"
#include "GroupReceiver.h"
#include "operations.pb.h"
#include "node_environment.h"
#include "logger.h"

using namespace std;
using namespace Engine;

void Writer(NodeEnvironmentInterface* nei) {
  char buff[20];
  sprintf(buff, "HEY from %d", nei->my_node_number());
  for (int i = 0 ; i < nei->nodes_count(); ++i) {
    if(i%2==0){
      nei->SendPacket(i, buff, strlen(buff));
    }
  }
}

void Reader(NodeEnvironmentInterface* nei) {
    for (int i = 0 ; i < nei->nodes_count(); ++i) {
      if(i%2){
        std::size_t data_len;
        boost::scoped_array<char> packet(nei->ReadPacketBlocking(&data_len));
        cout << nei->my_node_number() << " received " << packet.get() << endl;
      }
    }
//    if (packet != NULL) {
//      LOG3("worker %d/%d: GOT: %s", nei->my_node_number(), nei->nodes_count(), packet.get());
//    }
//  }
//  std::size_t data_len;
//  boost::scoped_array<char> packet(nei->ReadPacketNotBlocking(&data_len));
//  CHECK(packet == NULL, "");
}


int main(int argc, char** argv) {
  NodeEnvironmentInterface * nei(CreateNodeEnvironment(argc, argv));

  OperationTree::GroupByOperation *op = OperationTree::GroupByOperation::default_instance().New();
    
  if (nei->my_node_number() % 2) {
    GroupSender sender(nei, NULL, *op);
  } else {
    GroupReceiver receiver(nei, *op);
  }
  return 0;
}

