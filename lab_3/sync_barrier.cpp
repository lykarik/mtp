#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <pthread.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

pthread_mutex_t barier_mutex1;
sem_t *sem_barier1;
pthread_mutex_t barier_mutex2;
sem_t *sem_barier2;

const int threads = 10;

class Barier {
public:
	int count1;
	int max;
	int count2;
	
	Barier (int pthread_count) {
		count1 = 0;
		count2 = 0;
		max = pthread_count;
	}

	void Wait1() {
		pthread_mutex_lock(&barier_mutex1);
		count1++;
		if (count1 == max) {
			int k;
			for (k = 0; k < max; k++) {
				sem_post(sem_barier1);
			}
			count1 = 0;
		}
		pthread_mutex_unlock(&barier_mutex1);
		sem_wait(sem_barier1);
	}
	void Wait2() {
		pthread_mutex_lock(&barier_mutex2);
		count2++;		
		if (count2 == max) {
			int k;
			for (k = 0; k < max; k++) {
				sem_post(sem_barier2);
			}
			count2 = 0;
		}
		pthread_mutex_unlock(&barier_mutex2);
		sem_wait(sem_barier2);

	}
} B(threads);

const int POWER = 10000;

void* thread(void *bar) {
	//TODO
	printf("Start new Thread\n");
	srand(rand());
	unsigned int help = rand();
	int i = 0;
	int k = 0;
	int count = (1 + rand_r(&help) % 100) * POWER;
	for (i = 0; i < count; i++) {
		float x = rand_r(&help) / 1.0 / RAND_MAX;
		float y = rand_r(&help) / 1.0 / RAND_MAX;
		if (x * x + y * y < 1) {
			k++;
		}
	}
	float pi = 4.0 * k / count;
	// 
	printf("Start WAIT1\n");
	B.Wait1();
	printf("Start WAIT2\n");
	B.Wait2();

	//
	printf("My PI is %1.6f \n", pi);
	pthread_exit((void*)1);
}

int main(int argc, char** argv) {
	if (argc == 1) {
		int z;
		pthread_t* array = new pthread_t[threads];

		pthread_mutex_init(&barier_mutex1,NULL);
		pthread_mutex_init(&barier_mutex2,NULL);

		sem_unlink("/barier_semaphore1");
		if ( (sem_barier1 = sem_open("/barier_semaphore1", O_CREAT, 0777, 0)) == SEM_FAILED ) {
            perror("Sem_open error 1");
            return -1;
        }
		sem_unlink("/barier_semaphore2");
		if ( (sem_barier2 = sem_open("/barier_semaphore2", O_CREAT, 0777, 0)) == SEM_FAILED ) {
            perror("Sem_open error 2");
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
		pthread_mutex_destroy(&barier_mutex1);
		pthread_mutex_destroy(&barier_mutex2);

		sem_close(sem_barier1);
		sem_unlink("/barier_semaphore1");

		sem_close(sem_barier2);
		sem_unlink("/barier_semaphore2");

		delete array;
		return 0;
	} else {
		perror("Wrong Count of Arguments");
		return -1;
	}
}
