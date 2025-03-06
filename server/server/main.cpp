#include "stdafx.h"
#include <winsock2.h>
#pragma comment(lib, "ws2_32")

// Ŭ���̾�Ʈ ó���� ���� ������ �Լ�
DWORD WINAPI ThreadFunction(LPVOID pParam);

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

	// ��Ʈ ���ε�
	SOCKADDR_IN svraddr = { 0 };
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(43210);
	svraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	if (::bind(hSocket, (SOCKADDR*)&svraddr, sizeof(svraddr)) == SOCKET_ERROR) {
		puts("�� ERROR: ��Ʈ ���ε� ����!");
		return 0;
	}

	// ����
	if (::listen(hSocket, SOMAXCONN) == SOCKET_ERROR) {
		puts("�� ERROR: ���� ����!");
		return 0;
	}
	puts("���� ������....");

	SOCKADDR_IN clientaddr = { 0 };
	int nAddrLen = sizeof(clientaddr);
	SOCKET hClient = 0;
	DWORD dwThreadID = 0;
	HANDLE hThread;
	// ���� ���� �� ���ο� ���� ����
	while ((hClient = ::accept(hSocket, (SOCKADDR*)&clientaddr, &nAddrLen)) != INVALID_SOCKET) {
		hThread = ::CreateThread(NULL, 0, ThreadFunction, (LPVOID)hClient, 0, &dwThreadID);

		::CloseHandle(hThread);
	}

	// ���� ���� �ݱ�
	::closesocket(hSocket);

	// ���� ����
	::WSACleanup();
	return 0;
}


DWORD WINAPI ThreadFunction(LPVOID pParam) {
	char szBuffer[128] = { 0 };
	int nReceive = 0;
	SOCKET hClient = (SOCKET)pParam;

	puts("���ο� Ŭ���̾�Ʈ�� ����Ǿ����ϴ�.");

	// Ŭ���̾�Ʈ�κ��� ���ڿ� ����
	while ((nReceive = ::recv(hClient, szBuffer, sizeof(szBuffer), 0) > 0)) {
		// ������ ���ڿ� �״�� ����
		::send(hClient, szBuffer, sizeof(szBuffer), 0);
		puts(szBuffer);
		memset(szBuffer, 0, sizeof(szBuffer));
	}

	puts("Ŭ���̾�Ʈ ���� ����");
	::closesocket(hClient);
	return 0;
}