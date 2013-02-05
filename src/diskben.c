#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define FILE_NAME "file.txt"
#define FILE_SIZE 1024*1024*1024;

void *ranRead(void *n) {
	int  *size  = (int *)n;
	FILE *pFile = fopen(FILE_NAME, "r");
	int pos = 0;
	int i   = 0;
	for (i = 0; i < *size; i++) {
		pos = rand() % FILE_SIZE;
		fseek(pFile, pos, SEEK_SET);
		fgetc(pFile);
	}
	fclose(pFile);
}

void *ranWrite(void *n) {
	int  *size  = (int *)n;
	FILE *pFile = fopen(FILE_NAME, "r+");
	int pos = 0;
	int i   = 0;
	char b  = 'b';
	for (i = 0; i < *size; i++) {
		pos = rand() % FILE_SIZE;
		fseek(pFile, pos, SEEK_SET);
		fputc(b, pFile);
	}
	fclose(pFile);
}

void *seqRead(void *n) {
	int  *size  = (int *)n;
	FILE *pFile = fopen(FILE_NAME, "r");
	int pos = 0;
	int i   = 0;
	for (i = 0; i < *size; i++) {
		fgetc(pFile);
	}
	fclose(pFile);
}

void *seqWrite(void *n) {
	int  *size  = (int *)n;
	FILE *pFile = fopen(FILE_NAME, "w");
	int pos = 0;
	int i	= 0;
	char a  = 'a';
	for (i = 0; i < *size; i++) {
		fputc(a, pFile);
	}
	fclose(pFile);
}

int main() {

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
			double exec_t = 1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0;
			double throughput = (n / (1024.0 * 1024.0)) / (exec_t / 1000.0);
			printf("With %d threads, sequential write %10d bytes, the latency is %10f ms and the throughput     is %10f MB/sec\n", i, n, exec_t, throughput);

			gettimeofday(&start_t, NULL);
			for (k = 0; k < i; k++)
				pthread_create(&pthreads[k], NULL, seqRead, &n);
			for (k = 0; k < i; k++)
				pthread_join(pthreads[k], NULL);
			gettimeofday(&end_t, NULL);
			exec_t = 1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0;
			throughput = (n / (1024.0 * 1024.0)) / (exec_t / 1000.0);
			printf("With %d threads, sequential read %10d bytes, the latency is %10f ms and the throughput     is %10f MB/sec\n", i, n, exec_t, throughput);

			gettimeofday(&start_t, NULL);
			for (k = 0; k < i; k++)
				pthread_create(&pthreads[k], NULL, ranWrite, &n);
			for (k = 0; k < i; k++)
				pthread_join(pthreads[k], NULL);
			gettimeofday(&end_t, NULL);
			exec_t = 1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0;
			throughput = (n / (1024.0 * 1024.0)) / (exec_t / 1000.0);
			printf("With %d threads, randomly write %10d bytes, the latency is %10f ms and the throughput     is %10f MB/sec\n", i, n, exec_t, throughput);

			gettimeofday(&start_t, NULL);
			for (k = 0; k < i; k++)
				pthread_create(&pthreads[k], NULL, ranRead, &n);
			for (k = 0; k < i; k++)
				pthread_join(pthreads[k], NULL);
			gettimeofday(&end_t, NULL);
			exec_t = 1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0;
			throughput = (n / (1024.0 * 1024.0)) / (exec_t / 1000.0);
			printf("With %d threads, randomly read %10d bytes, the latency is %10f ms and the throughput     is %10f MB/sec\n", i, n, exec_t, throughput);
		}
	}
	remove(FILE_NAME);
}
