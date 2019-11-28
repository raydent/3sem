#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
char** names;
void main(){
	names = (char**) calloc (100, sizeof(char*));
	for(int i = 0; i < 100; i++)
	{
		names[i] = (char*) calloc (100, sizeof(char));	
	}
	int sockfd, newsockfd; /* Дескрипторы для слушающего и присоединенного сокетов */
	int clilen; /* Длина адреса клиента */
	int n; /* Количество принятых символов */
	char line[1000]; /* Буфер для приема информации */
	struct sockaddr_in servaddr, cliaddr; /* Структуры для размещения полных адресов сервера и клиента */
	/* Создаем TCP-сокет */
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror(NULL);
		exit(1);
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family= AF_INET;
	servaddr.sin_port= htons(51000);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
/* Настраиваем адрес сокета */
	if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
	{
		perror(NULL);
		close(sockfd);
		exit(1);
	}/* Переводим созданный сокет в пассивное (слушающее)состояние. Глубину очереди для установленных соединений описываем значением 5 */
	if(listen(sockfd, 5) < 0)
	{
		perror(NULL);
		close(sockfd);
		exit(1);
	}/* Основной цикл сервера */
	while(1)
	{
	/* В переменную clilen заносим максимальнуюдлину ожидаемого адреса клиента */
		clilen = sizeof(cliaddr);
	/* Ожидаем полностью установленного соединенияна слушающем сокете. При нормальном завершенииу нас в структуре cliaddr будет лежать полныйадрес клиента, установившего соединение, а впеременной clilen – его фактическая длина. Вызовже вернет дескриптор присоединенного сокета,через который будет происходить общение с клиентом.Заметим, что информация о клиенте унас в дальнейшем никак не используется, поэтомувместо второго и третьего параметров можно былопоставить значения NULL. */
		if((newsockfd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen)) < 0)
		{
			perror(NULL);
			close(sockfd);
			exit(1);
		}
	/* В цикле принимаем информацию от клиента дотех пор, пока не произойдет ошибки (вызов read()вернет отрицательное значение) или клиент не закроет соединение (вызов read() вернет значение0). Максимальную длину одной порции данных отклиента ограничим 999 символами. В операцияхчтения и записи пользуемся дескриптором присоединенного сокета, т. е. значением, котороевернул вызов accept().*/
		while((n = read(newsockfd, line, 999)) > 0)
		{
		/* Принятые данные отправляем обратно */
			if((n = write(newsockfd, line, strlen(line)+1)) < 0)
			{
				perror(NULL);
				close(sockfd);
				close(newsockfd);
				exit(1);
			}
		}
		/* Если при чтении возникла ошибка – завершаем работу */
		if(n < 0)
		{
			perror(NULL);
			close(sockfd);
			close(newsockfd);
			exit(1);
		}/* Закрываем дескриптор присоединенного сокетаи уходим ожидать нового соединения */
		close(newsockfd);
		}
}
