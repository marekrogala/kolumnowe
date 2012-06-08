#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/thread.hpp>

#include "grid_server/grid_server.h"
#include "netio/network_input.h"
#include "utils/logger.h"

int number_of_active_connections(NetworkInput* input) {
  int count = 0;
  for (int i = 0; i < input->connections_count(); ++i) {
    count += input->is_active(i) ? 1 : 0;
  }
  return count;
}

int main(int argc, char** argv) {
  CHECK(argc == 3, "Two argument expected: [listening_port] [number_of_nodes]");
  int port = atoi(argv[1]);
  int number_of_nodes = atoi(argv[2]);
  GridServer server(port);
  server.WaitForActiveNodes(number_of_nodes);
  server.BroadcastTopologyToActive(number_of_nodes);
  while (true) { sleep(1); };
}

