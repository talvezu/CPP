/*
 * UdpServer.cpp
 *
 *  Created on: Apr 8, 2018
 *      Author: me
 */

#include <iterator> //back_inserter
#include "UdpServer.h"
using boost::asio::ip::udp;

UdpServer::UdpServer(boost::shared_ptr< boost::asio::io_service > io_service, int port_number)
		:_io_service(*io_service),
		 _port(port_number),
		 _p_remote_endpoint{new udp::endpoint(boost::asio::ip::udp::udp::v4(), port_number)},
		 _socket(*io_service, *_p_remote_endpoint)
{
	start_receive();
}

void UdpServer::start_receive()
{
  std::cout << "UdpServer::start_receive on port:"<<_port<< std::endl;

  auto f = [this](auto err, std::size_t bytes_transferred){
	  std::string data;
  	  std::cout<<"handle_read"<<" ,bytes_transferred:"<<bytes_transferred<<std::endl;
  	  std::copy(_buffer.begin(), _buffer.begin()+bytes_transferred, std::back_inserter(data));
  	  std::cout<<"data received :"<<data<<std::endl;
	  // On error, return early.
	  if (err)
	  {
		  std::cout << "handle receive: " << err.message() << std::endl;
		  return;
	  }
	  this->start_receive();
  };
  _socket.async_receive_from(boost::asio::buffer(_buffer, _buffer.size()), *_p_remote_endpoint, f);

}


UdpServer::~UdpServer() {
	// TODO Auto-generated destructor stub
}

