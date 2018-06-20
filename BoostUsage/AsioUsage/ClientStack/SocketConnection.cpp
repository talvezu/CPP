/*
 * RemoteConnectionMgr.cpp
 *
 *  Created on: May 3, 2018
 *      Author: me
 */

#include "SocketConnection.h"


SocketConnection::~SocketConnection() {
	// TODO Auto-generated destructor stub
}
/*TODO complete moving sent to base class
op_code SocketConnection::send(std::string &&message)
{
	if (_sync_mode == IPConfigurationSpec::SyncMode::sync)
		return sendSync(std::move(message));
	else if (_sync_mode == IPConfigurationSpec::SyncMode::async)
		return sendAsync(std::move(message));
	else
		return  op_code::op_mode_illegal;

}
*/