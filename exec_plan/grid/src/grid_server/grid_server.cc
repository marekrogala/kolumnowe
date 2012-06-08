#include "grid_server/grid_server.h"

#include "utils/logger.h"
#include "netio/basic.h"
#include "grid_server/proto/grid_server.pb.h"

using boost::asio::ip::tcp;

GridServer::GridServer(short listening_port)
    : acceptor_(io_service_, tcp::endpoint(tcp::v4(), listening_port), true),
      thread_(boost::bind((size_t (boost::asio::io_service::*)())(&boost::asio::io_service::run), &io_service_)),
      pc_queue_(100) {
  start_accept();
}


void GridServer::start_accept() {
  waiting_endpoint_.reset(new boost::asio::ip::tcp::endpoint());
  waiting_socket_.reset(new boost::asio::ip::tcp::socket(io_service_));
  acceptor_.async_accept(*waiting_socket_, *waiting_endpoint_,
      boost::bind(&GridServer::accept_connection, this,
              boost::asio::placeholders::error));
  LOG2("Waiting for connections on: %s:%d",
       acceptor_.local_endpoint().address().to_string().c_str(),
       acceptor_.local_endpoint().port());

}

void GridServer::accept_connection(const boost::system::error_code& e) {
  boost::mutex::scoped_lock lock(mutex_);
  LOG2("%s:%d CONNECTED.", waiting_endpoint_->address().to_string().c_str(),
                           waiting_endpoint_->port());
  uint32_t length;
  boost::scoped_array<char> data(ReadPacket(*waiting_socket_, &length));
  uint16_t server_port;
  CHECK(length == sizeof(server_port), "");
  memcpy(&server_port, data.get(), sizeof(server_port));
  LOG3("%s:%d CLIENT PORT: %d.", waiting_endpoint_->address().to_string().c_str(),
                                 waiting_endpoint_->port(), server_port);

  connections_.push_back(new InputConnection(&io_service_,
                                             waiting_socket_.release(),
                                             waiting_endpoint_.release(),
                                             &pc_queue_));
  ports_.push_back(server_port);
  if (!boost::this_thread::interruption_requested()) start_accept();
}

void GridServer::WaitForActiveNodes(int target_nodes_count) {
  while (active_nodes_count() < target_nodes_count) {
    LOG2("Waiting for connections. Got %d of %d", active_nodes_count(),
         target_nodes_count);
    sleep(1);
  }
  thread_.interrupt();
}

void GridServer::BroadcastTopologyToActive(int target_nodes_count) {
  boost::mutex::scoped_lock lock(mutex_);
  LOG0("Building topology...");
  TopologyAssigment topology;
  for (int i = 0; i < connections_.size(); ++i) {
    if (connections_[i]->is_active()) {
      Peer* peer = topology.add_peers();
      peer->set_host(strdup(connections_[i]->endpoint().address().to_string().c_str()));
      peer->set_port(ports_[i]);
      LOG3("PEER %5d: %s:%d", topology.peers_size() - 1, peer->host().c_str(), peer->port());
    }
    if (topology.peers().size() == target_nodes_count) break;
  }
  CHECK(topology.peers().size() == target_nodes_count, "Some node died in meantime");

  LOG1("Broadcasting topology:\n%s", topology.DebugString().c_str());
  int j = 0;
  for (int i = 0; i < connections_.size(); ++i) {
    if (connections_[i]->is_active()) {
      TopologyAssigment topology_per_peer = topology;
      topology_per_peer.set_your_number(j++);
      CHECK(connections_[i]->SendPacket(topology_per_peer.SerializeAsString()), "");
    }
  }
}

int GridServer::active_nodes_count() const {
  int count = 0;
  boost::mutex::scoped_lock lock(mutex_);
  for (int i = 0; i < connections_.size(); ++i) {
    count += connections_[i]->is_active() ? 1 : 0;
  }
  return count;
}

GridServer::~GridServer() {
  io_service_.stop();
  thread_.join();
}
