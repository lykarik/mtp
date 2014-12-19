/*Hello, world with use pthread
	for compilating use key -lpthread
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <pthread.h>

void *thread(void *vargp) { 
	printf("hello world\n");
	pthread_exit((void*)1);
}

int main(int argc, char** argv) {
	if (argc == 2) {
		int count = atoi(argv[1]);
		pthread_t* array = malloc(count * sizeof(pthread_t)); 
		int i;
		
		for (i = 0; i < count; i++) {
			if (pthread_create(&array[i], NULL, thread, NULL) != 0) { 
				perror("Cannot create thread");
			}
		}
		
		int ans;
		
		for (i = 0; i < count; i++) { 
			pthread_join(array[i], NULL);
		}
		
		printf("goodbye world\n");
		free(array);
	
	} else {
		perror("Wrong Count of Arguments");
		return -1;
	}
}
