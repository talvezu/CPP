/*
 * UdpConnection.cpp
 *
 *  Created on: May 4, 2018
 *      Author: me
 */

#include "UdpConnection.h"

using namespace IPConfigurationSpec;


UdpConnection::UdpConnection(IPConfigurationSpec::sptr_io_service p_io_service,
		                     SyncMode sync_mode,
							 std::string server_ip,
							 std::string server_port):
SocketConnection(p_io_service, sync_mode, server_ip, server_port),
_p_udp_socket{new udp::socket (*_p_io_service, boost::asio::ip::udp::udp::endpoint(boost::asio::ip::udp::udp::v4(), 0))}
{


}

op_code UdpConnection::init()
{
	op_code op = op_code::op_ok;
	try
		{
		    udp::resolver resolver(*_p_io_service);
		    udp::resolver::query query(boost::asio::ip::udp::udp::v4(), _destination_ip, _destination_port);
		    udp::resolver::iterator iter = resolver.resolve(query);
		    _udp_endpoint = *iter;
		    std::cout << "WheelHandler::init resoved query for: " << _udp_endpoint << std::endl;

		}
		catch( std::exception & ex )
		{
			//global_stream_lock.lock();
			std::cout << "[" << boost::this_thread::get_id()
				<< "] Exception: " << ex.what() << std::endl;
			//global_stream_lock.unlock();
			op = op_code::op_init_failed;
		}

	return op;

}

op_code UdpConnection::sendAsync(std::string &&message)
{
	op_code op = op_code::op_ok;
	try
		{
			std::cout<<"---------"<<std::endl;
			auto f = [this](const boost::system::error_code& error, std::size_t bytes_transferred){
				std::cout<<"WheelHandler::sendAsyncWheelEvent()::f"<<std::endl;

			};
			_p_udp_socket->async_send_to(boost::asio::buffer(message, message.length()),_udp_endpoint, f);


		}
		catch( std::exception & ex )
		{
			//global_stream_lock.lock();
			std::cout << "[" << boost::this_thread::get_id()
				<< "] Exception: " << ex.what() << std::endl;
			//global_stream_lock.unlock();
			op = op_code::op_async_failed;
		}
	return op;
}

op_code UdpConnection::sendSync(std::string &&message)
{
	op_code op = op_code::op_ok;
	try
		{
			std::cout << "Connecting to: " << _udp_endpoint << std::endl;
			_p_udp_socket->send_to(boost::asio::buffer(send_buf), _udp_endpoint);
		}
		catch( std::exception & ex )
		{
			//global_stream_lock.lock();
			std::cout << "[" << boost::this_thread::get_id()
				<< "] Exception: " << ex.what() << std::endl;
			//global_stream_lock.unlock();
			op = op_code::op_sync_failed;
		}
	return op;
}

op_code UdpConnection::send(std::string &&message)
{
	if (_sync_mode == IPConfigurationSpec::SyncMode::sync)
		return sendSync(std::move(message));
	else if (_sync_mode == IPConfigurationSpec::SyncMode::async)
		return sendAsync(std::move(message));
	else
		return  op_code::op_mode_illegal;

}

op_code UdpConnection::send(const boost::asio::mutable_buffer&& buffer){
	op_code op = op_code::op_ok;

	try
		{
			std::cout << "Connecting to: " << _udp_endpoint << std::endl;
			if (_sync_mode == IPConfigurationSpec::SyncMode::sync)
				_p_udp_socket->send_to(boost::asio::buffer(buffer), _udp_endpoint);
			else{
				auto f = [this](const boost::system::error_code& error, std::size_t bytes_transferred){
					std::cout<<"UdpConnection::send(mutable_buffer)::f"<<std::endl;

				};
				_p_udp_socket->async_send_to(boost::asio::buffer(buffer),_udp_endpoint, f);
			}
		}
		catch( std::exception & ex )
		{
			//global_stream_lock.lock();
			std::cout << "[" << boost::this_thread::get_id()
				<< "] Exception: " << ex.what() << std::endl;
			//global_stream_lock.unlock();
			op = op_code::op_sync_failed;
		}
	return op;
}

UdpConnection::~UdpConnection() {
	// TODO Auto-generated destructor stub
}