/*
 * TcpConnection.h
 *
 *  Created on: Apr 8, 2018
 *      Author: me
 */

#ifndef UDPSERVER_H_
#define UDPSERVER_H_

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

class UdpServer {
	boost::asio::io_service& _io_service;
	int _port;
	udp::endpoint            *_p_remote_endpoint;
	udp::socket              _socket;
	boost::array<char, 1024> _buffer;
	void start_receive();
public:
	UdpServer(boost::shared_ptr< boost::asio::io_service > io_service, int port_number);
	virtual ~UdpServer();
};

#endif /* UDPSERVER_H_ */
