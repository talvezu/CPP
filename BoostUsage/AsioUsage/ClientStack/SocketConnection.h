/*
 * SocketConnection.h
 *
 *  Created on: May 3, 2018
 *      Author: me
 */

#ifndef SOCKETCONNECTION_H_
#define SOCKETCONNECTION_H_
#include "IPConfigurationSpec.h"
#include <boost/thread/thread.hpp>
#include <string>

using string = std::string;

class SocketConnection {

protected:
	IPConfigurationSpec::sptr_io_service _p_io_service;
	IPConfigurationSpec::SyncMode _sync_mode;
	string _destination_ip;
	string _destination_port;
public:

	SocketConnection( IPConfigurationSpec::sptr_io_service p_io_service,
					  IPConfigurationSpec::SyncMode sync_mode,
					  std::string server_ip,
					  std::string server_port):
		_p_io_service(p_io_service),
		_sync_mode(sync_mode),
		_destination_ip(server_ip),
		_destination_port(server_port)
	{}
	virtual IPConfigurationSpec::op_code send(std::string &&message){
		return IPConfigurationSpec::op_code::undetermined;
	}

	virtual IPConfigurationSpec::op_code send(const boost::asio::mutable_buffer&& buffer){
		return IPConfigurationSpec::op_code::undetermined;
	}

	virtual IPConfigurationSpec::op_code init(){return IPConfigurationSpec::op_code::op_init_failed;}
	//virtual void tmp() = 0;
	virtual ~SocketConnection();
};

#endif /* SOCKETCONNECTION_H_ */
