#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

#define NUM_LOOPS 1000

void *flops(void *arg) {
	double sum = 0.0;
	double i = 0.0;
	double j = 0.0;
	for (i = 0.0; i < NUM_LOOPS; i = i + 1.0) {
		for (j = 0.0; j < NUM_LOOPS; j = j + 1.0) {
			sum = sum + i + j;
		}		
	}
}

void *iops(void *arg) {
	int sum = 0;
	int i = 0;
	int j = 0;
	for (i = 0; i < NUM_LOOPS; i++) {
		for (j = 0; j < NUM_LOOPS; j++) {
			sum = sum + i + j;
		}
	}
}



int main() {
	int i = 0;
	for (i = 0; i < 3; i++) {
		int n = pow(2.0, (double) i);
		int operations = 3 * 2 * NUM_LOOPS * 2 * NUM_LOOPS * n;
		pthread_t pthreads[n];
		int k = 0;
		void *arg;
		struct timeval start_t, end_t;
		
		gettimeofday(&start_t, NULL);
		for (k = 0; k < n; k++)
			pthread_create(&pthreads[k], NULL, flops, arg);
		for (k = 0; k < n; k++)
			pthread_join(pthreads[k], NULL);
		gettimeofday(&end_t, NULL);
		double exec_t = 1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0;
		double flops  = ((double) operations / (exec_t / 1000.0)) / 1e9;
		printf("With %d threads, the execution time is %10f ms and the GFLOPS is %10f\n", n, exec_t, flops);

		gettimeofday(&start_t, NULL);
		for (k = 0; k < n; k++)
			pthread_create(&pthreads[k], NULL, iops, arg);
		for (k = 0; k < n; k++)
			pthread_join(pthreads[k], NULL);
		gettimeofday(&end_t, NULL);
		exec_t = 1000.0*(end_t.tv_sec - start_t.tv_sec) + (end_t.tv_usec - start_t.tv_usec)/1000.0;
		double iops  = ((double) operations / (exec_t / 1000.0)) / 1e9;
		printf("With %d threads, The execution time is %10f ms and the GIOPS  is %10f\n", n, exec_t, iops);
	}
}
