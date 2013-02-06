#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

#define NUM_LOOPS 1000

struct thread_info {
	int bufferSize;
	struct sockaddr_in server_addr;
};

void *udpClient(void *arg) {
	
	struct thread_info * tinfo = (struct thread_info *)arg;
	char * buffer = (char *)malloc(sizeof(char)*(tinfo->bufferSize));
	memset(buffer, 0, sizeof(char)*(tinfo->bufferSize));
	int sin_size = sizeof(struct sockaddr_in);

	int client_sock = socket(AF_INET, SOCK_DGRAM, 0);
	int i = 0;
	for (i = 0; i < NUM_LOOPS; i++) {
		sendto(client_sock, buffer, tinfo->bufferSize, 0, (struct sockaddr *)&(tinfo->server_addr), sizeof(struct sockaddr));
		recvfrom(client_sock, buffer, tinfo->bufferSize, 0, (struct sockaddr *)&(tinfo->server_addr), &sin_size);
	}
	close(client_sock);
}

void *tcpClient(void *arg) {

	struct thread_info * tinfo = (struct thread_info *)arg;
	char * buffer = (char *)malloc(sizeof(char)*(tinfo->bufferSize));
	memset(buffer, 0, sizeof(char)*(tinfo->bufferSize));

	int client_sock = socket(AF_INET, SOCK_STREAM, 0);
	connect(client_sock, (struct sockaddr *)&(tinfo->server_addr), sizeof(struct sockaddr));
	int i = 0;
	for (i = 0; i < NUM_LOOPS; i++) {
		send(client_sock, buffer, tinfo->bufferSize, 0);
		recv(client_sock, buffer, tinfo->bufferSize, 0);
	}
	close(client_sock);
}

int main(int argc, char *argv[]) {

	if (argc != 5) {
		printf("It should contain five arguments, including TCP/UDP, server ip address, buffer size and number of threads.\n");
		return 1;
	}
	char * server_ip = argv[2];
	int bufferSize = atoi(argv[3]);
	int numOfThreads = atoi(argv[4]);

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(server_ip);
	server_addr.sin_port = htons(5501);

	struct thread_info * tinfo = (struct thread_info *)malloc(sizeof(struct thread_info));
	tinfo->server_addr = server_addr;
	tinfo->bufferSize  = bufferSize;
	pthread_t threads[numOfThreads];
	
	int i = 0;
	struct timeval start_t, end_t;
	gettimeofday(&start_t, NULL);
	if (strcmp(argv[1], "tcp") == 0) {
		for (i = 0; i < numOfThreads; i++) {
			pthread_create(&threads[i], NULL, tcpClient, tinfo);
		}
		for (i = 0; i < numOfThreads; i++) {
			pthread_join(threads[i], NULL);
		}
	} else {
		for (i = 0; i < numOfThreads; i++) {
			pthread_create(&threads[i], NULL, udpClient, tinfo);
		}
		for (i = 0; i < numOfThreads; i++) {
			pthread_join(threads[i], NULL);
		}
	}
	gettimeofday(&end_t, NULL);
	double exec_t = (1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0);
	double throughput  = (numOfThreads * NUM_LOOPS * bufferSize / (1024 * 1024)) / (exec_t / 1000.0);
	printf("With %d threads, the execution time is %10f ms and the throughput is %10f\n", numOfThreads, exec_t, throughput);
   
	return 0;
}
