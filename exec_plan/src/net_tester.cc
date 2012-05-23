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

}

void Reader(NodeEnvironmentInterface* nei) {
    for (int i = 0 ; i < nei->nodes_count(); ++i) {
      if(i%2){
        std::size_t data_len;
        boost::scoped_array<char> packet(nei->ReadPacketBlocking(&data_len));
        cout << nei->my_node_number() << " received " << packet.get() << endl;
      }
    }

}


void print_rows(vector<void*> r){
  for(int i=0;i<r.size();i++){
    cout << (char*)r[i]<< endl;
  }
}

int main(int argc, char** argv) {
  NodeEnvironmentInterface * nei(CreateNodeEnvironment(argc, argv));

  OperationTree::GroupByOperation *op = OperationTree::GroupByOperation::default_instance().New();
    
  if (nei->my_node_number() % 2) {
    cout<<"sender"<<nei->my_node_number()<<endl;
    GroupSender sender(nei, NULL, *op);
    int nrows;
    sender.pull(nrows);
  } else {
    cout<<"receiver"<<nei->my_node_number()<<endl;
    GroupReceiver receiver(nei, *op);
    
    int nrows = 0;
    vector<void*> received = receiver.pull(nrows);
    while(nrows){
      print_rows(received);
      received = receiver.pull(nrows);
    }
  }
  return 0;
}

