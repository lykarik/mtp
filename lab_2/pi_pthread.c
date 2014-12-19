/* Monte-Carlo algorithm for find pi via pthread
	for compiling use -lpthread key with gcc
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <pthread.h>

pthread_mutex_t mutex; 
int n = 0; 
int threads;
int iters;
int count;

void *thread(void *vargp) {
	printf("Start new Thread\n");
	srand(n);
	int i = 0;
	int k = 0;
	
	for (i = 0; i < count; i++) {
		float x = rand() / 1.0 / RAND_MAX;
		float y = rand() / 1.0 / RAND_MAX;
		if (x * x + y * y < 1) {
			k++;
		}
	}
	
	pthread_mutex_lock(&mutex); 
	n += k;
	pthread_mutex_unlock(&mutex);
	pthread_exit((void*)1);
}

int main(int argc, char** argv) {
	if (argc == 3) {
		threads = atoi(argv[1]);
		iters = atoi(argv[2]);
		count = iters / threads;
		pthread_t* array = malloc(threads * sizeof(pthread_t)); 
		pthread_mutex_init(&mutex,NULL); 
		int i;
		
		for (i = 0; i < threads; i++) {
			if (pthread_create(&array[i], NULL, thread, NULL) != 0) {  
				perror("Cannot create thread");
				return -1;
			}
		}
		
		pthread_mutex_destroy(&mutex); 
		for (i = 0; i < threads; i++) {  
			pthread_join(array[i], NULL);
		}
		printf("Total circle is %d \n", n);
		float pi = 4.0 * n / iters; 
		printf("PI is %1.6f \n", pi);
		free(array);
	
	} else {
		perror("Wrong Count of Arguments");
		return -1;
	}
}
