#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_LOOPS 100

struct thread_info {
	int sock;
	int bufferSize;
};

void *udp_send_data(void *t) {
	struct thread_info *tinfo = (struct thread_info *)t;
	struct sockaddr_in client_addr;
	char * buff = (char *)malloc(sizeof(char)*tinfo->bufferSize);
	memset(buff, 0, sizeof(char)*tinfo->bufferSize);
	int sin_size = sizeof(struct sockaddr_in);

	int i = 0;
	for (i = 0 ; i < NUM_LOOPS; i++) {
		//printf("0 %d\n", i);
		recvfrom(tinfo->sock, buff, tinfo->bufferSize, 0, (struct sockaddr *)&client_addr, &sin_size);
		//printf("1 %d\n", i);
		sendto(  tinfo->sock, buff, tinfo->bufferSize, 0, (struct sockaddr *)&client_addr, sizeof(struct sockaddr));
		//printf("2 %d\n", i);
	}
}

void udpServer(int bufferSize, int numOfThreads) {
	int server_sock;
	struct sockaddr_in server_addr;
	char * buff = (char *)malloc(sizeof(char)*bufferSize);
	memset(buff, 0, sizeof(char)*bufferSize);
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(5501);

	server_sock = socket(AF_INET, SOCK_DGRAM, 0);
	bind(server_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
	int len = 0;
	pthread_t threads[numOfThreads];
	int i = 0;
	struct thread_info * t = (struct thread_info *)malloc(sizeof(struct thread_info));
	t->sock = server_sock;
	t->bufferSize = bufferSize;
	for (i = 0; i < numOfThreads; i++) {
		pthread_create(&threads[i], NULL, udp_send_data, t);
	}
	for (i = 0; i < numOfThreads; i++) {
		pthread_join(threads[i], NULL);
	}
	//close(server_sock);
}

void *tcp_recv_data(void *t) {
	struct thread_info *tinfo = (struct thread_info *)t;
	char * buff = (char *)malloc(sizeof(char)*tinfo->bufferSize);
	memset(buff, 0, sizeof(char)*tinfo->bufferSize);
	int i = 0;
	for (i = 0; i < NUM_LOOPS; i++) {
		recv(tinfo->sock, buff, tinfo->bufferSize, 0);
		send(tinfo->sock, buff, tinfo->bufferSize, 0);
	}
	free(buff);
}

void tcpServer(int bufferSize, int numOfThreads) {
	int server_sock;
	int client_sock;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(5501);

	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	bind(server_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
	listen(server_sock, 10);
	int sin_size = sizeof(struct sockaddr_in);
	pthread_t threads[numOfThreads];
	int i = 0;
	while (1) {
		client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &sin_size);
		struct thread_info * t = (struct thread_info *)malloc(sizeof(struct thread_info));
		t->sock = client_sock;
		t->bufferSize = bufferSize;
		pthread_create(&threads[i], NULL, tcp_recv_data, t);
		i++;
	}
	close(server_sock);
	close(client_sock);
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("It should contain four arguments, including TCP/UDP, buffer_size and num_of_threads\n");
		return 1;
	}

	int bufferSize   = atoi(argv[2]);
	int numOfThreads = atoi(argv[3]);
	if (strcmp(argv[1], "TCP") == 0)
		tcpServer(bufferSize, numOfThreads);
	else
		udpServer(bufferSize, numOfThreads);
	return 0;
}
