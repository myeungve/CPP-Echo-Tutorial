#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <WinSock2.h>
using namespace std;

void ShowErrorMessage(string message) {
	cout << "오류 발생: " << message << endl;
	system("pause");
	exit(1);
}

int main() {
	cout << "[Wellcome]\n";
	WSADATA wsaData; //구조체변수생성(WInSock라이브러리에포함되있음)
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;

	int serverPort = 9876; //서버주소 동일해야함
	char received[256]; //문자열버퍼생성
	string sent;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //Winsock을 초기화
		ShowErrorMessage("WSAStartup()");
	clientSocket = socket(PF_INET, SOCK_STREAM, 0); //TCP 소켓 생성 (IPv4방식,TCP방식 사용)

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress)); //serverAddress 초기화
	serverAddress.sin_family = AF_INET; //어떠한 체계?
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); //문자열 IP(자기자신)를 네트워크 바이트 형식으로
	serverAddress.sin_port = htons(serverPort); //2바이트 정수를 네트워크 바이트 형식으로
	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) //클라이언트는 커넥트함수로 서버에 연결요청보냄
		ShowErrorMessage("connect()");
	cout << "[현재 상태] connect()\n";

	while (1) { //반복적으로 서버로 메시지를 전송하고 에코 메시지를 전달받음
		cout << "[메시지 전송]: ";
		getline(cin, sent);
		if (sent == "") continue;
		send(clientSocket, sent.c_str(), sent.length(), 0);
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = '\0';
		if (strcmp(received, "[exit]") == 0) {
			cout << "[서버 종료]\n";
			break;
		}
		cout << "[서버 메시지]: " << received << '\n';
	}

	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}