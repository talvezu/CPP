/*
 * UdpConnection.h
 *
 *  Created on: May 4, 2018
 *      Author: me
 */

#ifndef UDPCONNECTION_H_
#define UDPCONNECTION_H_

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "IPConfigurationSpec.h"
#include "SocketConnection.h"
#include <string>

using std::string;

using boost::asio::ip::udp;

class UdpConnection : public SocketConnection {

	string port;
    udp::socket *_p_udp_socket;
    udp::endpoint _udp_endpoint;
    boost::array<char, 1> send_buf  = {{ 0 }};

    IPConfigurationSpec::op_code sendAsync(std::string &&message);
    IPConfigurationSpec::op_code sendSync(std::string &&message);
public:
	UdpConnection(IPConfigurationSpec::sptr_io_service p_io_service,
			      IPConfigurationSpec::SyncMode sync_mode,
				  std::string server_ip,
				  std::string server_port);
	IPConfigurationSpec::op_code init();

	IPConfigurationSpec::op_code send(std::string &&message);
	IPConfigurationSpec::op_code send(const boost::asio::mutable_buffer&& buffer);
	virtual ~UdpConnection();
};

#endif /* UDPCONNECTION_H_ */
