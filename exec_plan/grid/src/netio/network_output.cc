/*
 * NetworkOutput.cpp
 *
 *  Created on: 25-04-2012
 *      Author: ptab
 */

#include "network_output.h"

#include <boost/asio.hpp>

#include "basic.h"
#include "utils/logger.h"
#include <string.h>

using boost::asio::ip::tcp;

NetworkOutput::NetworkOutput(const std::string& host,
                             const std::string& service)
    : host_(host),
      service_(service),
      socket_(NULL) {}

NetworkOutput::~NetworkOutput() {
}

bool NetworkOutput::EnsureConnectionExists() {
  if (socket_.get() == NULL || !socket_->is_open()) {
    socket_.reset(ConnectClient(io_service_, host_, service_));
  }
  return socket_.get() != NULL && socket_->is_open();
}

bool NetworkOutput::SendPacket(const char* data, std::size_t data_len) {
  mutex_.lock();
  try {
    if (!EnsureConnectionExists()) return false;
    uint32_t net_buffer_length = htonl(data_len);
    CHECK(boost::asio::write(*socket_,
                             boost::asio::buffer(&net_buffer_length,
                                                 sizeof(net_buffer_length)))
          == sizeof(net_buffer_length),
          "Write failed");
    CHECK(boost::asio::write(*socket_,
                             boost::asio::buffer(data, data_len)) == data_len,
          "Write failed");
    // LOG3("%s:%s Sent packet of length: %ld", host_.c_str(), service_.c_str(), data_len);
    mutex_.unlock();
    return true;
  } catch(...) {
    LOG2("%s:%s Exception.", host_.c_str(), service_.c_str());
  }
  return false;
}
