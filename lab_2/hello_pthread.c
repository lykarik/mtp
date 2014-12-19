//gcc Hello_pthread.c -lpthread
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <pthread.h>

void *thread(void *vargp) {  //функция для нити
	printf("hello world\n");
	pthread_exit((void*)1);
}

int main(int argc, char** argv) {
	if (argc == 2) {
		int count = atoi(argv[1]);
		pthread_t* array = malloc(count * sizeof(pthread_t)); //массив индексов, в котором храним инфу о нитях
		int i;
		for (i = 0; i < count; i++) {
			if (pthread_create(&array[i], NULL, thread, NULL) != 0) { //создаём нити
				perror("Cannot create thread");
			}
		}
		int ans;
		for (i = 0; i < count; i++) { //собираем нити обратно
			pthread_join(array[i], NULL);
		}
		printf("goodbye world\n");
		free(array);
	} else {
		perror("Wrong Count of Arguments");
		return -1;
	}
}
