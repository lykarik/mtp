#include <stdio.h>
#include <unistd.h>
	
int main()
{
	
	printf("This is to demonstrate fork() PID=%d\n",(int) getpid());
	fork();
	printf("Hello world PID=%d\n", (int) getpid());

return 0;
}