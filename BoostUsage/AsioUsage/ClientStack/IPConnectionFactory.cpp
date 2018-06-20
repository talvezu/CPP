/*
 * IPConnectionFactory.cpp
 *
 *  Created on: May 4, 2018
 *      Author: me
 */
#include "IPConfigurationSpec.h"
#include "IPConnectionFactory.h"
#include "SocketConnection.h"
#include "UdpConnection.h"
#include "TcpConnection.h"


IPConnectionFactory::IPConnectionFactory() {
	// TODO Auto-generated constructor stub
}

SocketConnection *IPConnectionFactory::createConnection(
		IPConfigurationSpec::sptr_io_service p_io_service,
		IPConfigurationSpec::Configuration conf,
		IPConfigurationSpec::SyncMode mode,
		std::string &&server_ip,
		std::string &&server_port)
{
	if (IPConfigurationSpec::Configuration::tcp_configuration == conf )
		return new TcpConnection(p_io_service,mode,server_ip,server_port);
	else if (IPConfigurationSpec::Configuration::udp_configuration == conf )
		return new UdpConnection(p_io_service,mode,server_ip,server_port);
	else
		return NULL;

}

IPConnectionFactory::~IPConnectionFactory() {
	// TODO Auto-generated destructor stub
}