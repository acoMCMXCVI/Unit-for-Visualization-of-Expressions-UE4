// Fill out your copyright notice in the Description page of Project Settings.

#include "TunnelledInputFunLib.h"


#define _APISET_RTLSUPPORT_VER 0
#define _APISET_INTERLOCKED_VER 0
#define _WIN32_WINNT_WINTHRESHOLD 0
#define _APISET_SECURITYBASE_VER 0
#define NTDDI_WIN7SP1 0
#include <winsock2.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define IP_ADDR "127.0.0.1"
#define IP_PORT 27000


static SOCKET ConnectSocket = INVALID_SOCKET;

#include "TunnelData.h" 

static TunnelData tunnelData;


void UTunnelledInputFunLib::TestPrintSomething(FString nesto)
{
	UE_LOG(LogTemp, Display, TEXT("Nesto nesto %s"), *nesto);
}


void UTunnelledInputFunLib::TestReturnVector(FVector & myVec_out)
{
	myVec_out = FVector(0.0f, 0.0f, 3.0f);
}


void UTunnelledInputFunLib::ConnectToTunnel(bool& connected)
{
	connected = false;

	int iResult;

	// Doint initialization.
	UE_LOG(LogTemp, Display, TEXT("Opening socket at %s:%d..."), *FString(IP_ADDR), IP_PORT);

	// Initialize Winsock

	WSADATA wsaData;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		UE_LOG(LogTemp, Error, TEXT("WSAStartup failed with error: %d"), iResult);
		return;
	}

	// Create a SOCKET for connecting to server
	ConnectSocket = socket(AF_UNSPEC, SOCK_STREAM, IPPROTO_TCP);
	if (ConnectSocket == INVALID_SOCKET) {
		UE_LOG(LogTemp, Error, TEXT("socket failed with error: %ld\n"), WSAGetLastError());
		WSACleanup();
		return;
	}

	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(IP_ADDR);
	clientService.sin_port = htons(IP_PORT);

	// Connect to server.
	iResult = connect(ConnectSocket, (SOCKADDR *)& clientService, sizeof(clientService));
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		UE_LOG(LogTemp, Error, TEXT("connect() failed: %ld"), WSAGetLastError());
		ConnectSocket = INVALID_SOCKET;
		return;
	}

	if (ConnectSocket == INVALID_SOCKET) {
		UE_LOG(LogTemp, Error, TEXT("Unable to connect to server!"));
		WSACleanup();
		return;
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Connection to server successful!"));
	}

	connected = true;

}


void UTunnelledInputFunLib::GetTunnelledInput(TArray<float>& tracks)
{
	//UE_LOG(LogTemp, Display, TEXT("GetTunnelledInput"));

	u_long bytes_available = -1;
	int num_of_packets_in_buffer = 0;
	do {

		int iResult = recv(ConnectSocket, (char*)&tunnelData, sizeof(tunnelData), 0);
		if (iResult < 0) {
			UE_LOG(LogTemp, Error, TEXT("recv failed with error: %d\n"), WSAGetLastError());
		}

		ioctlsocket(ConnectSocket, FIONREAD, &bytes_available);
		//UE_LOG(LogTemp, Display, TEXT("bytes_available = %ld"), bytes_available);

		num_of_packets_in_buffer++;
	} while (bytes_available != 0);

	if (num_of_packets_in_buffer > 1) {
		UE_LOG(LogTemp, Error, TEXT("Number of packets = %d"), num_of_packets_in_buffer);
	}

	tracks.Add(tunnelData.r_eyebrow_out);
	tracks.Add(tunnelData.r_eyebrow_in);
	tracks.Add(tunnelData.l_eyebrow_in);
	tracks.Add(tunnelData.l_eyebrow_out);

	//text = FString(UTF8_TO_TCHAR(reciveBuf));
	//eyebrow_point = FVector(0.0f, 0.0f, tunnelData.r_eyebrow_move);

}