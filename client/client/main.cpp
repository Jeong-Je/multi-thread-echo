//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "stdafx.h"
#include <winsock2.h>
#pragma comment(lib, "ws2_32")

int _tmain() {
	// 윈속 초기화
	WSADATA wsa = { 0 };
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		puts("※ ERROR: 윈속 초기화 실패!");
		return 0;
	}

	// 소켓 생성
	SOCKET hSocket = ::socket(AF_INET, SOCK_STREAM, 0); // IPv4 & TCP를 사용
	if (hSocket == INVALID_SOCKET) {
		puts("※ ERROR: 소켓 생성 실패!");
		return 0;
	}

	//2. 포트 바인딩 및 연결
	SOCKADDR_IN	svraddr = { 0 };
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(43210);
	svraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (::connect(hSocket, (SOCKADDR*)&svraddr, sizeof(svraddr)) == SOCKET_ERROR) {
		puts("※ ERROR: 서버 연결 실패!");
		return 0;
	}

	// 메시지 송수신
	char szBuffer[128] = { 0 };
	while (true) {
		// 문자열 입력
		gets_s(szBuffer);
		if (strcmp(szBuffer, "EXIT") == 0) break;

		// 문자열 전송
		::send(hSocket, szBuffer, strlen(szBuffer) + 1, 0);

		// 에코된 문자열 수신
		memset(szBuffer, 0, sizeof(szBuffer));
		::recv(hSocket, szBuffer, sizeof(szBuffer), 0);
		printf("From server: %s\n", szBuffer);
	}

	// 종료
	::shutdown(hSocket, SD_BOTH);
	::closesocket(hSocket);

	// 윈속 해제
	::WSACleanup();
	return 0;
}