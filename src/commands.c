#include "commands.h"
#include "version.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

bool g_force = false;
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

  if( g_working_dir[strlen(g_working_dir)-1] != '/' &&
     g_working_dir[strlen(g_working_dir)-1] != '\\' )
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
  print_usage(0, NULL);
  return e_err_none;
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

char * read_input_default( char * buf, unsigned int max_len, const char * default_str )
{
  if( !buf || max_len == 0 || !default_str ) return NULL;
  fflush(stdout);

  if (fgets(buf, max_len, stdin) != NULL){
    /**
     * Find the newline and, if present, zero it out
     */
    char *newline = strchr(buf, '\n');
    if (newline)
      *newline = 0;
    // input was empty
    if (strlen(buf) == 0){
      strncpy(buf, default_str, max_len);
    }
  }
  return buf;
}

int my_mkdir( const char *path )
{
  unsigned short sz;
  char *p;
  if( !path ) return -1;
  sz = sizeof(char) * (strlen(path) + 20);
  p=(char*)malloc( sz );
  if( !p ) return -2;
  snprintf( p, 260, "mkdir -m 777 -p \"%s\"", path );
  int ret = system(p);
  free( p );
  return ret;
}

int create_mode_interactive(void)
{
  int errc;
  unsigned short errcnt;
  char src[260];
  char inc[260];
  char bin[260];
  char obj[260];
  char buf[512];
  char default_src[] = "src";
  char default_inc[] = "inc";
  char default_bin[] = "bin";
  char default_obj[] = "obj";

  printf("\tSource directory (default \"%s\"): ", default_src);
  read_input_default( src, 260, default_src );

  printf("\tInclude directory (default \"%s\"): ", default_inc);
  read_input_default( inc, 260, default_inc );

  printf("\tBinary directory (default \"%s\"): ", default_bin);
  read_input_default( bin, 260, default_bin );

  printf("\tObject directory (default \"%s\"): ", default_obj);
  read_input_default( obj, 260, default_obj );  
  
  errcnt = 0;
  snprintf(buf, 512, "%s%s", g_working_dir, src );
  errc = my_mkdir( buf );
  if( g_verbose ) printf("Making directory %s: %s(%i)\n", buf, (errc==0)?"success":"error", errc );
  if( errc != 0 ) errcnt++;
  
  snprintf(buf, 512, "%s%s", g_working_dir, inc );
  errc = my_mkdir( buf );
  if( g_verbose ) printf("Making directory %s: %s(%i)\n", buf, (errc==0)?"success":"error", errc );
  if( errc != 0 ) errcnt++;

  snprintf(buf, 512, "%s%s", g_working_dir, obj );
  errc = my_mkdir( buf );
  if( g_verbose ) printf("Making directory %s: %s(%i)\n", buf, (errc==0)?"success":"error", errc );
  if( errc != 0 ) errcnt++;
  
  snprintf(buf, 512, "%s%s", g_working_dir, bin );
  errc = my_mkdir( buf );
  if( g_verbose ) printf("Making directory %s: %s(%i)\n", buf, (errc==0)?"success":"error", errc );
  if( errc != 0 ) errcnt++;

  if( g_verbose )
    printf("Done, %i error(s)", errcnt);
  if( errcnt != 0 )
    return errc;
  return e_err_none;
}

