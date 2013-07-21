#include "commands.h"
#include "version.h"

#include <dirent.h>
#include <stdio.h>
#include <string.h>

bool g_interactive = false;
bool g_verbose = false;
char g_working_dir[260];

void extract_working_dir( int argc, char ** argv )
{
  strncpy( g_working_dir, "", 260 );
  if(has_mode("-d", argc, argv) || has_mode("--dir", argc, argv)){
    if( has_mode("-d", argc, argv) )
      get_mode_args( "-d", g_working_dir, 260, argc, argv );
    else
      get_mode_args( "--dir", g_working_dir, 260, argc, argv );
  }
  
  if( strlen(g_working_dir) == 0 ){
    if( g_verbose )
      printf("No directory supplied, using current working directory\n");
    strncpy( g_working_dir, "./", 260 );
  }
  if( g_working_dir[strlen(g_working_dir-1)] != '/' || g_working_dir[strlen(g_working_dir-1)] != '\\' )
    snprintf( g_working_dir, 260, "%s%c", g_working_dir, '/' );
  if( g_verbose )
    printf("Directory: %s\n", g_working_dir);
}

int version_mode( void )
{
  char buffer[255];
  t_version vers;

  vers = get_current_version();
  if( g_verbose )
    printf( "Version: %s\n", 
	  build_version_string( &vers,
				buffer,
				255 ) );
  else printf("%s\n", build_version_string( &vers, buffer, 255 ));
  return e_err_none;
}

int help_mode( void )
{
  return 0;
}

bool dir_exists( const char * path )
{
  DIR * dip;
  if ((dip=opendir(path)) == NULL )
    return false;
  closedir(dip);
  return true;
}
bool file_exists( const char * path )
{
  FILE *fd = NULL;
  if( (fd=fopen(path, "r")) != NULL ){
    fclose(fd);
    return true;
  }
  return false;
}

char* get_mode_args( const char * name, char *buf, unsigned int buf_len, int argc, char **argv )
{
  int i;
  if( !name || !buf ) return NULL;
  strncpy( buf, "", buf_len );
  for( i=1; i<argc; i++ ){
    if( !strcmp( argv[i], name ) ){
      if( (i+1) < argc )
	strncpy( buf, argv[i+1], buf_len );
      break;
    }
  }
  return buf;
}

int summary_mode(void)
{
  char buf[512];
  if( g_verbose )
    printf("Making summary\n");

  snprintf( buf, 512, "%ssrc", g_working_dir );
  if( g_verbose )
    printf("Has source dir: %s\n", dir_exists(buf)?"yes":"no" );
  else
    printf("src:%s|", dir_exists(buf)?"yes":"no" );

  snprintf( buf, 512, "%sinc", g_working_dir );
  if( g_verbose )
    printf("Has include dir: %s\n", dir_exists(buf)?"yes":"no");
  else
    printf("inc:%s|", dir_exists(buf)?"yes":"no" );

  snprintf( buf, 512, "%sbin", g_working_dir );
  if( g_verbose )
    printf("Has binary dir: %s\n", dir_exists(buf)?"yes":"no");
  else
    printf("bin:%s|", dir_exists(buf)?"yes":"no" );

  snprintf( buf, 512, "%sobj", g_working_dir );
  if( g_verbose )
    printf("Has object dir: %s\n", dir_exists(buf)?"yes":"no");
  else
    printf("obj:%s|", dir_exists(buf)?"yes":"no" );

  snprintf( buf, 512, "%smakefile", g_working_dir );
  if( g_verbose )
    printf("Has makefile: %s\n", file_exists(buf)?"yes":"no");
  else
    printf("makefile:%s\n", file_exists(buf)?"yes":"no" );
  return e_err_none;
}

int create_mode_interactive(void)
{
  char input_buf[1024];
  bool stop;
  stop = false;

  while( !stop ){
    printf("Add File: ");
    scanf( "%s", input_buf );
    fflush(stdout);
    if( !strcmp( input_buf, "q" ) ){
      stop = true;
    }
    else{
      printf("\tadded %s\n", input_buf);
    }
  }

  return e_err_none;
}

int create_mode_automatic( int argc, char **argv )
{
  print_arguments( argc, argv );
  return e_err_none;
}

int create_mode( int argc, char **argv )
{
  printf("Creating pack ...\n");

  if( g_interactive )
    return create_mode_interactive();
  else
    return create_mode_automatic( argc, argv );

  return e_err_none;
}


int print_usage( int argc, char ** argv )
{
  printf("%s [OPTIONS] <extra_args ...>\n", PROGRAM_NAME);
  printf("\t-v(--version): prints the version and quits\n");
  printf("\t-h(--help): prints this and quits\n");
  printf("\t-c(--create): creates a classic programming dir\n");
  printf("\t-s(--summary): creates a summary of a programming dir\n");
  printf("\t-V(--Verbose): enables verbose mode\n");
  printf("\t-i(--interactive): enables interactive mode\n");
  printf("\t-d(--dir): specify the directory to work on\n");
  print_arguments( argc, argv );
  return e_err_no_args;
}

void print_arguments( int argc, char **argv )
{
  int i;
  printf("Supplied arguments: %i\n", argc );
  for( i=1; i<argc; i++ )
    printf("Arg[%i]: %s\n", i, argv[i]);
}

bool has_mode( const char * lookup, int argc, char **argv )
{
  int i;
  if( !lookup ) return false;
  for( i=1; i<argc; i++ )
    if( !strcmp( argv[i], lookup ) )
      return true;
  return false;
}

bool has_interactive_mode( int argc, char **argv ){
  return 
    has_mode( "-i", argc, argv ) || 
    has_mode( "--interactive", argc, argv );
}

bool has_verbose_mode( int argc, char **argv ){
  return 
    has_mode( "-V", argc, argv ) || 
    has_mode( "--verbose", argc, argv )|| 
    has_mode( "--Verbose", argc, argv );
}

int check_mode( int argc, char ** argv )
{
  int i;
  if( argc < REQUIRED_MIN_ARGS ){
    return print_usage( argc, argv );
  }
  for( i=0; i<argc; i++ )
  {
    if( !strcmp( argv[i], "-v" ) ||
	!strcmp( argv[i], "--version" ) )
      return e_cmd_version;
    else if( !strcmp( argv[i], "-h" ) ||
	     !strcmp( argv[i], "--help" ) )
      return e_cmd_help;
    else if( !strcmp( argv[i], "-c" ) ||
	     !strcmp( argv[i], "--create" ) )
      return e_cmd_create;
    else if( !strcmp( argv[i], "-s" ) ||
	     !strcmp( argv[i], "--summary" ) )
      return e_cmd_summary;
  }
  return e_err_none;
}

int print_error_value( int code )
{
  if( code == e_err_none )
    return code;
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
