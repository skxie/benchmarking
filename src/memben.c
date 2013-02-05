#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <string.h>
/*
void *ranRead(void *n) {
	int * size = (int *)n;
	int m = 1024 * 1024 * 10;
	char * mem = (char *)malloc(m*sizeof(char));
	int i = 0;
	int idx =  rand()%(m - *size - 1);
	char a;
	int j = 0;
	for (j = 0; j < 10000; j++) {
		for (i = 0; i < *size; i++) {
			memcpy(&a, mem+idx+i, 1);
		}
	}
}
*/
void *ranAccess(void *n) {
	int * size = (int *)n;
	int m = 1024 * 1024 * 100;
	char * mem = (char *)malloc(m*sizeof(char));
	int i = 0;
	char a = '0';
	int j = 0;
	for (j = 0; j < 100; j++) {
		srand((int)time(0));
		int idx =  rand()%(m - *size - 1);
		memcpy(mem+idx, &a, *size);
	}
}
/*
void *seqRead(void *n) {
	int * size = (int *)n;
	int m = 1024 * 1024 * 10;
	char * mem = (char *)malloc(m*sizeof(char));
	int i = 0;
	char a;
	int j = 0;
	for (j = 0; j < 10000; j++) {
		for (i = 0; i < *size; i++) {
			memcpy(&a, mem+i, 1);
		}
	}
}
*/
void *seqAccess(void *n) {
	int * size = (int *)n;
	int m = 1024 * 1024 * 100;
	char * mem = (char *)malloc(m*sizeof(char));
	int i = 0;
	char a = '0';
	int j = 0;
	for (j = 0; j < 100; j++) {
		memcpy(mem+j*(*size), &a, *size);
	}
}

int main() {

	srand((int)time(0));
	int i = 0;
	for (i = 1; i <= 2; i++) {
		int j = 0;
		for (j = 0; j < 3; j++) {
			int n = pow(1024.0, (double) j);
			pthread_t pthreads[i];
			struct timeval start_t, end_t;
			int k = 0;

			gettimeofday(&start_t, NULL);
			for (k = 0; k < i; k++)
				pthread_create(&pthreads[k], NULL, seqAccess, &n);
			for (k = 0; k < i; k++)
				pthread_join(pthreads[k], NULL);
			gettimeofday(&end_t, NULL);
			double exec_t = 1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0;
			double throughput = (n / (1024.0 * 1024.0)) / (exec_t / 1000.0);
			printf("With %d threads, sequential access %10d bytes, the latency is %10f ms and the throughput is %10f MB/sec\n", i, n, exec_t/100, throughput*100);
		
			gettimeofday(&start_t, NULL);
			for (k = 0; k < i; k++)
				pthread_create(&pthreads[k], NULL, ranAccess, &n);
			for (k = 0; k < i; k++)
				pthread_join(pthreads[k], NULL);
			gettimeofday(&end_t, NULL);
			exec_t = 1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0;
			throughput = (n / (1024.0 * 1024.0)) / (exec_t / 1000.0);
			printf("With %d threads, random     access %10d bytes, the latency is %10f ms and the throughput is %10f MB/sec\n", i, n, exec_t/100, throughput*100);
		}
	}
}
