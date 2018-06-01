using System;
using System.IO.Pipes;
using System.Text;

namespace Server
{
	class Program
	{
		static void Main(string[] args)
		{
			//Nome do Named Pipe
			const string nomeDoPipe = "MeuPipe";

			//Cria o Named Pipe
			using (NamedPipeServerStream server = new NamedPipeServerStream(nomeDoPipe))
			{
				Console.WriteLine("Esperando conexão com o cliente...");

				//Espera por uma conexão
				server.WaitForConnection();

				Console.WriteLine("Cliente conectado");

				//Loop para que sempre que ele receber um pacote, ele entre em "modo de escuta" novamente
				while (true)
				{
					byte[] buffer = new byte[512];

					//Ao chamar o Read, o servidor fica na espera até ter uma informação disponível e seguidamente faz a escrita da mesma no buffer
					int quantidadeDeBytes = server.Read(buffer, 0, buffer.Length);

					//Se a quantidade recebida for 0, o cliente se desconectou e o servidor pode ser encerrado.
					if (quantidadeDeBytes == 0)
					{
						break;
					}

					//Faz a conversão dos bytes recebidos em string
					string textoRecebido = Encoding.UTF8.GetString(buffer, 0, quantidadeDeBytes);

					//Transforma em maiúscula
					string textoMaiusculo = textoRecebido.ToUpper();

					//Mostra a informação recebida em maiúscula
					Console.WriteLine($"Informação recebida: {textoMaiusculo}");

					//Pega a data atual em string e armazena no array de bytes
					byte[] dataAtual = Encoding.UTF8.GetBytes(DateTime.Now.ToString("dd/MM/yyyy HH:mm"));

					//Envia os bytes para o cliente
					server.Write(dataAtual, 0, dataAtual.Length);
				}
			}
		}
	}
}