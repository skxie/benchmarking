#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <string.h>

//#define FILE_NAME "/mnt/common/sxie/YOUR_FILE"

void *ranRead(void *n) {
	int  *size  = (int *)n;
	FILE *pFile = fopen("/mnt/common/sxie/FILE1", "r");
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
	fseek(pFile, 0, SEEK_SET);
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
	fseek(pFile, 0, SEEK_SET);
	int i = 0;
	for (i = 0; i < k; i++)
		fwrite(buffer, *size, 1, pFile);
	fclose(pFile);
	free(buffer);
}

void makeNewFile(int size, char *arg) {
	
	char * fileName = (char *)malloc(sizeof(char)*100);
	memset(fileName, 0, sizeof(char)*100);
	sprintf(fileName, "/mnt/common/sxie/%s-%d.txt", arg, size);
	FILE * pFile = fopen(fileName, "w");
	int i = 0;
	int a = 0;
	char c = 'a';
	for (i = 0; i < size; i++) {
		a = rand()&1;
		if (a == 0)
			c = 'a' + rand()%26;
		else
			c = 'A' + rand()%26;
		fread(&c, 1, 1, pFile);	
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
			int m = 0;
			if (n == 1 || n == 1024)
				m = 10000;
			else if (n == 1024*1024)
				m = 1000;
			else m = 2;

			//makeNewFile(n, "seqwrite");
			gettimeofday(&start_t, NULL);
			for (k = 0; k < i; k++)
				pthread_create(&pthreads[k], NULL, seqWrite, &n);
			for (k = 0; k < i; k++)
				pthread_join(pthreads[k], NULL);
			gettimeofday(&end_t, NULL);
			double exec_t = (1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0)/m;
			double throughput = (n / (1024.0 * 1024.0)) / (exec_t / 1000.0);
			printf("With %d threads, sequential write %10d bytes, the latency is %10lf ms and the throughput     is %10lf MB/sec\n", i, n, exec_t/n, throughput * i);
			//char * fileName = (char *)malloc(sizeof(char)*100);
			//memset(fileName, 0, sizeof(char)*100);
			//sprintf(fileName, "/mnt/common/sxie/%s-%d.txt", "seqwrite", n);
			//remove(fileName);
			
			//makeNewFile(n, )
			gettimeofday(&start_t, NULL);
			for (k = 0; k < i; k++)
				pthread_create(&pthreads[k], NULL, seqRead, &n);
			for (k = 0; k < i; k++)
				pthread_join(pthreads[k], NULL);
			gettimeofday(&end_t, NULL);
			exec_t = (1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0)/m;
			throughput = (n / (1024.0 * 1024.0)) / (exec_t / 1000.0);
			printf("With %d threads, sequential  read %10d bytes, the latency is %10lf ms and the throughput     is %10lf MB/sec\n", i, n, exec_t/n, i*throughput);

			gettimeofday(&start_t, NULL);
			for (k = 0; k < i; k++)
				pthread_create(&pthreads[k], NULL, ranWrite, &n);
			for (k = 0; k < i; k++)
				pthread_join(pthreads[k], NULL);
			gettimeofday(&end_t, NULL);
			exec_t = (1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0)/m;
			throughput = (n / (1024.0 * 1024.0)) / (exec_t / 1000.0);
			printf("With %d threads, randomly   write %10d bytes, the latency is %10lf ms and the throughput     is %10lf MB/sec\n", i, n, exec_t/n, throughput * i);

			gettimeofday(&start_t, NULL);
			for (k = 0; k < i; k++)
				pthread_create(&pthreads[k], NULL, ranRead, &n);
			for (k = 0; k < i; k++)
				pthread_join(pthreads[k], NULL);
			gettimeofday(&end_t, NULL);
			exec_t = (1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0)/m;
			throughput = (n / (1024.0 * 1024.0)) / (exec_t / 1000.0);
			printf("With %d threads, randomly    read %10d bytes, the latency is %10lf ms and the throughput     is %10lf MB/sec\n", i, n, exec_t/n, throughput * i);
		}
	}
}
