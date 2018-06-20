/*
 * TcpServer.cpp
 *
 *  Created on: Apr 8, 2018
 *      Author: me
 */

#include <iterator> //back_inserter
#include "TcpServer.h"
using boost::asio::ip::tcp;



TcpServer::TcpServer(boost::shared_ptr< boost::asio::io_service > io_service, int port)
		:_io_service(*io_service),
	    _socket(*io_service),
		_acceptor(*io_service, tcp::endpoint(tcp::v4(), port))
{
	    accept_connection();
}

void TcpServer::accept_connection()
{
  std::cout << "accepting connection" << std::endl;
  // Verify socket is in a closed state.
  _socket.close();
  // On success or failure, acceptor will open() socket_.

  auto f = [this](auto err){
	  std::cout<<"handle accept"<<std::endl;
	  // On error, return early.
	  if (err)
	  {
		  std::cout << "handle_accept: " << err.message() << std::endl;
		  return;
	  }
	  this->read();
  };
  _acceptor.async_accept(_socket, f);
}

void TcpServer::read()
{
    std::cout << "reading from socket" << std::endl;
    auto handle_read = [this](auto err, auto bytes_transferred){
      std::string data;
  	  std::cout<<"handle_read"<<" ,bytes_transferred:"<<bytes_transferred<<std::endl;
  	  std::copy(_buffer.begin(), _buffer.begin()+bytes_transferred, std::back_inserter(data));
  	  std::cout<<"data received :"<<data<<std::endl;
  	  /*
  	   * not working
  	  for (bytes_transferred)
  		  for (const char &c : _buffer)
  			  std::cout << c << '\n';
  	  */
  	  // On error, return early.
  	  if (err)
  	  {
  	      std::cout << "read: " << err.message() << std::endl;
  	      this->accept_connection();
  	      return;
  	  }
  	  this->read();
	  //std::string sClientIp = tcp::socket().remote_endpoint().address().to_string();
	  //unsigned short uiClientPort = tcp::socket().remote_endpoint().port();
	  //std::cout<<"ip:port "<<sClientIp<<":" <<uiClientPort<<std::endl;

    };
    async_read(_socket, boost::asio::buffer(_buffer),
      boost::asio::transfer_at_least(1),
      handle_read);
}

TcpServer::~TcpServer() {
	// TODO Auto-generated destructor stub
}

