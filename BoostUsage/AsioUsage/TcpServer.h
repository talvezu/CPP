/*
 * TcpConnection.h
 *
 *  Created on: Apr 8, 2018
 *      Author: me
 */

#ifndef TCPSERVER_H_
#define TCPSERVER_H_

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class TcpServer {
	boost::asio::io_service& _io_service;
	tcp::socket              _socket;
	tcp::acceptor            _acceptor;
	boost::array<char, 1024> _buffer;


public:
	//TcpConnection(boost::asio::io_service& io_service);
	TcpServer(boost::shared_ptr< boost::asio::io_service > io_service, int port);
	void accept_connection();
	void read();
	virtual ~TcpServer();
};

#endif /* TCPSERVER_H_ */
