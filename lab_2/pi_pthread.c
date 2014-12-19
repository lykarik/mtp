//gcc PI_pthread.c -lpthread
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <pthread.h>

pthread_mutex_t mutex; //глобальный мьютекс
int n = 0; //для метод Монте-Карло, сколько раз попало в круг
int threads; //кол-во нитей
int iters; //кол-во итераций
int count; // iters / threads в коде

void *thread(void *vargp) {
	printf("Start new Thread\n");
	srand(n);
	int i = 0;
	int k = 0;
	for (i = 0; i < count; i++) {           //сам метод Монте карло
		float x = rand() / 1.0 / RAND_MAX;
		float y = rand() / 1.0 / RAND_MAX;
		if (x * x + y * y < 1) {
			k++;
		}
	}
	pthread_mutex_lock(&mutex);  //блокируем мьютекс
	n += k;     //пишем, сколько попало в круг
	pthread_mutex_unlock(&mutex);  //разблокируем мьютекс
	pthread_exit((void*)1);
}

int main(int argc, char** argv) {
	if (argc == 3) {
		threads = atoi(argv[1]); //кол-во нитей
		iters = atoi(argv[2]);  //кол-во итераций
		count = iters / threads;  //итераций на одну нить
		pthread_t* array = malloc(threads * sizeof(pthread_t)); //массив нитей
		pthread_mutex_init(&mutex,NULL); //инициализация мьютекса
		int i;
		for (i = 0; i < threads; i++) {
			if (pthread_create(&array[i], NULL, thread, NULL) != 0) {  //создаём нити
				perror("Cannot create thread");
				return -1;
			}
		}
		pthread_mutex_destroy(&mutex);  //удаляем к чертям мьютекс
		for (i = 0; i < threads; i++) {  //сливаем все нити обратно в главную
			pthread_join(array[i], NULL);
		}
		printf("Total circle is %d \n", n);
		float pi = 4.0 * n / iters;  //вычисляем пи
		printf("PI is %1.6f \n", pi);
		free(array);
	} else {
		perror("Wrong Count of Arguments");
		return -1;
	}
}
