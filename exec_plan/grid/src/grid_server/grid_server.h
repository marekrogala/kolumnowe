/*
 * grid_server.h
 *
 *  Created on: 06-05-2012
 *      Author: ptab
 */

#ifndef GRID_SERVER_H_
#define GRID_SERVER_H_

#include <stdlib.h>
#include <boost/system/error_code.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "netio/network_input.h"

class GridServer {
 public:
  GridServer(short listening_port);
  virtual ~GridServer();

  void WaitForActiveNodes(int target_nodes_count);

  void BroadcastTopologyToActive(int target_nodes_count);

  int active_nodes_count() const;
//  int connections_count() const;
//  const boost::asio::ip::tcp::endpoint& endpoint(int connection_id) const;
//  bool is_active(int connection_id) const;

 private:
  void start_accept();
  void accept_connection(const boost::system::error_code& e);

  mutable boost::mutex mutex_;
  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;
  std::auto_ptr<boost::asio::ip::tcp::socket> waiting_socket_;
  std::auto_ptr<boost::asio::ip::tcp::endpoint> waiting_endpoint_;
  boost::thread thread_;
  std::vector<InputConnection*> connections_;
  std::vector<uint16_t> ports_;
  util::PCQueue<Packet*> pc_queue_;
};

#endif /* GRID_SERVER_H_ */
