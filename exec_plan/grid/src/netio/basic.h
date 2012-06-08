#ifndef BASIC_H_
#define BASIC_H_

#include <boost/asio.hpp>

bool SendPacket(boost::asio::ip::tcp::socket &socket, const std::string& data);
bool SendPacket(boost::asio::ip::tcp::socket &socket, const void* data, uint32_t length);

char* ReadPacket(boost::asio::ip::tcp::socket &socket, uint32_t* length);

boost::asio::ip::tcp::socket* ConnectClient(boost::asio::io_service& io_service,
                                            const std::string& host,
                                            const std::string& service);


#endif /* BASIC_H_ */
