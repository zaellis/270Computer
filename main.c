#include <stdio.h>
#include <stdlib.h>
#include "code.h"

int main(int argc, char * * argv)
{
	if(argc != 3)
	{
		return EXIT_FAILURE;
	}
	
	char success = createout(argv[1], argv[2]);
	
	if(success)
	{
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}