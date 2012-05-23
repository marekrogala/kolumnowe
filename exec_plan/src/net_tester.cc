#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include<iostream>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/thread.hpp>

#include "node_environment.h"
#include "logger.h"

using namespace std;

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
  boost::scoped_ptr<NodeEnvironmentInterface> nei(
      CreateNodeEnvironment(argc, argv));

  /*// Lets open data sources.
  boost::scoped_ptr<DataSourceInterface> data_source_0(nei->OpenDataSourceFile(0));
  boost::scoped_ptr<DataSourceInterface> data_source_1(nei->OpenDataSourceFile(1));
  // Lets open sink on node 0.
  if (nei->my_node_number() == 0) {
    boost::scoped_ptr<DataSinkInterface> data_sink(nei->OpenDataSink());
  }*/
  
  if (nei->my_node_number() % 2) {
    Writer(nei.get());
  } else {
    Reader(nei.get());
  }
  return 0;
}

