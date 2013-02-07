#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <string.h>

void *ranRead(void *n) {
	int  *size  = (int *)n;
	FILE *pFile = fopen("/mnt/common/sxie/FILE1", "r");
	unsigned long m = 1024ul * 1024ul * 1024ul * 2ul;
	unsigned long pos = 0;
  	int i   = 0;
	char * buffer = (char *)malloc((*size)*sizeof(char)); //buffer
	int k = 0; //number of loops
	if (*size == 1 || *size == 1024)
		k = 10000;
	else if (*size == 1024*1024)
		k = 1000;
	else k = 2;
	for (i = 0; i < k; i++) {
		pos = rand() % (m - *size); //randomly find a position
		fseek(pFile, pos, SEEK_SET);
		fread(buffer, *size, 1, pFile); //read one block from disk
	}
	fclose(pFile);
	free(buffer);
}

void *ranRead2(void *n) {
	int  *size  = (int *)n;
	FILE *pFile = fopen("/mnt/common/sxie/FILE1_2", "r");
	unsigned long m = 1024ul * 1024ul * 1024ul * 2ul;
	unsigned long pos = 0;
  	int i   = 0;
	char * buffer = (char *)malloc((*size)*sizeof(char));
	int k = 0;
	if (*size == 1 || *size == 1024)
		k = 10000;
	else if (*size == 1024*1024)
		k = 1000;
	else k = 2;
	for (i = 0; i < k; i++) {
		pos = rand() % (m - *size);
		fseek(pFile, pos, SEEK_SET);
		fread(buffer, *size, 1, pFile);
	}
	fclose(pFile);
	free(buffer);
}

void *ranWrite(void *n) {
	int  *size  = (int *)n;
	FILE *pFile = fopen("/mnt/common/sxie/FILE2", "r+");
	fseek(pFile, 0, SEEK_SET);
	unsigned long m =  1024ul * 1024ul * 1024ul * 2ul;
	unsigned long pos = 0;
	int i   = 0;
	char * buffer = (char *)malloc((*size)*sizeof(char));
	int k = 0; //number of loops
	if (*size == 1 || *size == 1024)
		k = 10000;
	else if (*size == 1024*1024)
		k = 1000;
	else k = 2;
	for (i = 0; i < k; i++) {
		pos = rand() % (m - *size); //randomly find a position
		fseek(pFile, pos, SEEK_SET);
		fwrite(buffer, *size, 1, pFile);
	}
	fclose(pFile);
	free(buffer);
}

void *ranWrite2(void *n) {
	int  *size  = (int *)n;
	FILE *pFile = fopen("/mnt/common/sxie/FILE2_2", "r+");
	fseek(pFile, 0, SEEK_SET);
	unsigned long m =  1024ul * 1024ul * 1024ul * 2ul;
	unsigned long pos = 0;
	int i   = 0;
	char * buffer = (char *)malloc((*size)*sizeof(char));
	int k = 0;
	if (*size == 1 || *size == 1024)
		k = 10000;
	else if (*size == 1024*1024)
		k = 1000;
	else k = 2;
	for (i = 0; i < k; i++) {
		pos = rand() % (m - *size);
		fseek(pFile, pos, SEEK_SET);
		fwrite(buffer, *size, 1, pFile);
	}
	fclose(pFile);
	free(buffer);
}

void *seqRead(void *n) {
	int  *size  = (int *)n;
	FILE *pFile = fopen("/mnt/common/sxie/FILE3", "r");
	int k = 0;
	if (*size == 1 || *size == 1024)
		k = 10000;
	else if (*size == 1024*1024)
		k = 1000;
	else k = 2;
	char *buffer = (char *)malloc((*size)*sizeof(char));
	int pos = rand(); //randomly find a position to read first
	fseek(pFile, pos, SEEK_SET);
	int i = 0;
	for (i = 0; i < k; i++)
		fread(buffer, *size, 1, pFile); //read one block multiple times
	fclose(pFile);
	free(buffer);
}

void *seqRead2(void *n) {
	int  *size  = (int *)n;
	FILE *pFile = fopen("/mnt/common/sxie/FILE3_2", "r");
	int k = 0;
	if (*size == 1 || *size == 1024)
		k = 10000;
	else if (*size == 1024*1024)
		k = 1000;
	else k = 2;
	char *buffer = (char *)malloc((*size)*sizeof(char));
	int pos = rand();
	fseek(pFile, pos, SEEK_SET);
	int i = 0;
	for (i = 0; i < k; i++)
		fread(buffer, *size, 1, pFile);
	fclose(pFile);
	free(buffer);
}

