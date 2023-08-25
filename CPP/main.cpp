#include <iostream>
#include <thread>
#include <cstring>
#include <ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
#include <shellapi.h>
#include <vector>
class MainFunction {
private:
    SOCKET sock;
    uint32_t header[2] = { 0x12345678, 0 };

    void findHardware() {
        wchar_t currentDir[MAX_PATH];
        GetCurrentDirectoryW(MAX_PATH, currentDir); 
        std::wstring mapper = std::wstring(currentDir) + L"data\\Mapper.exe";
        std::wstring driver = std::wstring(currentDir) + L"data\\mouseMoveDriver.sys";
        std::this_thread::sleep_for(std::chrono::seconds(1));

        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(6666);
        if (inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) != 1) {
            MessageBoxA(0, "error 405", 0, MB_ICONWARNING);
            exit(1);
        }
        if (connect(sock, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
            MessageBoxA(0, "error 402", 0, MB_ICONWARNING);
            exit(1);
        }
    }

    void send_packet(uint32_t packet_data[5]) {
        int dataSize = sizeof(uint32_t) * 5;
        std::vector<char> buffer(dataSize);
        memcpy(buffer.data(), packet_data, dataSize);
        send(sock, buffer.data(), dataSize, 0);
    }

public:
    MainFunction() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            MessageBoxA(0, "error 401", 0, MB_ICONWARNING);
            exit(1);
        }

        sock = socket(AF_INET, SOCK_STREAM, 0);
        findHardware();
    }

    ~MainFunction() {
        closesocket(sock);
        WSACleanup();
    }

    void deactivate() {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }

    void move(int x, int y) {
        uint32_t memory_data[5] = { header[0], header[1], static_cast<uint32_t>(x), static_cast<uint32_t>(y), 0 };
        send_packet(memory_data);


    }

    void shoot() {
        uint32_t packet[5] = { header[0], header[1], 0, 0, 0x1 };
        send_packet(packet);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        packet[4] = 0x2;
        send_packet(packet);
    }
};

int main() {
	/// brazil on top , the kings 
    ///just copying and pasting
    MainFunction obj;
    obj.move(10, 20);
    return 0;
}
