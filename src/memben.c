#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <string.h>

void *ranAccess(void *n) {
	int * size = (int *)n;
	unsigned long m = 1024ul * 1024ul * 2100ul;
	int k = 0; //number of loops
	if (*size > 1024)
		k = 2000;
	else 
		k = 1000000;
	char * mem = (char *)malloc(m*sizeof(char)); //destination memory
	char * a = (char *)malloc(k*(*size)*sizeof(char)); //source memory
	int j = 0;	
	for (j = 0; j < k; j++) {
		int idx =  rand()%(m - *size - 1); //random index
		int idx2 = rand()%(k*(*size) - *size - 1); //random index
		memcpy(mem+idx, a+idx2, *size);
	}
	free(mem);
	free(a);
}

void *seqAccess(void *n) {
	int * size = (int *)n;
	unsigned long m = 1024ul * 1024ul * 2100ul;
	int k  = 0; //number of loops
	if (*size > 1024)
		k = 2000;
	else 
		k = 1000000;
	char * mem = (char *)malloc(m*sizeof(char)); //destination memory
	char * a = (char *)malloc(k*(*size)*sizeof(char)); //source memory
	int i = 0;
	int mm = 0;
	for (i = 0; i < k; i++) {
		memcpy(mem+mm, a+mm, *size) ;
		mm += (*size); //memory access index
	}
	free(mem);
	free(a);
}

int main() {

	srand((int)time(0));
	int i = 0;
	for (i = 1; i <= 2; i++) { //number of threads
		int j = 0;
		for (j = 0; j < 3; j++) {
			int n = pow(1024.0, (double) j); //number of bytes
			int k = 0;
			int m = 0; //number of loops
			if (n > 1024)
				m = 2000;
			else 
				m = 1000000;
	
			struct timeval start, end;
			pthread_t pthreads2[i];
			gettimeofday(&start, NULL);
			for (k = 0; k < i; k++)
				pthread_create(&pthreads2[k], NULL, ranAccess, &n); //random access
			for (k = 0; k < i; k++)
				pthread_join(pthreads2[k], NULL);
			gettimeofday(&end, NULL);
			double exec_t = (1000.0*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec)/1000.0)/m;
			double throughput = (i * n / (1024.0 * 1024.0)) / (exec_t / 1000.0);
			printf("With %d threads, random     access %10d bytes, the latency is %10f ms and the throughput is %10f MB/sec\n", i, n, exec_t/n, throughput);
		

			pthread_t pthreads[i];
			struct timeval start_t, end_t;
			gettimeofday(&start_t, NULL);
			for (k = 0; k < i; k++)
				pthread_create(&pthreads[k], NULL, seqAccess, &n); // sequential access
			for (k = 0; k < i; k++)
				pthread_join(pthreads[k], NULL);
			gettimeofday(&end_t, NULL);
			exec_t = (1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0)/m;
			throughput = (i * n / (1024.0 * 1024.0)) / (exec_t / 1000.0);
			printf("With %d threads, sequential access %10d bytes, the latency is %10f ms and the throughput is %10f MB/sec\n", i, n, exec_t/n, throughput);

		}
	}
}
