#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <vector>
#include <windows.h>
#include <sstream>
#include <algorithm>
#include <limits>



#pragma comment(lib , "Ws2_32.lib")
using namespace std;

static const char *
Inet_ntop(int address_family, const void *source,
          char *destination, socklen_t destination_length) {
    DWORD socket_address_length;
    DWORD winsock_destination_length = destination_length;

    switch (address_family) {
        case AF_INET:
            socket_address_length = sizeof(struct sockaddr_in);
            break;
        case AF_INET6:
            socket_address_length = sizeof(struct sockaddr_in6);
            break;
        default:
            return NULL;
            break;
    }

    if (WSAAddressToStringA((LPSOCKADDR)source, socket_address_length, NULL,
                           (LPSTR)destination, &winsock_destination_length))
        return NULL;
    else
        return destination;
}

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
    strcpy_s(src_copy, sizeof(src_copy), src);

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


string addSurname(const string& name) {
    return name + " Bobro";
}

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

string numbersMinMax(const string& numbers) {
    istringstream iss(numbers);
    vector<int> nums;
    int num;

    while (iss >> num){
        nums.push_back(num);
    }

    if (nums.empty()){
        return "No numbers";
    }

    nums.erase(nums.begin());

    if (nums.empty()){
        return "No valid numbers after 1";
    }

    int minNum = nums[0], maxNum = nums[0];

    for (int n : nums){
        if (n < minNum) minNum = n;
        if (n > maxNum) maxNum = n;
    }

    return to_string(maxNum) + " " + to_string(minNum);
}



string add_arithmetic_mean(const string& numbers) {
    istringstream iss(numbers);
    vector<int> nums;
    int num;

    while (iss >> num) {
        nums.push_back(num);
    }

    if (nums.empty()) {
        return "No numbers";
    }

    double mean = ((nums[0] + nums[1] + nums[2] + nums[3] ) / 4);
    
    for (int& n : nums) {
        n += mean;
    }

    ostringstream oss;
    for (int n : nums) {
        oss << n << " ";
    }

    return oss.str();
}



int main(void){
    //Key constants
    const char IP_SERV[] = "127.0.0.1";        // local Server IP address
    const int PORT_NUM = 8081;          // working server port
    const short BUFF_SIZE = 1024;   // Maximum size of buffer for
                                    // exchange info between server and client
    // Key variables for all program
    int erStart;

    //IP in string format to numeric format for socket functions.
    // Data is in "ip_to_num"
    in_addr ip_to_num;
    erStart = Inet_pton(AF_INET , IP_SERV , &ip_to_num );
    // erStart = inet_ntoa(AF_INET, IP_SERV, &ip_to_num);
    if (erStart == 0) {
        cout << "Invalid IP address format" << endl;
        return 1;
    } else if (erStart == -1) {
        cout << "Error in IP translation" << endl;
        return 1;
    }
    
    if (erStart <= 0) {
        cout << "Error in IP translation to special numeric format " << endl;
        return 1;
    }

    // WinSock initialization

    WSADATA wsData;
    erStart = WSAStartup(MAKEWORD(2,2), &wsData);

    if(erStart != 0){
        cout << "Error WinSock version initializaion #";
        cout << WSAGetLastError();
        return 1;
    }
    else{
        cout << "WinSock is OK" << endl;
    }

    // Server socket initialization

    SOCKET ServSock = socket(AF_INET , SOCK_STREAM , 0);

    if(ServSock == INVALID_SOCKET){
        cout << "Error initialization socket # " << WSAGetLastError() << endl;
        closesocket(ServSock);
        WSACleanup();
        return 1;
    }
    else{
        cout << "Server socket initialization is OK" << endl;
    }

    // Server socket binding
    sockaddr_in servInfo;
    // Initializing servInfo structure
    ZeroMemory(&servInfo, sizeof(servInfo));

    servInfo.sin_family = AF_INET;
    servInfo.sin_addr = ip_to_num;
    servInfo.sin_port = htons(PORT_NUM);

    erStart = bind(ServSock ,(sockaddr*)&servInfo, sizeof(servInfo));

    if( erStart != 0){
        cout << "Error Socket binding to server info. Error #" <<
        WSAGetLastError() << endl;
        closesocket(ServSock);
        WSACleanup();

        return 1;
    }
    else{
        cout << "Binding socket to Server info is OK" << endl;
    }

    // Start listening for incoming connection
    erStart = listen(ServSock , SOMAXCONN );
    if (erStart != 0){
        cout << "Can't start to listen to. Error #" <<
        WSAGetLastError() << endl;
        closesocket(ServSock);
        WSACleanup();
        return 1;
    }
    else{
        cout << "Listening..." << endl;
    }

    // Accept client connection
    sockaddr_in clientInfo;
    ZeroMemory(&clientInfo , sizeof(clientInfo));

    int clientInfo_size = sizeof(clientInfo);

    SOCKET  ClientConn = accept(ServSock , (sockaddr*)&clientInfo, &clientInfo_size );

    if (ClientConn == INVALID_SOCKET){
        cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << endl;
        closesocket(ServSock);
        closesocket(ClientConn);
        WSACleanup();
        return 1;
    } 
    else {
        cout << "Connection to a client established successfully" << endl;
        char clientIP[22];

        Inet_ntop(AF_INET, &clientInfo.sin_addr, clientIP, INET_ADDRSTRLEN);
        cout << "Client connected with IP address " << inet_ntoa(clientInfo.sin_addr) << endl;

    }

    vector<char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);

    short packet_size = 0;

    while(true){
        packet_size = recv(ClientConn, servBuff.data(), servBuff.size(), 0);
        if (packet_size == SOCKET_ERROR){
            cout << "Can't receive data from client. Error # " << WSAGetLastError() << endl;
            break;
        }
        if (packet_size == 0) {
            cout << "Client disconnected." << endl;
            break;
        }
        string receivedData(servBuff.data(), packet_size);
        string response;

        // Перевіряємо, чи це ім'я чи масив чисел
        if (receivedData.find_first_of("0123456789") == string::npos) {
            // Це ім'я
            response = addSurname(receivedData);
        } 
        else{
            if(receivedData[0] == '1'){
                response = numbersMinMax(receivedData);
            }
            else{
                response = add_arithmetic_mean(receivedData);
            }
            
        }

        // Send the result back to the client
        packet_size = send(ClientConn, response.c_str(), response.size(), 0);
        if (packet_size == SOCKET_ERROR) {
            cout << "Can't send message to Client. Error # " << WSAGetLastError() << endl;
            break;
        }
}


    closesocket(ServSock);
    closesocket(ClientConn);
    WSACleanup();
    return 0;
}
