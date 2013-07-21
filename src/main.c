#include "main.h"
#include "version.h"
#include "commands.h"
#include <stdio.h>
#include <string.h>


int main( int argc, char ** argv )
{
  int ret_c;

  if( has_verbose_mode( argc, argv ) ){
    g_verbose = true;
    printf("Verbose mode enabled\n");
  }
  if( has_interactive_mode( argc, argv ) ){
    g_interactive = true;
    if( g_verbose )
      printf("Running in interactive mode\n");
  }
  extract_working_dir( argc, argv );
  ret_c = check_mode( argc, argv );
  if( ret_c == e_cmd_create )
    return create_mode( argc, argv );
  else if( ret_c == e_cmd_summary )
    return summary_mode( argc, argv );
  else if( ret_c == e_cmd_help )
    return help_mode();
  else if( ret_c == e_cmd_version )
    return version_mode();

  return (print_error_value( ret_c ));
}
