/*
 * sent_main.cc
 *
 *  Created on: 07-06-2012
 *      Author: ptab
 */


#include <boost/asio.hpp>
#include <unistd.h>

#include "netio/basic.h"
#include "utils/logger.h"
#include "utils/ip_address.h"

using boost::asio::ip::tcp;

int main(int argc, char** argv) {
  CHECK(argc == 2, "Requires 1 argument [host:port]");
  IpAddress server_address = IpAddress::Parse(argv[1]);
  boost::asio::io_service io_service;
  boost::scoped_ptr<tcp::socket> socket(
      ConnectClient(io_service, server_address.getHostAddress(),
                    server_address.getService()));
  std::string data;
  int size;
  char buf[1024];
  while ((size = read(STDIN_FILENO, &buf, 1024)) > 0) {
    data.append(buf, size);
  }
  CHECK(SendPacket(*socket, data), "Cannot send packet");
  return 1;
}
