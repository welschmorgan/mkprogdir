#include "main.h"
#include <stdio.h>

int print_usage( int argc, char ** argv )
{
	int i=0;
	printf("packer [OPTIONS] <extra_args ...>\n");
	printf("Supplied arguments: %i\n", argc );
	for( i=0; i<argc; i++ )
		printf("Arg[%i]: %s\n", i, argv[i]);
	return -1;
}
int check_mode( int argc, char ** argv )
{
	if( strchr( argv[2]
}

int main( int argc, char ** argv )
{
	return ( argc >= 1 ) ? check_mode( argc, argv ) : print_usage( argc, argv );
}
