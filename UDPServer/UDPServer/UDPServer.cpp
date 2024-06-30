// UDPServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

#include "ServerManager.h"

// using Debug = UnityEngine.Debug와 같은 개념.

int main() {
    try {
        ServerManager* server = new ServerManager();
        boost::asio::io_service io_service;

        //std::string host = "127.0.0.1"; // Unity 클라이언트가 실행되는 호스트 주소
        //int send_port = 9000; // Unity 클라이언트가 수신할 포트 번호
        //int receive_port = 9001; // C++ 서버가 수신할 포트 번호

        std::thread server_thread([server]() {server->StartServer();});

        while (true) {
            std::cout << "Press 's' to send packet to Unity client, 'q' to quit: ";
            char key;
            std::cin >> key;

            if (key == 's') {
                std::string message = "Hello from C++ server!";
                server->SendPacket(message);
                std::cout << "Packet sent to Unity client.\n";
            }
            else if (key == 'q') {
                break;
            }
        }

        /*
            이 부분은 메인 스레드가 server_thread가 종료될 때까지 기다리게 합니다. 
            이를 통해 server_thread가 완전히 종료되기 전에 프로그램이 종료되지 않도록 보장합니다.
        */
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
