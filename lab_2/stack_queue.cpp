#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <pthread.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

struct Node_Stack {
	Node_Stack* next;
	int value;
};

struct Node_Queue {
	Node_Queue* next;
	int value;
};


class Stack {
public:
	pthread_mutex_t stack_mutex;
	sem_t sem_stack;
	int count;
	Node_Stack* head;
	Stack () {
		printf("stack creating\n");
		if ( sem_init(&sem_stack, 0, 0) == -1 ) {
        	perror("Sem_open error in STACK");
         
    		}
		pthread_mutex_init(&stack_mutex,NULL);

		head = NULL;
		count = 0;
		printf("stack created\n");
	}
	void Push(const int src) {
		pthread_mutex_lock(&stack_mutex);
		Node_Stack* help = new Node_Stack;
		help->next = head;
		help->value = src;
		head = help;
		count++;
		sem_post(&sem_stack);
		pthread_mutex_unlock(&stack_mutex);
		return;
	}
	int Pop() {
		sem_wait(&sem_stack);
		pthread_mutex_lock(&stack_mutex);
		int ans = head->value;
		head = head->next;
		count--;
		pthread_mutex_unlock(&stack_mutex);
		return ans;
	}
};


class Queue {
public:
		pthread_mutex_t queue_mutex;
		sem_t sem_queue;
		
	int count;
	Node_Queue* start;
	Node_Queue* end;
	Queue () {
		printf("queue creating\n");
		if ( sem_init(&sem_queue, 0, 0) == -1 ) {
	                perror("Sem_open error in QUEUE");
	        }
		pthread_mutex_init(&queue_mutex,NULL);
		start = NULL;
		end = NULL;
		count = 0;
		printf("queue created\n");
	}
	void Push(const int src) {
		pthread_mutex_lock(&queue_mutex);
		if (count == 0) {
			Node_Queue* help = new Node_Queue;
			help->value = src;
			help->next = NULL;
			start = help;
			end = help;
			count++;
			sem_post(&sem_queue);
			pthread_mutex_unlock(&queue_mutex);
			return;
		} else {
			Node_Queue* help = new Node_Queue;
			help->value = src;
			help->next = NULL;
			end->next = help;
			end = help;
			count++;
			sem_post(&sem_queue);
			pthread_mutex_unlock(&queue_mutex);
			return;
		}
	}
	int Pop() {
		sem_wait(&sem_queue);
		pthread_mutex_lock(&queue_mutex);
		int ans = start->value;
		start = start->next;
		count--;
		pthread_mutex_unlock(&queue_mutex);
		return ans;
	}
};

int ns = 0;
int nq = 0;
int threads;
int iters;
int count;

Stack stack;
Queue queue;

void* thread(void *vargp) {
	printf("Start new Thread\n");
	srand(rand());
	unsigned int help = rand();
	int i = 0;
	int k = 0;
	for (i = 0; i < count; i++) {
		float x = rand_r(&help) / 1.0 / RAND_MAX;
		float y = rand_r(&help) / 1.0 / RAND_MAX;
		if (x * x + y * y < 1) {
			k++;
		}
	}
	stack.Push(k);
	queue.Push(k);

	pthread_exit((void*)1);
}

int main(int argc, char** argv) {
	if (argc == 3) {
		threads = atoi(argv[1]);
		iters = atoi(argv[2]);
		count = iters / threads;
		pthread_t* array = new pthread_t[threads];

		
		int i;
		for (i = 0; i < threads; i++) {
			if (pthread_create(&array[i], NULL, thread, NULL) != 0) {
				std::cout << "Cannot create thread" << std::endl;
				return -1;
			}
		}
		for (i = 0; i < threads; i++) {
			ns += stack.Pop();
			nq += queue.Pop();
		}
		for (i = 0; i < threads; i++) {
			pthread_join(array[i], NULL);
		}

		printf("Total Stack circle is %d \n", ns);
		printf("Total Queue circle is %d \n", nq);
		float pi = 4.0 * ns / iters;
		printf("Stack PI is %1.6f \n", pi);
		pi = 4.0 * nq / iters;
		printf("Queue PI is %1.6f \n", pi);

		//delete array;
		return 0;
	} else {
		perror("Wrong Count of Arguments");
		return -1;
	}
}
