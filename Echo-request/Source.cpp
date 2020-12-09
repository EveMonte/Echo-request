#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <stdio.h>
#include "Ws2tcpip.h"
#include <iostream>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main() {
	setlocale(LC_ALL, "rus");
	HANDLE hIcmpFile = IcmpCreateFile();
	if (hIcmpFile == INVALID_HANDLE_VALUE)
	{
		printf("\tНевозможно открыть дескриптор.\n");
		printf("\tОшибка IxmpCreatefile: %ld\n" , WSAGetLastError());
		return -1;
	}
//#define ADDR "81.19.70.1"  // IP-address
	//cout << "Введите IP- адрес\n";
	unsigned long ipaddr = inet_addr("169.254.25.32"); //преобразование IP-адреса
	//InetPton(AF_INET, (PCWSTR)("172.16.192.12"), &ipaddr);
	char SendData[32] = "Data Buffer";
	DWORD ReplySize = 0;
	LPVOID ReplyBuffer = NULL;
	ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
	ReplyBuffer = (VOID*)malloc(ReplySize);
	//ReplySize = 0;
	if (ReplyBuffer == NULL) {
		printf("\tНевозможно выделить память\n");
		return -1;
	}
	/*DWORD IcmpSendEcho(
		__in HANDLE IcmpHandle,
		__in IPAddr DestinationAddress,
		__in LPVOID RequestData,
		__in WORD RequestSize,
		__in PIP_OPTION_INFORMATION RequestOptions,
		__in LPVOID ReplyBuffer,
		__in DWORD ReplySize,
		__in DWORD Timeout
	);*/
	DWORD dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData), NULL, ReplyBuffer, ReplySize, 1000);


	if (dwRetVal != 0) {
		PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
		struct in_addr ReplyAddr;
		ReplyAddr.S_un.S_addr = pEchoReply->Address;
		printf("\tПосылка icmp сообщения на %s\n", "169.254.25.32");

		if (dwRetVal > 1) {
			printf("\tПолучен %ldicmp ответ\n", dwRetVal);
			printf("\tИнформация:\n");
		}
		else {
			printf("\tПолучен %ld icmp ответ\n", dwRetVal);
			printf("\tИнформация:\n");
		}
		printf("\tполучено от %s\n", inet_ntoa(ReplyAddr));
		printf("\tСтатус = %ld\n", pEchoReply->Status);
		printf("\tВремя отклика = %ldмиллисекунд \n", pEchoReply->RoundTripTime);
	}
	else {
		printf("\tIcmpSendEcho ошибка: %ld\n", WSAGetLastError());
		return -1;
	}
	typedef struct icmp_echo_reply {
		IPAddr Address;
		ULONG Status;
		ULONG RoundTripTime;
		USHORT DataSize;
		USHORT Reserved;
		PVOID Data;
		struct ip_option_information Options;
	}; ICMP_ECHO_REPLY* PICMP_ECHOREPLY;
	BOOL bRetVal = IcmpCloseHandle(hIcmpFile);
	if (bRetVal)
		printf("\tHandle was closed\n");
	else
		printf("IcmpCloseHandle failed with error: %ld\n", WSAGetLastError());
}