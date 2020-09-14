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
	WSADATA wsaData; //구조체변수생성(WInSock라이브러리에포함되있음)
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256]; //문자열버퍼생성
	
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //Winsock을 초기화
		ShowErrorMessage("WSAStartup()");
	serverSocket = socket(PF_INET, SOCK_STREAM, 0); //TCP 소켓 생성 (IPv4방식,TCP방식 사용)

	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress)); //serverAddress 초기화
	serverAddress.sin_family = AF_INET; //어떠한 체계?
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); //4바이트 정수를 네트워크 바이트 형식으로
	serverAddress.sin_port = htons(serverPort); //2바이트 정수를 네트워크 바이트 형식으로
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) //서버소켓으로 바인딩처리
		ShowErrorMessage("bind()");
	cout << "[현재 상태] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) //listen과정 수행
		ShowErrorMessage("listen()");
	cout << "[현재 상태] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress); //클라이언트 정보가 clientAddress에 담김
	cout << "[현재 상태] accet()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()\n");
	while (1) { //클라이언트의 메시지를 받아서 그대로 다시 전달 무한반복
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL; //마지막문자를NULL로 바꿈(예기치못한문자가들어왔을때자름)
		cout << "[클라이언트 메시지]: " << received << '\n';
		cout << "[메시지 전송]: " << received << '\n';
		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1/*마지막공백문자제거*/, 0);
			cout << "[서버 종료]\n";
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0); //클라이언트에 전달
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;
}