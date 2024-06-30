#include "ServerManager.h"
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;
ServerManager::ServerManager() : host("127.0.0.1"), sendPort(9000), receivePort(9001),
	ioServer(new boost::asio::io_service())
{
}

ServerManager::~ServerManager()
{
}

void ServerManager::LogClientInfo(const udp::endpoint& clientEndPoint)
{
	std::cout << "Client connected: " << clientEndPoint.address().to_string()
		<< ":" << clientEndPoint.port() << std::endl;
}

void ServerManager::SendPacket(boost::asio::io_service& io_service, const std::string& message, const std::string& host, int port)
{
	udp::socket socket(io_service);
	udp::endpoint remote_endpoint(boost::asio::ip::address::from_string(host), port);
	socket.open(udp::v4());
	socket.send_to(boost::asio::buffer(message), remote_endpoint);
}

void ServerManager::SendPacket(const std::string& message)
{
	SendPacket(*ioServer, message, host, sendPort);
}

void ServerManager::StartServer(boost::asio::io_service& io_service, short port)
{
	udp::socket socket(io_service, udp::endpoint(udp::v4(), port));

	char recv_buffer[1024];
	udp::endpoint remote_endpoint;

	while (true) {
		std::size_t len = socket.receive_from(boost::asio::buffer(recv_buffer), remote_endpoint);
		LogClientInfo(remote_endpoint);
		std::cout << "\nReceived: " << std::string(recv_buffer, len) << std::endl;
	}
}

void ServerManager::StartServer()
{
	StartServer(*ioServer, receivePort);
}

const boost::asio::io_service& ServerManager::GetIoServer()
{
	return *ioServer;
}

const std::string ServerManager::GetHost()
{
	return host;
}

const int ServerManager::GetSendPort()
{
	return sendPort;
}

const __int32 ServerManager::GetReceivePort()
{
	return receivePort;
}

