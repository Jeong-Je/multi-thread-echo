//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "stdafx.h"
#include <winsock2.h>
#pragma comment(lib, "ws2_32")

int _tmain() {
	// ���� �ʱ�ȭ
	WSADATA wsa = { 0 };
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		puts("�� ERROR: ���� �ʱ�ȭ ����!");
		return 0;
	}

	// ���� ����
	SOCKET hSocket = ::socket(AF_INET, SOCK_STREAM, 0); // IPv4 & TCP�� ���
	if (hSocket == INVALID_SOCKET) {
		puts("�� ERROR: ���� ���� ����!");
		return 0;
	}

	//2. ��Ʈ ���ε� �� ����
	SOCKADDR_IN	svraddr = { 0 };
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(43210);
	svraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (::connect(hSocket, (SOCKADDR*)&svraddr, sizeof(svraddr)) == SOCKET_ERROR) {
		puts("�� ERROR: ���� ���� ����!");
		return 0;
	}

	// �޽��� �ۼ���
	char szBuffer[128] = { 0 };
	while (true) {
		// ���ڿ� �Է�
		gets_s(szBuffer);
		if (strcmp(szBuffer, "EXIT") == 0) break;

		// ���ڿ� ����
		::send(hSocket, szBuffer, strlen(szBuffer) + 1, 0);

		// ���ڵ� ���ڿ� ����
		memset(szBuffer, 0, sizeof(szBuffer));
		::recv(hSocket, szBuffer, sizeof(szBuffer), 0);
		printf("From server: %s\n", szBuffer);
	}

	// ����
	::shutdown(hSocket, SD_BOTH);
	::closesocket(hSocket);

	// ���� ����
	::WSACleanup();
	return 0;
}