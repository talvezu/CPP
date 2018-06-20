/*
 * IPConfigurationSpec.h
 *
 *  Created on: May 4, 2018
 *      Author: me
 */

#ifndef IPCONFIGURATIONSPEC_H_
#define IPCONFIGURATIONSPEC_H_
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <stdint.h>


namespace IPConfigurationSpec{

typedef boost::shared_ptr< boost::asio::io_service> sptr_io_service;

enum class op_code: uint8_t {
	op_ok = 0,
	op_init_failed = (uint8_t)-1,
	op_sync_failed = (uint8_t)-2,
	op_async_failed = (uint8_t)-3,
	op_mode_illegal = (uint8_t)-4,
	undetermined = (uint8_t)-5
};

enum class Configuration: uint8_t {
	tcp_configuration = 0,
	udp_configuration = 1,
	undetermined = (uint8_t)-1
};

enum class SyncMode: uint8_t {
	async = 0,
	sync = 1,
	indeterminate = (uint8_t)-1
};

}//namespace




#endif /* IPCONFIGURATIONSPEC_H_ */
