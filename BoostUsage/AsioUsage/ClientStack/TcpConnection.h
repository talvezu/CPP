/*
 * Tcpconnection.h
 *
 *  Created on: May 4, 2018
 *      Author: me
 */

#ifndef TCPCONNECTION_H_
#define TCPCONNECTION_H_
#include "IPConfigurationSpec.h"
#include "SocketConnection.h"
#include "boost/array.hpp"
using namespace IPConfigurationSpec;
using boost::asio::ip::tcp;

class TcpConnection :public SocketConnection{

    IPConfigurationSpec::op_code sendAsync(std::string &&message);
    IPConfigurationSpec::op_code sendSync(std::string &&message);
    tcp::socket   _tcp_socket;
    tcp::endpoint _tcp_endpoint;
public:
	TcpConnection(IPConfigurationSpec::sptr_io_service p_io_service,
			      IPConfigurationSpec::SyncMode sync_mode,
				  std::string server_ip,
				  std::string server_port);
	IPConfigurationSpec::op_code init();
	IPConfigurationSpec::op_code send(std::string &&message);
	IPConfigurationSpec::op_code send(const boost::asio::mutable_buffer&& buffer);
	virtual ~TcpConnection();
};

#endif /* TCPCONNECTION_H_ */
