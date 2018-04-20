#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

typedef struct addr_in_use
{
	void* addr;
	int ref_count;
}addr_in_use;

typedef struct files_in_use
{
	char* filename;
	int ref_count;
	FILE* filePoint;
}files_in_use;

addr_in_use address[25] = {NULL};
files_in_use files[25] = {NULL};
int numOfAddr = 0;
int numOfFile = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void cse320_init()
{
	pthread_mutex_lock( &mutex );
	int i;
	for(i = 0; i < 25; i++)
	{
		address[i].addr = NULL;
		address[i].ref_count = 0;
		files[i].filename = NULL;
		files[i].ref_count = 0;
		files[i].filePoint = NULL;
	}
	pthread_mutex_unlock( &mutex );
}

void* cse320_malloc(int size)
{
	pthread_mutex_lock( &mutex );
	if(numOfAddr >= 25)
	{
		errno = ENOMEM;
		write(1, "Not Enough Memory \n", sizeof("Not Enough Memory \n"));
		pthread_mutex_unlock( &mutex );
		_exit(-1);
	}
	void* pointer = malloc(size);
	int counter = 0;
	while(address[counter].ref_count != 0)
	{
		counter++;
	}
	address[counter].addr = pointer;
	address[counter].ref_count = 1;
	numOfAddr++;
	pthread_mutex_unlock( &mutex );
	return pointer;
}

void cse320_free(void* addr)
{
	pthread_mutex_lock( &mutex );
	int counter = 0;
	while(counter < numOfAddr && address[counter].addr != addr)
	{
		counter++;
	}
	if(counter < numOfAddr)
	{
		if(address[counter].ref_count == 0)
		{
			write(1, "Free: Double free attempt \n", sizeof("Free: Double free attempt \n"));
			errno = EADDRNOTAVAIL;
			pthread_mutex_unlock( &mutex );
			_exit(-1);
		}
		else
		{
			address[counter].ref_count = address[counter].ref_count - 1;
			free(addr);
			numOfAddr--;
			pthread_mutex_unlock( &mutex );
		}
	}
	else
	{
		write(1, "Free: Illegal Address \n", sizeof("Free: Illegal Address \n"));
		errno = EFAULT;
		pthread_mutex_unlock( &mutex );
		_exit(-1);
	}
}
FILE* cse320_fopen(char* file, char* command)
{
	pthread_mutex_lock( &mutex );
	if(numOfFile >= 25)
        {
                errno = ENOMEM;
                write(1, "Not Enough Memory \n", sizeof("Not Enough Memory \n"));
		pthread_mutex_unlock( &mutex );
                _exit(-1);
        }
        int counter = 0;
        while(files[counter].ref_count != 0)
        {
		if(strcmp(files[counter].filename, file) == 0)
		{
			break;
		}
                counter++;
        }
	if(files[counter].ref_count == 0)
	{
        	files[counter].filename = file;
        	files[counter].ref_count = 1;
        	numOfFile++;
		FILE* fp = fopen(file, command);
		files[counter].filePoint = fp;
		pthread_mutex_unlock( &mutex );
		return fp;
	}
	else
	{
		files[counter].ref_count = files[counter].ref_count + 1;
		pthread_mutex_unlock( &mutex );
		return files[counter].filePoint;
	}
}
int cse320_fclose(FILE* closeFile)
{
	pthread_mutex_lock( &mutex );
	int counter = 0;
        while(counter < numOfFile && files[counter].filePoint != closeFile)
        {
                counter++;
        }
        if(counter < numOfFile)
        {
                if(files[counter].ref_count == 0)
                {
                        write(1, "Close: Ref Count is zero \n", sizeof("Close: Ref Count is zero \n"));
                        errno = EINVAL;
			pthread_mutex_unlock( &mutex );
                        _exit(-1);
                }
                else if(files[counter].ref_count == 1)
		{
			if(files[counter].filePoint != NULL)
			{
				fclose(closeFile);
			}
			files[counter].ref_count = 0;
			numOfFile = numOfFile - 1;
			pthread_mutex_unlock( &mutex );
			return 0;
		}
		else
                {
                        files[counter].ref_count = files[counter].ref_count - 1;
			pthread_mutex_unlock( &mutex );
			return 0;
                }
        }
        else
        {
                write(1, "Close: Illegal filename \n", sizeof("Close: Illegal filename \n"));
                errno = ENOENT;
		pthread_mutex_unlock( &mutex );
                _exit(-1);
        }
}
void cse320_clean()
{
	pthread_mutex_lock( &mutex );
	int i;
	for(i = 0; i < 25; i++)
	{
		if(address[i].ref_count != 0)
		{
			free(address[i].addr);
			address[i].ref_count = 0;
		}
		if(files[i].ref_count != 0)
		{
			if(files[i].filePoint != NULL)
			{
				fclose(files[i].filePoint);
			}
			files[i].ref_count = 0;
		}
	}
	pthread_mutex_unlock( &mutex );
}

int setTimer = 0;

void cse320_settimer(int i)
{
	setTimer = i;
}

void handler()
{
	wait(NULL);
	alarm(setTimer);
}

pid_t cse320_fork()
{
	pid_t pid = fork();
	if(pid < 0)
	{
		perror("Fork Failed");
        	return errno;
	}
	if(pid == 0)
	{
		return pid;
	}
	else
	{
		signal(SIGALRM, handler);
		alarm(setTimer);
		pause();
	}
}





