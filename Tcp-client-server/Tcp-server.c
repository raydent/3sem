#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>


int* newsockets;
int clientnum;

struct mysocket
{
	int sockfd;
	int newsockfd;
} mysockfd;

void* Process(void* my_socket) 
{
	struct mysocket mysockfd = *(struct mysocket*)my_socket;
	char line[1000];
	int n;
	while ((n = read(mysockfd.newsockfd, line, 999)) > 0)
	{
		for (int i = 0; i < clientnum; ++i) 
		{
			if (newsockets[i] != mysockfd.newsockfd)
			{
				if ((n = write(newsockets[i], line, strlen(line) + 1)) < 0)
				{
					perror(NULL);
					close(mysockfd.sockfd);
					close(mysockfd.newsockfd);
					exit(1);
				}
			}
		}
	}
}

int main()
{
	clientnum = 0;
	newsockets = malloc(1000 * sizeof(int));
	int clilen;
	int n; 
	char line[1000];
	struct sockaddr_in servaddr, cliaddr; 
	if ((mysockfd.sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror(NULL);
		exit(1);
	}
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(51000);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(mysockfd.sockfd, (struct sockaddr*) & servaddr,
		sizeof(servaddr)) < 0) {
		perror(NULL);
		close(mysockfd.sockfd);
		exit(1);
	}
	if (listen(mysockfd.sockfd, 5) < 0) {
		perror(NULL);
		close(mysockfd.sockfd);
		exit(1);
	}
	//int i = 0;
	while (1) 
	{
		pthread_t thid;
		clilen = sizeof(cliaddr);
		if ((mysockfd.newsockfd = accept(mysockfd.sockfd, (struct sockaddr*) & cliaddr, &clilen)) < 0) {
			perror(NULL);
			close(mysockfd.sockfd);
			exit(1);
		}
		newsockets[clientnum] = mysockfd.newsockfd;
		clientnum++;
		//newsockets[i] = mysockfd.newsockfd;
		pthread_create(&thid, (pthread_attr_t*)NULL, Process, &mysockfd);
		//i++;
		//close(mysockfd.newsockfd);
	}
}
