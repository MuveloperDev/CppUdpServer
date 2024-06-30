#pragma once
#include <iostream>
// using Debug = UnityEngine.Debug와 같은 개념.
#include <boost/asio.hpp>
//using boost::asio::ip::udp;

class ServerManager
{
public:
	ServerManager();
	~ServerManager();

public:
	void LogClientInfo(const boost::asio::ip::udp::endpoint& clientEndPoint);
	void SendPacket(boost::asio::io_service& io_service, const std::string& message, const std::string& host, int port);
	void SendPacket(const std::string& message);
	void StartServer(boost::asio::io_service& io_service, short port);
	void StartServer();
public:
	const boost::asio::io_service& GetIoServer();
	const std::string GetHost();
	const __int32 GetSendPort();
	const __int32 GetReceivePort();
private:
	boost::asio::io_service* ioServer;
	std::string host;	// Unity 클라이언트가 실행되는 호스트 주소
	__int32 sendPort;		// Unity 클라이언트가 수신할 포트 번호
	__int32 receivePort;	// C++ 서버가 수신할 포트 번호
};

