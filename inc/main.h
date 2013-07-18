#ifndef __MAIN__HH__INCLUDED__
#define __MAIN__HH__INCLUDED__

typedef enum
{
  e_cmd_none,
  e_cmd_pack,
  e_cmd_unpack
} e_command;

typedef enum
{
  e_err_none,
  e_err_no_args       = -1,
  e_err_invalid_arg   = -2,
  e_err_missing_arg   = -3
} e_error;

/*
** Sets the minimum number of arguments required
*/
#define REQUIRED_MIN_ARGS 2


int print_error_value( int code );
int check_mode( int argc, char ** argv );
int print_usage( int argc, char ** argv );
int main( int argc, char ** argv );

#endif
