/*
 * Tcpconnection.cpp
 *
 *  Created on: May 4, 2018
 *      Author: me
 */

#include "TcpConnection.h"
#include <iostream>
TcpConnection::TcpConnection(IPConfigurationSpec::sptr_io_service p_io_service,
        SyncMode sync_mode,
		std::string server_ip,
		std::string server_port):
SocketConnection(p_io_service, sync_mode,server_ip, server_port),
_tcp_socket(*this->_p_io_service)
{

	std::cout<<"TcpConnection\n"<<std::endl;
}

IPConfigurationSpec::op_code TcpConnection::init()
{
	op_code op = op_code::op_ok;
	try
		{
			tcp::resolver tcp_resolver(*_p_io_service);
			tcp::resolver::query query(
				_destination_ip,
				_destination_port
			);
			tcp::resolver::iterator iterator = tcp_resolver.resolve(query);
			_tcp_endpoint = *iterator;
            if(_sync_mode == SyncMode::async){

				auto f = [](const boost::system::error_code& ec){
					std::cout<<"---------"<<std::endl;
					if( ec )
					{
						//global_stream_lock.lock();
						std::cout << "[" << boost::this_thread::get_id()
								<< "] Error: " << ec << std::endl;
						//global_stream_lock.unlock();
					}
					else
					{
						//global_stream_lock.lock();
						std::cout << "[" << boost::this_thread::get_id()
								<< "] Connected!" << std::endl;
						//global_stream_lock.unlock();
					}
				};
				_tcp_socket.async_connect(_tcp_endpoint, f);
				_p_io_service->run();
            }
            else
                _tcp_socket.connect(_tcp_endpoint);

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


IPConfigurationSpec::op_code TcpConnection::sendAsync(std::string &&message)
{
	op_code op = op_code::op_ok;
	try
	{
		std::cout<<"---------"<<std::endl;
		//_tcp_socket.connect(_endpoint);
		auto f = [this](const boost::system::error_code& error, std::size_t bytes_transferred){
			//this->_tcp_socket.async_send(boost::asio::buffer(buf, message.size()), error);
			std::cout<<"TcpConnection::sendAsync()::f"<<std::endl;
		};
		//_tcp_socket.async_send( _endpoint, f );
		_tcp_socket.async_send( boost::asio::buffer(message, message.length()), f );

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

IPConfigurationSpec::op_code TcpConnection::sendSync(std::string &&message)
{
	op_code op = op_code::op_ok;
	try
	{
		boost::asio::ip::tcp::resolver resolver( *_p_io_service );
		boost::asio::ip::tcp::socket sock( *_p_io_service );
		boost::asio::ip::tcp::resolver::query query(
			_destination_ip,
			_destination_port
		);
		boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve( query );
		boost::asio::ip::tcp::endpoint endpoint = *iterator;

		std::cout << "Connecting to: " << endpoint << std::endl;

		sock.connect(endpoint);
		boost::array<char, 128> buf;
		boost::system::error_code error;
		std::copy(buf.begin(), buf.begin() + sizeof(message), std::back_inserter(message));

		//std::copy(message.begin(),message.end(),buf.begin());
		sock.write_some(boost::asio::buffer(buf, message.size()), error);
		sock.close();
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

op_code TcpConnection::send(std::string &&message)
{
	if (_sync_mode == IPConfigurationSpec::SyncMode::sync)
		return sendSync(std::move(message));
	else if (_sync_mode == IPConfigurationSpec::SyncMode::async)
		return sendAsync(std::move(message));
	else
		return  op_code::op_mode_illegal;

}

op_code TcpConnection::send(const boost::asio::mutable_buffer&& buffer){
	op_code op = op_code::op_ok;
	//boost::array<char, 128> buf;
	boost::system::error_code error;
	//std::copy(buffer.begin(),buffer.end(),buf.begin());
	_tcp_socket.write_some(boost::asio::buffer(buffer,128), error);

	return op;
}

TcpConnection::~TcpConnection() {
	// TODO Auto-generated destructor stub
}

