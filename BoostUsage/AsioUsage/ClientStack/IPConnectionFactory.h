/*
 * IPConnectionFactory.h
 *
 *  Created on: May 4, 2018
 *      Author: me
 */

#ifndef IPCONNECTIONFACTORY_H_
#define IPCONNECTIONFACTORY_H_

#include "IPConfigurationSpec.h"
#include "SocketConnection.h"

//using namespace IPConfigurationSpec;

class IPConnectionFactory {

public:

	IPConnectionFactory();
	SocketConnection *createConnection(
			IPConfigurationSpec::sptr_io_service p_io_service,
			IPConfigurationSpec::Configuration conf,
			IPConfigurationSpec::SyncMode mode,
			std::string &&server_ip,
			std::string &&server_port);
	virtual ~IPConnectionFactory();
};

#endif /* IPCONNECTIONFACTORY_H_ */
