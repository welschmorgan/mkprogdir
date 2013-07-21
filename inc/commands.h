#ifndef __COMMANDS__H
#define __COMMANDS__H

#include <stdbool.h>
/*
** Sets the minimum number of arguments required
*/
#define REQUIRED_MIN_ARGS 2
#define PROGRAM_NAME "MakefileCreator"

typedef enum
{
  e_cmd_none,
  e_cmd_help,
  e_cmd_version,
  e_cmd_create,
  e_cmd_summary,
  e_cmd_validate
} e_command;

typedef enum
{
  e_err_none,
  e_err_no_args       = -1,
  e_err_invalid_arg   = -2,
  e_err_missing_arg   = -3
} e_error;

extern bool g_interactive;
extern bool g_verbose;
extern char g_working_dir[260];

/*
** Commands
*/
int version_mode( void );
int help_mode( void );
int create_mode( int argc, char **argv );
int summary_mode( );

char* get_mode_args( const char * name, char * buf, unsigned int buf_len, int argc, char **argv );
bool file_exists( const char * path );
bool dir_exists( const char * path );

bool has_interactive_mode( int argc, char **argv );
bool has_verbose_mode( int argc, char **argv );
bool has_mode( const char * lookup, int argc, char **argv );
int check_mode( int argc, char ** argv );

void extract_working_dir( int argc, char ** argv );
int print_error_value( int code );
int print_usage( int argc, char ** argv );
void print_arguments( int argc, char **argv );

#endif
