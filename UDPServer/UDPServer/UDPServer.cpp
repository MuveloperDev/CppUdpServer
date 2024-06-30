// UDPServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <boost/asio.hpp>

// using Debug = UnityEngine.Debug와 같은 개념.
using boost::asio::ip::udp;

void log_client_info(const udp::endpoint& client_endpoint) {
    std::cout << "Client connected: " << client_endpoint.address().to_string()
        << ":" << client_endpoint.port() << std::endl;
}

void send_packet(boost::asio::io_service& io_service, const std::string& message, const std::string& host, int port) {
    udp::socket socket(io_service);
    udp::endpoint remote_endpoint(boost::asio::ip::address::from_string(host), port);
    socket.open(udp::v4());
    socket.send_to(boost::asio::buffer(message), remote_endpoint);
}

void start_server(boost::asio::io_service& io_service, short port) {
    udp::socket socket(io_service, udp::endpoint(udp::v4(), port));

    char recv_buffer[1024];
    udp::endpoint remote_endpoint;

    while (true) {
        std::size_t len = socket.receive_from(boost::asio::buffer(recv_buffer), remote_endpoint);
        log_client_info(remote_endpoint);
        std::cout << "Received: " << std::string(recv_buffer, len) << std::endl;
    }
}

int main() {
    try {
        boost::asio::io_service io_service;

        std::string host = "127.0.0.1"; // Unity 클라이언트가 실행되는 호스트 주소
        int send_port = 9000; // Unity 클라이언트가 수신할 포트 번호
        int receive_port = 9001; // C++ 서버가 수신할 포트 번호

        std::thread server_thread([&io_service, receive_port]() {
            start_server(io_service, receive_port);
            });

        while (true) {
            std::cout << "Press 's' to send packet to Unity client, 'q' to quit: ";
            char key;
            std::cin >> key;

            if (key == 's') {
                std::string message = "Hello from C++ server!";
                send_packet(io_service, message, host, send_port);
                std::cout << "Packet sent to Unity client.\n";
            }
            else if (key == 'q') {
                break;
            }
        }

        server_thread.join();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
