#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct thread_info {
	int sock;
	int bufferSize;
};

struct udp_thread_info {
	int sock;
	int bufferSize;
	struct sockaddr_in client_addr;
};

void *udp_send_data(void *t) {
	struct udp_thread_info *tinfo = (struct udp_thread_info *)t;
	char * buff = (char *)malloc(sizeof(char)*tinfo->bufferSize);
	memset(buff, 0, sizeof(char)*tinfo->bufferSize);
	sendto(tinfo->sock, buff, tinfo->bufferSize, 0, (struct sockaddr *)&(tinfo->client_addr), sizeof(struct sockaddr));
}

void udpServer(int bufferSize, int numOfThreads) {
	int server_sock;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	char * buff = (char *)malloc(sizeof(char)*bufferSize);
	memset(buff, 0, sizeof(char)*bufferSize);
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(5501);

	server_sock = socket(AF_INET, SOCK_DGRAM, 0);
	bind(server_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
	int sin_size = sizeof(struct sockaddr_in);
	int len = 0;
	pthread_t threads[numOfThreads];
	int i = 0;
	while (1) {
		recvfrom(server_sock, buff, bufferSize, 0, (struct sockaddr *)&client_addr, &sin_size);
		struct udp_thread_info * t = (struct udp_thread_info *)malloc(sizeof(struct udp_thread_info));
		t->sock = server_sock;
		t->bufferSize = bufferSize;
		t->client_addr = client_addr;
		pthread_create(&threads[i], NULL, udp_send_data, t);
		i++;
	}
	close(server_sock);
}

void *tcp_recv_data(void *t) {
	struct thread_info *tinfo = (struct thread_info *)t;
	char * buff = (char *)malloc(sizeof(char)*tinfo->bufferSize);
	memset(buff, 0, sizeof(char)*tinfo->bufferSize);
	recv(tinfo->sock, buff, tinfo->bufferSize, 0);
	send(tinfo->sock, buff, tinfo->bufferSize, 0);
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
	if (argc != 5) {
		printf("It should contain five arguments, including TCP/UDP, buffer_size and num_of_threads\n");
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
