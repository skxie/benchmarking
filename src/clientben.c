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

#define NUM_LOOPS 100

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
	
	//recive and send messages multiple times
	for (i = 0; i < NUM_LOOPS; i++) {
		sendto(client_sock, buffer, tinfo->bufferSize, 0, (struct sockaddr *)&(tinfo->server_addr), sizeof(struct sockaddr));
		recvfrom(client_sock, buffer, tinfo->bufferSize, 0, (struct sockaddr *)&(tinfo->server_addr), &sin_size);
	}
}

void *tcpClient(void *arg) {

	struct thread_info * tinfo = (struct thread_info *)arg;
	char * buffer = (char *)malloc(sizeof(char)*(tinfo->bufferSize));
	memset(buffer, 0, sizeof(char)*(tinfo->bufferSize));
	
	int client_sock = socket(AF_INET, SOCK_STREAM, 0);
	connect(client_sock, (struct sockaddr *)&(tinfo->server_addr), sizeof(struct sockaddr));
	
	//send and recive messages multip times
	int i = 0;
	for (i = 0; i < NUM_LOOPS; i++) {
		send(client_sock, buffer, tinfo->bufferSize, 0);
		recv(client_sock, buffer, tinfo->bufferSize, 0);
	}
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
	gettimeofday(&start_t, NULL); //time start
	if (strcmp(argv[1], "TCP") == 0) {
		for (i = 0; i < numOfThreads; i++) {
			pthread_create(&threads[i], NULL, tcpClient, tinfo); //multiple tcp clients
		}
		for (i = 0; i < numOfThreads; i++) {
			pthread_join(threads[i], NULL);
		}
	} else {
		for (i = 0; i < numOfThreads; i++) {
			pthread_create(&threads[i], NULL, udpClient, tinfo); //multiple UDP clients
		}
		for (i = 0; i < numOfThreads; i++) {
			pthread_join(threads[i], NULL);
		}
	}
	gettimeofday(&end_t, NULL); //time end
	double exec_t = (1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0);
	double throughput  = (numOfThreads * 2.0 * NUM_LOOPS * 8.0 * bufferSize / (1024.0 * 1024.0)) / (exec_t / 1000.0);
	printf("With %d threads, the execution time is %10f ms and the throughput is %10f Mb/S\n", numOfThreads, exec_t/(numOfThreads * 2.0 * NUM_LOOPS * 8.0 * bufferSize),  throughput);
   
	return 0;
}
