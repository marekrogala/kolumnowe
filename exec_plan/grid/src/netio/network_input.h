#ifndef NETWORK_INPUT_H_
#define NETWORK_INPUT_H_

#include <vector>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/thread/thread.hpp>
#include "utils/pcqueue.h"

struct Packet {
  Packet(char* data, std::size_t size)
      : data_(data), size_(size) {};

  Packet(uint32_t size)
        : data_(new char[size]()), size_(size) {};

  virtual ~Packet() { if (data_ != NULL)  delete[] data_;};

  char* release_data() {
    char* result = data_;
    data_  =NULL;
    return result;
  }

  char* data() const { return data_; }

  std::size_t size() const { return size_; }

 private:
  char* data_;
  std::size_t size_;
};

class InputConnection {
 public:
  InputConnection(boost::asio::io_service* io_service,
                  boost::asio::ip::tcp::socket* socket,
                  boost::asio::ip::tcp::endpoint* endpoint,
                  util::PCQueue<Packet*>* pc_queue);

  virtual ~InputConnection();

  void read_handle(const boost::system::error_code& error,
                   std::size_t bytes_transferred);

  void schedule_read();

  std::size_t write(std::size_t size, char* data);

  bool SendPacket(const std::string& packet);
  const boost::asio::ip::tcp::endpoint& endpoint() const { return *endpoint_; }

  bool is_active() const {
    return socket_->is_open() &&
        ((status_ == WAIT_FOR_DATA) || (status_ == WAIT_FOR_LENGTH));
  };

 private:
  enum StatusEnum {
    WAIT_FOR_LENGTH,
    WAIT_FOR_DATA,
    DISCONNECTED
  };

  uint32_t buffer_length() const {
    return ntohl(net_buffer_length_);
  }

  std::string describe() const;

  StatusEnum status_;
  boost::asio::io_service* io_service_;
  boost::scoped_ptr<boost::asio::ip::tcp::socket> socket_;
  boost::scoped_ptr<boost::asio::ip::tcp::endpoint> endpoint_;
  util::PCQueue<Packet*>* pc_queue_;

  uint32_t net_buffer_length_; // Stored in network byte-order.
  std::auto_ptr<Packet> buffer_content_;
};


class NetworkInput {
 public:
  NetworkInput();
  NetworkInput(short listening_port);
  virtual ~NetworkInput();

  // Reads a single packet sent to this node.
  // Blocks if there is not packet ready until the packet arrive.
  // Caller takes ownership of the returned packet and should
  // destroy it using delete[].
  virtual char* ReadPacketBlocking(std::size_t* data_len);

  // Returns NULL if there is no packet waiting
  // Updates data_len to contain the size of the read packet.
  // Caller takes ownership of the returned packet and should
  // destroy it using delete[].
  virtual char* ReadPacketNotBlocking(std::size_t* data_len);

  int connections_count() const;
  const boost::asio::ip::tcp::endpoint& endpoint(int connection_id) const;
  bool is_active(int connection_id) const;

  uint16_t port() const { return acceptor_.local_endpoint().port(); };

 private:
  void accept_connection(const boost::system::error_code& e);

  const static std::size_t kThreadsCount = 8;
  const static std::size_t kQueueSize = 2 * kThreadsCount;

  void start_accept();

  mutable boost::mutex mutex_;
  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;
  std::auto_ptr<boost::asio::ip::tcp::socket> waiting_socket_;
  std::auto_ptr<boost::asio::ip::tcp::endpoint> waiting_endpoint_;
  std::vector<boost::shared_ptr<boost::thread> > threads_;
  std::vector<InputConnection*> connections_;
  boost::scoped_ptr<util::PCQueue<Packet*> > pc_queue_;
};

#endif /* NETWORK_INPUT_H_ */