int create_mode_automatic( int argc, char **argv )
{  
  int errc;
  unsigned short errcnt;
  char src[260];
  char inc[260];
  char bin[260];
  char obj[260];
  char buf[512];
  char default_src[] = "src";
  char default_inc[] = "inc";
  char default_bin[] = "bin";
  char default_obj[] = "obj";
  strncpy( src, default_src, 260 );
  strncpy( inc, default_inc, 260 );
  strncpy( bin, default_bin, 260 );
  strncpy( obj, default_obj, 260 );

  if( has_mode("-S", argc, argv ) || has_mode("--Source", argc, argv )  ){
    if( has_mode("-S", argc, argv ) )
      get_mode_args( "-S", src, 260, argc, argv );
    else
      get_mode_args( "--Source", src, 260, argc, argv );
  }

  if( has_mode("-I", argc, argv ) || has_mode("--Include", argc, argv )  ){
    if( has_mode("-I", argc, argv ) )
      get_mode_args( "-I", inc, 260, argc, argv );
    else
      get_mode_args( "--Include", inc, 260, argc, argv );
  }

  if( has_mode("-B", argc, argv ) || has_mode("--Binary", argc, argv )  ){
    if( has_mode("-B", argc, argv ) )
      get_mode_args( "-B", bin, 260, argc, argv );
    else
      get_mode_args( "--Binary", bin, 260, argc, argv );
  }

  if( has_mode("-O", argc, argv ) || has_mode("--Object", argc, argv ) ){
    if( has_mode("-O", argc, argv ) )
      get_mode_args( "-O", obj, 260, argc, argv );
    else
      get_mode_args( "--Object", obj, 260, argc, argv );
  }
  if( g_verbose ){
    printf("\tSource directory: %s\n", src);
    printf("\tInclude directory: %s\n", inc);
    printf("\tBinary directory: %s\n", bin);
    printf("\tObject directory: %s\n", obj);
  }
  errcnt = 0;
  snprintf(buf, 512, "%s%s", g_working_dir, src );
  errc = my_mkdir( buf );
  if( g_verbose ) printf("Making directory %s: %s(%i)\n", buf, (errc==0)?"success":"error", errc );
  if( errc != 0 ) errcnt++;
  
  snprintf(buf, 512, "%s%s", g_working_dir, inc );
  errc = my_mkdir( buf );
  if( g_verbose ) printf("Making directory %s: %s(%i)\n", buf, (errc==0)?"success":"error", errc );
  if( errc != 0 ) errcnt++;

  snprintf(buf, 512, "%s%s", g_working_dir, obj );
  errc = my_mkdir( buf );
  if( g_verbose ) printf("Making directory %s: %s(%i)\n", buf, (errc==0)?"success":"error", errc );
  if( errc != 0 ) errcnt++;
  
  snprintf(buf, 512, "%s%s", g_working_dir, bin );
  errc = my_mkdir( buf );
  if( g_verbose ) printf("Making directory %s: %s(%i)\n", buf, (errc==0)?"success":"error", errc );
  if( errc != 0 ) errcnt++;

  if( g_verbose )
    printf("Done, %i error(s)", errcnt);
  if( errcnt != 0 )
    return errc;
  return e_err_none;
}
int create_file( const char * fname, const char * content )
{
  char buf[512];
  char path[512];
  FILE *fd;

  if( !fname )
    return e_err_invalid_arg;
  snprintf(path, 512, "%s%s", g_working_dir, fname );
  if( file_exists( path ) ){
    if( g_force ){
      if( g_verbose )
	printf("File %s already exists, removing it !\n", path);
      remove( path );
    }
    else{
      if( g_verbose )
	printf("File %s already exists, use forced mode to overwrite it !\n", path);
      else
	printf("Skipping %s\n", path);
      return e_err_missing_arg;
    }
  }
  fd = fopen( path, "w" );
  if( !fd ){
    if( g_verbose )
      printf("Error: Cannot open file %s for writing\n", path);
    return e_err_invalid_arg;
  }
  if( content && strlen(content)>0 ){
    if( g_verbose )
      printf("Written %u bytes to %s\n", strlen(content), path);
    fwrite( content, sizeof(char), strlen( content ), fd );
  }
  else if( g_verbose )
    printf("Written empty file %s\n", path);
  fclose( fd );
  snprintf( buf, 512, "chmod 777 \"%s\"", path);
  system(buf);
  return e_err_none;
}
int create_file_safe( const char * fname, const char * content, int argc, char **argv ){
  char buf[260];
  unsigned int i;
  snprintf(buf, 260, fname, "--no-%s", fname);

  for( i=0; i<strlen(buf); i++ )
    i = tolower( (int)buf[i] );

  if( !is_file_disabled(fname, argc, argv) )
    create_file( fname, content );
  else if( g_verbose ) 
    printf("Skipping %s as %s was specified\n", fname, buf);
  return e_err_none;
}
int create_files( int argc, char **argv )
{
  printf("Creating files ...\n");
  create_file_safe( "makefile", NULL, argc, argv );
  create_file_safe( ".gitignore", "*.o\n", argc, argv );
  create_file_safe( "LICENSE", "No one shall copy my code because it is bad !", argc, argv );
  create_file_safe( "README", "Yeah like if you cared !", argc, argv );
  create_file_safe( "AUTHORS", "DarkBoss", argc, argv );
  return e_err_none;
}
int create_mode( int argc, char **argv )
{
  int retc;
  printf("Creating programming directory ...\n");
  if( g_interactive )
    retc = create_mode_interactive();
  else
    retc = create_mode_automatic( argc, argv );
  if( retc != e_err_none ){
    if( g_verbose )
      printf("Skipping files creation as previous errors occured\n");
    return retc;
  }
  return create_files( argc, argv );
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
  printf("\t-f(--force): forces the creation of dirs and files ( overwrites )\n\n");
  printf("\t--no-makefile: ignores the makefile\n");
  printf("\t--no-.gitignore: ignores the .gitignore file\n");
  printf("\t--no-readme: ignores the README file\n");
  printf("\t--no-license: ignores the LICENSE file\n");
  printf("\t--no-authors: ignores the AUTHORS file\n");
  if( argc > 1 )
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


bool is_file_disabled( const char * name, int argc, char **argv )
{
  char buf[255];
  snprintf( buf, 255, "--no-%s", name );
  return has_mode( buf, argc, argv );
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

bool has_force_mode( int argc, char **argv ){
  return
    has_mode( "-f", argc, argv ) ||
    has_mode( "--force", argc, argv );
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
    printf("Error(%i): Miissing argument\n", code);
  else
    printf("Error(%i): Unknown error\n", code);
  return code;
}
