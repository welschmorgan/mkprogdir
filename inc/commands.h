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
  e_cmd_match,
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

extern bool g_force;
extern bool g_interactive;
extern bool g_verbose;
extern char g_working_dir[260];

/*******************************
** Commands
********************************/
/*
** Outputs the current version
*/
int version_mode( void );

/*
** Prints available commands etc ...
*/
int help_mode( void );

/*
** Creates the files needed
*/
int create_files( int argc, char **argv );

/*
** Creates a programming dir, automatically
*/
int create_mode_automatic( int argc, char **argv );

/*
** Creates a programming dir, by asking each directory to create
** by the user.
*/
int create_mode_interactive(void);

/*
** Creates a programming dir
*/
int create_mode( int argc, char **argv );

/*
** Summarizes the content of a directory
*/
int summary_mode( );

/*
** Reads from stdin and if empty returns the default value supplied
*/
char * read_input_default( char * buf, unsigned int max_len, const char * default_str );

/*
** Returns the argument ( n+1 ) of a given command
*/
char* get_mode_args( const char * name, char * buf, unsigned int buf_len, int argc, char **argv );

/*
** Returns true if the file exists
*/
bool file_exists( const char * path );

/*
** Returns true if the directory exists
*/
bool dir_exists( const char * path );

/*
** Checks for different modes
*/
bool is_file_disabled( const char * name, int argc, char **argv );
bool has_force_mode( int argc, char **argv );
bool has_interactive_mode( int argc, char **argv );
bool has_verbose_mode( int argc, char **argv );
bool has_mode( const char * lookup, int argc, char **argv );
int check_mode( int argc, char ** argv );

/*
** Creates a directory, recursively
**/
int my_mkdir( const char *path );

void extract_working_dir( int argc, char ** argv );

int print_error_value( int code );
int print_usage( );
void print_arguments( int argc, char **argv );

#endif
