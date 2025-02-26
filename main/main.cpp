#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <inaddr.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

// To start the server in the terminal, you need to write the following commands 
// (g++ -o main.exe main.cpp -lws2_32) and (./main)

using namespace std;

int Inet_pton(int af, const char *src, void *dst) {
    struct sockaddr_storage ss;
    int size = sizeof(ss);
    char src_copy[INET6_ADDRSTRLEN + 1];

    ZeroMemory(&ss, sizeof(ss));

    strncpy_s(src_copy, src, INET6_ADDRSTRLEN + 1);
    src_copy[INET6_ADDRSTRLEN] = 0;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return 0;
    }

    // Копіювання рядка з перевіркою довжини
    if (strlen(src) > INET6_ADDRSTRLEN) {
        return 0; // Рядок занадто довгий
    }

    if (WSAStringToAddressA(src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0) {
        switch (af) {
            case AF_INET:
                *(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr;
                return 1;
            case AF_INET6:
                *(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr;
                return 1;
        }
    }
    return 0;
}

int main(void) {
    // Key constants
    const char SERVER_IP[] = "127.0.0.1"; // Enter IPv4 address of Server
    const short SERVER_PORT_NUM = 8081;   // Enter Listening port on Server side
    const short BUFF_SIZE = 1024;         // Maximum size of buffer for exchange info between server and client

    // Key variables for all program
    int erStat; // For checking errors in socket functions

    // IP in string format to numeric format for socket functions.
    in_addr ip_to_num;
    if (Inet_pton(AF_INET, SERVER_IP, &ip_to_num) == 0) {
        cout << "Invalid IP address format" << endl;
        return 1;
    }

    // WinSock initialization
    WSADATA wsData;
    erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

    if (erStat != 0) {
        cout << "Error WinSock version initialization # " << WSAGetLastError() << endl;
        return 1;
    } else {
        cout << "WinSock initialization is OK" << endl;
    }

    // Socket initialization
    SOCKET ClientSock = socket(AF_INET, SOCK_STREAM, 0);

    if (ClientSock == INVALID_SOCKET) {
        cout << "Error initialization socket # " << WSAGetLastError() << endl;
        closesocket(ClientSock);
        WSACleanup();
        return 1;
    } else {
        cout << "Client socket initialization is OK" << endl;
    }

    // Establishing a connection to Server
    sockaddr_in servInfo;
    ZeroMemory(&servInfo, sizeof(servInfo));

    servInfo.sin_family = AF_INET;
    servInfo.sin_addr = ip_to_num;
    servInfo.sin_port = htons(SERVER_PORT_NUM);

    erStat = connect(ClientSock, (sockaddr*)&servInfo, sizeof(servInfo));

    if (erStat != 0) {
        cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << endl;
        closesocket(ClientSock);
        WSACleanup();
        return 1;
    } else {
        cout << "Connection established SUCCESSFULLY. Ready to send a message to Server" << endl;
    }

    // Exchange text data between Server and Client.
    // Disconnection if a Client sends "xxx"
    vector<char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);
    short packet_size = 0; // The size of sending/receiving packet in bytes

    while (true) {
        cout << "enter a number for\n"
        "1. Add last name to first name\n"
             << "2. Find the maximum and minimum number from an array\n"
             << "3. array of four numbers add average to each\n"
             << "4. whether there are symbols in the text\n"
             << "5. 'exit' \n";
        string userInput;
        int choice;
        cin >> choice;
        cin.ignore();  // Очищаємо буфер вводу
    
        vector<int> arr; 
        stringstream ss; 
        string array_str;

        switch (choice) {
    case 1:
            cout << "Enter your name: ";
            getline(cin, userInput);
            packet_size = send(ClientSock, userInput.c_str(), userInput.size(), 0);
            if (packet_size == SOCKET_ERROR) {
            cout << "Failed to send message. Error #" << WSAGetLastError() << endl;
        }
        break;
    case 2:
        arr.resize(6); 
        cout << "Enter 5 numbers:\n";
        for (int i = 0; i < 6; i++) {
            if(i == 0){
                arr[0] = 1;
            }
            else{
            cin >> arr[i];
            cin.ignore();
            } 
        }

        ss.clear();  
        ss.str("");
        for (int i = 0; i < 6; ++i) {
            ss << arr[i];
            if (i < 5) ss << " ";
        }
        array_str = ss.str();

        packet_size = send(ClientSock, array_str.c_str(), array_str.size(), 0);
        if (packet_size == SOCKET_ERROR) {
            cout << "Failed to send array. Error #" << WSAGetLastError() << endl;
        }
        break;
    case 3:
        arr.resize(5); 
        cout << "Enter 4 numbers:\n";
        for (int i = 0; i < 5; i++) {
           if(i == 0){
            arr[0] = 2;
           }
           else{
            cin >> arr[i];
           }
            
        }

        ss.clear();  
        ss.str("");

        for (int i = 0; i < 5; ++i) {
            ss << arr[i];
            if (i < 4) ss << " ";
        }
        array_str = ss.str();

        packet_size = send(ClientSock, array_str.c_str(), array_str.size(), 0);
        if (packet_size == SOCKET_ERROR) {
            cout << "Failed to send array. Error # " << WSAGetLastError() << endl;
        }
        break;
    case 4:
        cout << "Enter your Text: ";
        getline(cin, userInput);
        userInput ='3' + userInput;
        packet_size = send(ClientSock, userInput.c_str(), userInput.size(), 0);
        if (packet_size == SOCKET_ERROR) {
        cout << "Failed to send message. Error #" << WSAGetLastError() << endl;
}
        break;
    case 5:
        cout << "Exit the program..." << endl;
        closesocket(ClientSock);
        WSACleanup();
        return 0;
    default:
        cout << "Wrong choice. Continuing the loop..." << endl;
        break;
}
if (choice >= 1 && choice <= 4) {
    packet_size = recv(ClientSock, servBuff.data(), servBuff.size(), 0);
    if (packet_size == SOCKET_ERROR) {
        cout << "Failed to receive message. Error # " << WSAGetLastError() << endl;
    } else if (packet_size == 0) {
        cout << "The server has gone offline." << endl;
        break;
    }

    string response(servBuff.data(), packet_size);
    cout << "Server response: " << response << endl;
    }
    }
    

    closesocket(ClientSock);
    WSACleanup();
    return 0;
}
