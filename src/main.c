#include "main.h"
#include <stdio.h>
#include <string.h>


int print_usage( int argc, char ** argv )
{
	int i=0;
	printf("packer [OPTIONS] <extra_args ...>\n");
	printf("Supplied arguments: %i\n", argc );
	for( i=1; i<argc; i++ )
		printf("Arg[%i]: %s\n", i, argv[i]);
	return e_err_no_args;
}

int check_mode( int argc, char ** argv )
{
  int i;
  if( argc < REQUIRED_MIN_ARGS )
    return print_usage( argc, argv );
  for( i=0; i<argc; i++ ){
    if( !strcmp( argv[i], "-c" ) )
      return e_cmd_pack;
  }
  return e_err_none;
}

int print_error_value( int code )
{
  if( code == e_err_none )
    printf("Success\n");
  else if( code == e_err_no_args )
    printf("Error(%i): Not enough arguments\n", code);
  else if( code == e_err_invalid_arg )
    printf("Error(%i): Invalid argument\n", code);
  else if( code == e_err_missing_arg )
    printf("Error(%i): Missing argument\n", code);
  else
    printf("Error(%i): Unknown error\n", code);
  return code;
}

int main( int argc, char ** argv )
{
  int ret_c;

  ret_c = check_mode( argc, argv );

  return (print_error_value( ret_c ));
}
