#include "node_environment.h"

#include <boost/scoped_ptr.hpp>
#include <string.h>
#include <vector>

#include <stdlib.h>
#include <stdio.h>

#include "utils/ip_address.h"
#include "utils/logger.h"
#include "netio/basic.h"
#include "netio/network_input.h"
#include "netio/network_output.h"
#include "node_environment/data_server.h"
#include "grid_server/proto/grid_server.pb.h"

namespace {

using boost::asio::ip::tcp;
using boost::asio::buffer;
using boost::asio::write;
using boost::asio::read;

class DataSource : public DataSourceInterface {
 public:
  DataSource(Server* server) : data_server_(server) {}

  virtual int GetDoubles(int column_index, int number, double* destination) {
    return data_server_->GetDoubles(column_index, number, destination);
  }

  virtual int GetInts(int column_index, int number, int32* destination) {
    return data_server_->GetInts(column_index, number, destination);
  }

  virtual int GetByteBools(int column_index, int number, bool* destination) {
    return data_server_->GetByteBools(column_index, number, destination);
  }

  virtual int GetBitBools(int column_index, int number, char* destination) {
    return data_server_->GetBitBools(column_index, number, destination);
  }

 private:
  boost::scoped_ptr<Server> data_server_;
};

// ----------------------------------------------------------------------------

class DataSink : public DataSinkInterface {
 public:
  DataSink(Server* server) : data_server_(server) {}

  virtual void ConsumeDoubles(int column_index, int number,
                              const double* destination) {
    data_server_->ConsumeDoubles(column_index, number, destination);
  }

  virtual void ConsumeInts(int column_index, int number,
                           const int32* destination) {
    data_server_->ConsumeInts(column_index, number, destination);
  }

  virtual void ConsumeByteBools(int column_index, int number,
                                const bool* destination) {
    data_server_->ConsumeByteBools(column_index, number, destination);
  }

  virtual void ConsumeBitBools(int column_index, int number,
                               const char* destination) {
    data_server_->ConsumeBitBools(column_index, number, destination);
  }

 private:
  boost::scoped_ptr<Server> data_server_;
};

// ----------------------------------------------------------------------------

class NodeEnvironment : public NodeEnvironmentInterface {
 public:
  NodeEnvironment(uint32 node_number,
                  NetworkInput* input,
                  std::vector<NetworkOutput*> outputs)
  : node_number_(node_number),
    node_count_(outputs.size()),
    input_(input),
    outputs_(outputs) {}

  virtual uint32 my_node_number() const {
    CHECK(node_number_>=0, "");
    return node_number_;
  }

  virtual uint32 nodes_count() const {
    CHECK(node_count_>=0, "");
    return node_count_;
  }

  virtual void SendPacket(uint32 target_node,
                          const char* data,
                          int  data_len) {
    CHECK(target_node < nodes_count(), "");
    CHECK(target_node >= 0, "");
    CHECK(outputs_[target_node]->SendPacket(data, data_len), "");
  }

  virtual char* ReadPacketBlocking(std::size_t* data_len) {
    return input_->ReadPacketBlocking(data_len);
  }

  virtual char* ReadPacketNotBlocking(std::size_t* data_len) {
    return input_->ReadPacketNotBlocking(data_len);
  }

  // -------- Reading input files and writing results --------------------------

  virtual DataSourceInterface* OpenDataSourceFile(int source_file_id) {
    return new DataSource(CreateServer(source_file_id, kQueryId));
  };

  virtual DataSinkInterface* OpenDataSink() {
    return new DataSink(CreateServer(kQueryId));
  };

 protected:
  virtual ~NodeEnvironment() {
    for (std::vector<NetworkOutput*>::const_iterator it = outputs_.begin();
         it != outputs_.end(); ++it) {
      delete (*it);
    }
    outputs_.clear();
  }

 private:
  const static int kQueryId = 12;
  uint32 node_number_;
  uint32 node_count_;
  boost::scoped_ptr<NetworkInput> input_;
  std::vector<NetworkOutput*> outputs_;
};

} // namespace

NodeEnvironmentInterface* CreateNodeEnvironment(int argc, char** argv) {
  CHECK(argc == 2, "Requires 1 argument [server_node:port]");
  IpAddress server_address = IpAddress::Parse(argv[1]);
  std::auto_ptr<NetworkInput> network_input(new NetworkInput());
  uint16_t listening_port = network_input->port();
  LOG1("Running node on port: %d", listening_port);
  fflush(stdout);

  boost::asio::io_service io_service;
  boost::scoped_ptr<tcp::socket> socket(
      ConnectClient(io_service, server_address.getHostAddress(),
                    server_address.getService()));
  CHECK(SendPacket(*socket, &listening_port, sizeof(listening_port)),
                   "Write failed");
  uint32_t length;
  boost::scoped_array<char> data(ReadPacket(*socket, &length));
  if (data.get() == NULL) {
    LOG0("Cannot load topology from server");
    return NULL;
  }
  TopologyAssigment assigment;
  LOG1("Parsing topology of length %d", length);
  if (!assigment.ParseFromArray(data.get(), length)) return NULL;
  LOG1("Got topology: %s", assigment.DebugString().c_str());
  std::vector<NetworkOutput*> outputs;
  for (int i = 0; i < assigment.peers_size(); ++i) {
    std::ostringstream ss;
    ss << assigment.peers(i).port();
    outputs.push_back(new NetworkOutput(assigment.peers(i).host(), ss.str()));
  }
  LOG0("Returning NodeEnvironment.");
  return new NodeEnvironment(assigment.your_number(),
                            network_input.release(),
                            outputs);
}
