#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <pthread.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

pthread_mutex_t barier_mutex;
sem_t *sem_barier;

const int threads = 10;

class Barier {
public:
	int count;
	int max;
	
	Barier (int pthread_count) {
		count = 0;
		max = pthread_count;
	}

	void Wait() {
		pthread_mutex_lock(&barier_mutex);
		count++;
		int k;
		if (count == max) {
			for (k = 0; k < max; k++) {
				sem_post(sem_barier);
			}
			count = 0;
		}
		pthread_mutex_unlock(&barier_mutex);
		sem_wait(sem_barier);
	}
} B(threads);

const int POWER = 100000;

void* thread(void *bar) {
	//TODO
	printf("Start new Thread\n");
	srand(rand());
	unsigned int help = rand();
	int i = 0;
	int k = 0;
	int count = (1 + rand_r(&help) % 100) * POWER; //подсчитываем кол-во итераций для каждой нити
	for (i = 0; i < count; i++) {
		float x = rand_r(&help) / 1.0 / RAND_MAX;
		float y = rand_r(&help) / 1.0 / RAND_MAX;
		if (x * x + y * y < 1) {
			k++;
		}
	}
	float pi = 4.0 * k / count;
	// 
	printf("Start WAIT\n");
	B.Wait();

	//
	printf("My PI is %1.6f \n", pi);
	pthread_exit((void*)1);
}

int main(int argc, char** argv) {
	if (argc == 1) {
		int z;
		pthread_t* array = new pthread_t[threads];  //

		pthread_mutex_init(&barier_mutex,NULL);

		sem_unlink("/barier_semaphore");
		if ( (sem_barier = sem_open("/barier_semaphore", O_CREAT, 0777, 0)) == SEM_FAILED ) {
            perror("Sem_open error");
            return -1;
        }
		int i;
		for (i = 0; i < threads; i++) {
			if (pthread_create(&array[i], NULL, thread, NULL) != 0) {
				perror("Cannot create thread");
				return -1;
			}
		}
		for (i = 0; i < threads; i++) {
			pthread_join(array[i], NULL);
		}
		pthread_mutex_destroy(&barier_mutex);

		sem_close(sem_barier);
		sem_unlink("/barier_semaphore");

		delete array;
		return 0;
	} else {
		perror("Wrong Count of Arguments");
		return -1;
	}
}
