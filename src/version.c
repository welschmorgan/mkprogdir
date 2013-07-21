#include "version.h"
#include <string.h>
#include <stdio.h>

char * get_brand_name( t_version_brand b ){
  static char _brand[10];
  strncpy( _brand, "unknown", 10 );

  if( b == e_brand_alpha )
    strncpy( _brand, "alpha", 10 );
  else if( b == e_brand_alpha )
    strncpy( _brand, "beta", 10 );
  else if( b == e_brand_stable )
    strncpy( _brand, "stable", 10 );
  else if( b == e_brand_dev )
    strncpy( _brand, "dev", 10 );
  
  return _brand;
}

t_version get_current_version( void ){
  t_version ret;
  init_version( &ret, 
		CURRENT_VERSION_MAJOR,
		CURRENT_VERSION_MINOR,
		CURRENT_VERSION_BUILD,
		CURRENT_VERSION_BRAND );
  return ret;
}

int init_version( t_version * vers, 
		  unsigned short major, 
		  unsigned short minor,
		  unsigned short build,
		  t_version_brand brand ){
  if( !vers )
    return 0;
  vers->major = major;
  vers->minor = minor;
  vers->build = build;
  vers->brand = brand;
  return 1;
}


char* build_version_string( t_version *vers, 
			    char * buf, 
			    size_t buf_size ){
  if( !buf )
    return NULL;
  if( !vers )
    return NULL;
  snprintf( buf, buf_size, "%u.%u.%u-%s",
	    vers->major,
	    vers->minor,
	    vers->build,
	    get_brand_name(vers->brand) );
  return buf;
}
