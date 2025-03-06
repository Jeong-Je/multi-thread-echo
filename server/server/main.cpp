#include "stdafx.h"
#include <winsock2.h>
#pragma comment(lib, "ws2_32")

// 클라이언트 처리를 위한 스레드 함수
DWORD WINAPI ThreadFunction(LPVOID pParam);

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

	// 포트 바인딩
	SOCKADDR_IN svraddr = { 0 };
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(43210);
	svraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	if (::bind(hSocket, (SOCKADDR*)&svraddr, sizeof(svraddr)) == SOCKET_ERROR) {
		puts("※ ERROR: 포트 바인딩 실패!");
		return 0;
	}

	// 리슨
	if (::listen(hSocket, SOMAXCONN) == SOCKET_ERROR) {
		puts("※ ERROR: 리슨 실패!");
		return 0;
	}
	puts("서버 리슨중....");

	SOCKADDR_IN clientaddr = { 0 };
	int nAddrLen = sizeof(clientaddr);
	SOCKET hClient = 0;
	DWORD dwThreadID = 0;
	HANDLE hThread;
	// 연결 수락 및 새로운 소켓 생성
	while ((hClient = ::accept(hSocket, (SOCKADDR*)&clientaddr, &nAddrLen)) != INVALID_SOCKET) {
		hThread = ::CreateThread(NULL, 0, ThreadFunction, (LPVOID)hClient, 0, &dwThreadID);

		::CloseHandle(hThread);
	}

	// 리슨 소켓 닫기
	::closesocket(hSocket);

	// 윈속 해제
	::WSACleanup();
	return 0;
}


DWORD WINAPI ThreadFunction(LPVOID pParam) {
	char szBuffer[128] = { 0 };
	int nReceive = 0;
	SOCKET hClient = (SOCKET)pParam;

	puts("새로운 클라이언트와 연결되었습니다.");

	// 클라이언트로부터 문자열 수신
	while ((nReceive = ::recv(hClient, szBuffer, sizeof(szBuffer), 0) > 0)) {
		// 수신한 문자열 그대로 전송
		::send(hClient, szBuffer, sizeof(szBuffer), 0);
		puts(szBuffer);
		memset(szBuffer, 0, sizeof(szBuffer));
	}

	puts("클라이언트 접속 해제");
	::closesocket(hClient);
	return 0;
}