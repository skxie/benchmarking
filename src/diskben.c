#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define FILE_NAME "file.txt"

void *ranRead(void *n) {
	int  *size  = (int *)n;
	FILE *pFile = fopen(FILE_NAME, "r");
	unsigned long m = 1024ul * 1024ul * 1024ul * 2ul;
	unsigned long pos = 0;
  	int i   = 0;
	char * buffer = (char *)malloc((*size)*sizeof(char));
	for (i = 0; i < 10; i++) {
		srand((int)time(0));
	 	pos = rand() % (m - *size);
		fseek(pFile, pos, SEEK_SET);
		fread(buffer, *size, 1, pFile);
	}
	fclose(pFile);
}

void *ranWrite(void *n) {
	int  *size  = (int *)n;
	FILE *pFile = fopen(FILE_NAME, "r+");
	fseek(pFile, 0, SEEK_SET);
	unsigned long m =  1024ul * 1024ul * 1024ul * 2ul;
	unsigned long pos = 0;
	int i   = 0;
	char * buffer = (char *)malloc((*size)*sizeof(char));
	for (i = 0; i < 10; i++) {
		srand((int)time(0));
		pos = rand() % (m - *size);
		fseek(pFile, pos, SEEK_SET);
		fwrite(buffer, *size, 1, pFile);
	}
	fclose(pFile);
}

void *seqRead(void *n) {
	int  *size  = (int *)n;
	FILE *pFile = fopen(FILE_NAME, "r");
	char *buffer = (char *)malloc((*size)*sizeof(char));
	int i = 0;
	for (i = 0; i < 10; i++) {
		fread(buffer, *size, 1, pFile);
	}
	fclose(pFile);
}

void *seqWrite(void *n) {
	int  *size  = (int *)n;
	char *buffer = (char *)malloc((*size)*sizeof(char));
	FILE *pFile = fopen(FILE_NAME, "r+");
	fseek(pFile, 0, SEEK_SET);
	int i = 0;
	for (i = 0; i < 10; i++) {
		fwrite(buffer, *size, 1, pFile);
	}
	fclose(pFile);
}

int main() {
	/*
	FILE *pFile = fopen(FILE_NAME, "w");
	unsigned  long k = 0;
	char a = 'a';
	unsigned long m = 1024ul * 1024ul * 1024ul * 2ul;
	for (k = 0; k < m; k++) {
		fputc(a, pFile);
	}
	fclose(pFile);
	*/

	srand((int)time(0));
	int i = 0;
	for (i = 1; i <=2; i++) {
		int j = 0;
		for (j = 0; j < 4; j++) {
			int n = pow(1024.0, (double) j);
			pthread_t pthreads[i];
			struct timeval start_t, end_t;
			int k = 0;

			gettimeofday(&start_t, NULL);
			for (k = 0; k < i; k++)
				pthread_create(&pthreads[k], NULL, seqWrite, &n);
			for (k = 0; k < i; k++)
				pthread_join(pthreads[k], NULL);
			gettimeofday(&end_t, NULL);
			double exec_t = (1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0)/10.0;
			double throughput = (n / (1024.0 * 1024.0)) / (exec_t / 1000.0);
			printf("With %d threads, sequential write %10d bytes, the latency is %10f ms and the throughput     is %10f MB/sec\n", i, n, exec_t, throughput);

			gettimeofday(&start_t, NULL);
			for (k = 0; k < i; k++)
				pthread_create(&pthreads[k], NULL, seqRead, &n);
			for (k = 0; k < i; k++)
				pthread_join(pthreads[k], NULL);
			gettimeofday(&end_t, NULL);
			exec_t = (1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0)/10.0;
			throughput = (n / (1024.0 * 1024.0)) / (exec_t / 1000.0);
			printf("With %d threads, sequential  read %10d bytes, the latency is %10f ms and the throughput     is %10f MB/sec\n", i, n, exec_t, throughput);

			gettimeofday(&start_t, NULL);
			for (k = 0; k < i; k++)
				pthread_create(&pthreads[k], NULL, ranWrite, &n);
			for (k = 0; k < i; k++)
				pthread_join(pthreads[k], NULL);
			gettimeofday(&end_t, NULL);
			exec_t = (1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0)/10.0;
			throughput = (n / (1024.0 * 1024.0)) / (exec_t / 1000.0);
			printf("With %d threads, randomly   write %10d bytes, the latency is %10f ms and the throughput     is %10f MB/sec\n", i, n, exec_t, throughput);

			gettimeofday(&start_t, NULL);
			for (k = 0; k < i; k++)
				pthread_create(&pthreads[k], NULL, ranRead, &n);
			for (k = 0; k < i; k++)
				pthread_join(pthreads[k], NULL);
			gettimeofday(&end_t, NULL);
			exec_t = (1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0)/10.0;
			throughput = (n / (1024.0 * 1024.0)) / (exec_t / 1000.0);
			printf("With %d threads, randomly    read %10d bytes, the latency is %10f ms and the throughput     is %10f MB/sec\n", i, n, exec_t, throughput);
		}
	}
	//remove(FILE_NAME);
}
