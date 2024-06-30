#pragma once
#include <iostream>
// using Debug = UnityEngine.Debug�� ���� ����.
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
	std::string host;	// Unity Ŭ���̾�Ʈ�� ����Ǵ� ȣ��Ʈ �ּ�
	__int32 sendPort;		// Unity Ŭ���̾�Ʈ�� ������ ��Ʈ ��ȣ
	__int32 receivePort;	// C++ ������ ������ ��Ʈ ��ȣ
};

