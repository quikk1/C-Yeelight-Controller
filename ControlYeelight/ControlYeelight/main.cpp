#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <conio.h>
#pragma comment(lib, "ws2_32.lib")

void sendCommand(std::string iCommand, std::string ipAddr, int port) {

    std::string sCommand = iCommand + "\r\n";

    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if (wsResult != 0) {
        std::cerr << "Cant start Winsock... ERR ID: " << wsResult << std::endl;
        WSACleanup();

    }
    SOCKET cSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (cSocket == INVALID_SOCKET) {
        std::cerr << "Cant create socket... ERR ID: " << WSAGetLastError() << std::endl;
        WSACleanup();

    }
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddr.c_str(), &hint.sin_addr);

    int connResult = connect(cSocket, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR) {

        std::cerr << "Cant connect to server... ERR ID:" << connResult << std::endl;
        WSACleanup();

    }


    std::cout << ">Command: " << sCommand.c_str() << std::endl;

    int sendCommand = send(cSocket, sCommand.c_str(), sCommand.size() + 1, 0);

    if (sendCommand != SOCKET_ERROR) {

        char buf[4096];
        ZeroMemory(buf, 4096);

        int bytesReceived = recv(cSocket, buf, 4096, 0);
        if (bytesReceived > 0) {

            std::cout << "SERVER> " << std::string(buf, 0, bytesReceived) << std::endl;

        }
        else {

            std::cout << "Server response error..." << std::endl;

        }

    }

    closesocket(cSocket);
    WSACleanup();

}

int main() {
    
    int selection;
    std::string iIP;

    SetConsoleTitleA("C++ Yeelight Control");

    std::cout << "C++ YEELIGHT CONTROL" << std::endl << std::endl;

    std::cout << "Write Yeelight IP: " << std::endl;
    std::cin >> iIP;

    system("cls");

    while (true) {

        std::cout << "Select COMMAND:" << std::endl << std::endl;

        std::cout << "  1. Toggle light" << std::endl;
        std::cout << "  2. Set bright" << std::endl;
        std::cout << "  3. Set color temperature" << std::endl;
        std::cout << "  0. Exit" << std::endl << std::endl;

        std::cin >> selection;

        if (selection == 1)
            sendCommand("{ \"id\": 1, \"method\": \"toggle\", \"params\":[,,]}", "192.168.1.138", 55443);
        else if (selection == 2) {
            int iBrightness = 0;

            std::cout << std::endl << "Enter brightness (1-100): " << std::endl;
            std::cin >> iBrightness;

            if (iBrightness < 1)
                iBrightness = 1;

            if (iBrightness > 100)
                iBrightness = 100;

            sendCommand("{ \"id\": 1, \"method\": \"set_bright\", \"params\":[" + std::to_string(iBrightness) + ",\"smooth\",500] }", iIP, 55443);
        }
        else if (selection == 3) {
        
            int iTemp = 0;

            std::cout << std::endl << "Enter color temperature (2700-6500): " << std::endl;
            std::cin >> iTemp;

            if (iTemp < 2700)
                iTemp = 2700;
            if (iTemp > 6500)
                iTemp = 6500;

            sendCommand("{ \"id\": 1, \"method\": \"set_ct_abx\", \"params\":[" + std::to_string(iTemp) + ",\"smooth\",500] }", iIP, 55443);
        }
        else if (selection == 0)
            return 0;
        else
            std::cout << std::endl << "Command not found..." << std::endl;

        std::cout << std::endl << "Press any key to send another command!";

        _getch();

        system("cls");
  
    }

    return 0;

}