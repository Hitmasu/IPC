#include <Windows.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
	//Tamanho do Buffer
	const int TamanhoBuffer = 512;

	//Buffer para armazenar a informação recebida do servidor.
	char buffer[TamanhoBuffer];

	//Mensagem a ser enviada
	string mensagemEnviada = "";

	//Nome do pipe
	string pipeName = "\\\\.\\pipe\\MeuPipe";

	////Handler do pipe
	HANDLE hPipe;

	cout << "Tentando conectar-se ao servidor..." << endl;

	while (true) {
		//Tenta obter uma conexão com o named pipe
		hPipe = CreateFile(pipeName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

		//Se não tiver sucesso na conexão, espera 5 segundos para tentar se conectar novamente
		if (hPipe == INVALID_HANDLE_VALUE) {
			cout << "Tenta se conectar novamente em 5 segundos..." << endl;
			Sleep(5000);
		}
		else {
			cout << "Conectado!" << endl;
			break;
		}
	}

	cout << "Digite o texto a ser enviado: ";

	//Captura a entrada do usuário
	getline(cin, mensagemEnviada);

	while (mensagemEnviada != "sair") {
		DWORD tamanhoMensagemEnviada = mensagemEnviada.size();
		DWORD tamanhoMensagemRecebida;
		string mensagemRecebida;

		bool k = WriteFile(hPipe, mensagemEnviada.c_str(), tamanhoMensagemEnviada, &tamanhoMensagemEnviada, NULL);

		//Efetua a limpeza do buffer
		memset(buffer,0,sizeof(buffer));

		//Recebe a resposta do servidor
		ReadFile(hPipe, buffer, TamanhoBuffer * sizeof(char), &tamanhoMensagemRecebida, NULL);

		//Transforma a informação recebida do servidor em string
		mensagemRecebida = string(buffer);

		cout << "O servidor recebeu a mensagem em " << mensagemRecebida << endl;

		cout << "Digite o texto a ser enviado: ";
		getline(cin, mensagemEnviada);
	}

	return 0;
}