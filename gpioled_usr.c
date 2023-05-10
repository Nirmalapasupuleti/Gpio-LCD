#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>


int main(void)
{

	//variable declaration 
	int led,led1;
	// file open and the path to set direction 
	led=open("/sys/class/gpio/gpio60/direction",O_RDWR);
	
		write(led,"out",3);

		close(led);
	
	
	// file open path to set the value
	led1=open("/sys/class/gpio/gpio60/value",O_WRONLY);
	
		write(led,"1",1);
		sleep(2);

	
	
		write(led,"0",1);

		sleep(2);
	        close(led1);
	return 0;
}


