#include <iostream>
#include <string>
#include <WinSock2.h>
using namespace std;

void ShowErrorMessage(string message) {
	cout << "���� �߻�: " << message << endl;
	system("pause");
	exit(1);
}

int main() {
	WSADATA wsaData; //����ü��������(WInSock���̺귯�������Ե�����)
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256]; //���ڿ����ۻ���
	
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //Winsock�� �ʱ�ȭ
		ShowErrorMessage("WSAStartup()");
	serverSocket = socket(PF_INET, SOCK_STREAM, 0); //TCP ���� ���� (IPv4���,TCP��� ���)

	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress)); //serverAddress �ʱ�ȭ
	serverAddress.sin_family = AF_INET; //��� ü��?
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); //4����Ʈ ������ ��Ʈ��ũ ����Ʈ ��������
	serverAddress.sin_port = htons(serverPort); //2����Ʈ ������ ��Ʈ��ũ ����Ʈ ��������
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) //������������ ���ε�ó��
		ShowErrorMessage("bind()");
	cout << "[���� ����] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) //listen���� ����
		ShowErrorMessage("listen()");
	cout << "[���� ����] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress); //Ŭ���̾�Ʈ ������ clientAddress�� ���
	cout << "[���� ����] accet()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()\n");
	while (1) { //Ŭ���̾�Ʈ�� �޽����� �޾Ƽ� �״�� �ٽ� ���� ���ѹݺ�
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL; //���������ڸ�NULL�� �ٲ�(����ġ���ѹ��ڰ����������ڸ�)
		cout << "[Ŭ���̾�Ʈ �޽���]: " << received << '\n';
		cout << "[�޽��� ����]: " << received << '\n';
		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1/*���������鹮������*/, 0);
			cout << "[���� ����]\n";
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0); //Ŭ���̾�Ʈ�� ����
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;
}