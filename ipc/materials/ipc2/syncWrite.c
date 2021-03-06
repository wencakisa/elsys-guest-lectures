#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include "syncStruct.h"

int main()
{
	int memFd = shm_open( "simple_memory", O_CREAT | O_RDWR, S_IRWXO );
	if( memFd == -1 )
	{
		perror("Can't open file");
		return 1;
	}

	int res;
	res = ftruncate( memFd, 4096 );
	if( res == -1 )
	{
		perror("Can't truncate file");
		return res;
	}
	
	struct shared_mem* mem = mmap( NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, memFd, 0 );
	if( mem == NULL )
	{
		perror("Can't mmap");
		return -1;
	}

	bool flip = true;
	while(true)
	{
		if(flip)
		{
			mem->first = 1;
			mem->second = 1;
		}
		else
		{
			mem->second = 0;
			mem->first = 0;
		}
		flip = !flip;
	}
	return 0;
}