void *seqWrite(void *n) {
	int  *size  = (int *)n;
	char *buffer = (char *)malloc((*size)*sizeof(char));
	FILE *pFile = fopen("/mnt/common/sxie/FILE4", "r+");
	int k = 0;
	if (*size == 1 || *size == 1024)
		k = 10000;
	else if (*size == 1024*1024)
		k = 1000;
	else k = 2;
	int pos = rand();
	fseek(pFile, pos, SEEK_SET);
	int i = 0;
	for (i = 0; i < k; i++)
		fwrite(buffer, *size, 1, pFile); //sequential read multiple blocks
	fclose(pFile);
	free(buffer);
}

void *seqWrite2(void *n) {
	int  *size  = (int *)n;
	char *buffer = (char *)malloc((*size)*sizeof(char));
	FILE *pFile = fopen("/mnt/common/sxie/FILE4_2", "r+");
	int k = 0;
	if (*size == 1 || *size == 1024)
		k = 10000;
	else if (*size == 1024*1024)
		k = 1000;
	else k = 2;
	int pos = rand();
	fseek(pFile, pos, SEEK_SET);
	int i = 0;
	for (i = 0; i < k; i++)
		fwrite(buffer, *size, 1, pFile);
	fclose(pFile);
	free(buffer);
}

int main() {

	srand((int)time(0));
	int i = 0; //number of threads
	for (i = 1; i <=2; i++) {
		int j = 0;
		for (j = 0; j < 4; j++) {
			int n = pow(1024.0, (double) j); //block size
			pthread_t pthreads[i];
			struct timeval start_t, end_t;
			int k = 0;
			int m = 0; //number of loops
			if (n == 1 || n == 1024)
				m = 10000;
			else if (n == 1024*1024)
				m = 1000;
			else m = 2;

			gettimeofday(&start_t, NULL);  //time start
			for (k = 0; k < i; k++) //sequential write
				if (k == 0)	
					pthread_create(&pthreads[k], NULL, seqWrite, &n);
				else
					pthread_create(&pthreads[k], NULL, seqWrite2, &n);
			for (k = 0; k < i; k++)
				pthread_join(pthreads[k], NULL);
			gettimeofday(&end_t, NULL); //time end
			double exec_t = (1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0)/m;
			double throughput = (n / (1024.0 * 1024.0)) / (exec_t / 1000.0);
			printf("With %d threads, sequential write %10d bytes, the latency is %10lf ms and the throughput     is %10lf MB/sec\n", i, n, exec_t/n, throughput * i);
		
			//sequential read
			gettimeofday(&start_t, NULL);
			for (k = 0; k < i; k++)
				if (k == 0)
					pthread_create(&pthreads[k], NULL, seqRead, &n);
				else
					pthread_create(&pthreads[k], NULL, seqRead2, &n);
			for (k = 0; k < i; k++)
				pthread_join(pthreads[k], NULL);
			gettimeofday(&end_t, NULL);
			exec_t = (1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0)/m;
			throughput = (n / (1024.0 * 1024.0)) / (exec_t / 1000.0);
			printf("With %d threads, sequential  read %10d bytes, the latency is %10lf ms and the throughput     is %10lf MB/sec\n", i, n, exec_t/n, i*throughput);

			//randomly write
			gettimeofday(&start_t, NULL);
			for (k = 0; k < i; k++) {
				if (k == 0)
					pthread_create(&pthreads[k], NULL, ranWrite, &n);
				else
					pthread_create(&pthreads[k], NULL, ranWrite2, &n);
			}
			for (k = 0; k < i; k++)
				pthread_join(pthreads[k], NULL);
			gettimeofday(&end_t, NULL);
			exec_t = (1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0)/m;
			throughput = (n / (1024.0 * 1024.0)) / (exec_t / 1000.0);
			printf("With %d threads, randomly   write %10d bytes, the latency is %10lf ms and the throughput     is %10lf MB/sec\n", i, n, exec_t/n, throughput * i);

			//randomly read
			gettimeofday(&start_t, NULL);
			for (k = 0; k < i; k++) {
				if (k == 0)
					pthread_create(&pthreads[k], NULL, ranRead, &n);
				else
					pthread_create(&pthreads[k], NULL, ranRead2, &n);
			}
			for (k = 0; k < i; k++)
				pthread_join(pthreads[k], NULL);
			gettimeofday(&end_t, NULL);
			exec_t = (1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0)/m;
			throughput = (n / (1024.0 * 1024.0)) / (exec_t / 1000.0);
			printf("With %d threads, randomly    read %10d bytes, the latency is %10lf ms and the throughput     is %10lf MB/sec\n", i, n, exec_t/n, throughput * i);
		}
	}
}
