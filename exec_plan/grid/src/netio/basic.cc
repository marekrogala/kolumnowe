#include "basic.h"

#include "utils/logger.h"

using boost::asio::buffer;
using boost::asio::ip::tcp;
using boost::asio::write;
using boost::asio::read;

bool SendPacket(tcp::socket &socket, const void* data, uint32_t length) {
  try {
    uint32_t net_buffer_length = htonl(length);
    //LOG1("Sending: %ld", sizeof(net_buffer_length));
    CHECK(boost::asio::write(socket,
                             boost::asio::buffer(&net_buffer_length,
                                                 sizeof(net_buffer_length)))
          == sizeof(net_buffer_length),
          "Write failed");
    //LOG1("Sending: %;d", data.length());
    CHECK(boost::asio::write(socket,
                             boost::asio::buffer(data, length)) == length,
          "Write failed");
    // LOG3("%s:%s Sent packet of length: %ld", host_.c_str(), service_.c_str(), data_len);
    return true;
  } catch(...) {
    LOG2("%s:%d Exception.",
         socket.remote_endpoint().address().to_string().c_str(),
         socket.remote_endpoint().port());
  }
  return false;
}

bool SendPacket(tcp::socket &socket, const std::string& data) {
  return SendPacket(socket, data.data(), data.length());
}

char* ReadPacket(tcp::socket &socket, uint32_t* length) {
  uint32_t net_buffer_length;
  //LOG1("Reading packet header: %ld", sizeof(net_buffer_length));
  int read_bytes =
      read(socket, buffer(&net_buffer_length, sizeof(net_buffer_length)));
  //LOG1("Was able to read: %d", read_bytes);
  if (read_bytes != sizeof(net_buffer_length)) return NULL;
  *length = ntohl(net_buffer_length);
  char* data = new char[*length];
  //LOG1("Reading length: %d", *length);
  if (read(socket, buffer(data, *length)) != *length)
    return NULL;
  //LOG0("Reading done");
  return data;
}

tcp::socket* ConnectClient(boost::asio::io_service& io_service,
                           const std::string& host,
                           const std::string& service) {
  std::auto_ptr<tcp::socket> socket(new tcp::socket(io_service));
  tcp::resolver resolver(io_service);
  tcp::resolver::query query(host, service);
  tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
  tcp::resolver::iterator end;
  boost::system::error_code error = boost::asio::error::host_not_found;
  while (error && endpoint_iterator != end) {
    socket->close();
    socket->connect(*endpoint_iterator++, error);
  }
  if (error) {
    LOG3("%s:%s, Cannot connect: %s", host.c_str(), service.c_str(),
         error.message().c_str());
    return NULL;
  }
  LOG2("%s:%s, Connected.", host.c_str(), service.c_str());
  return socket.release();
}
