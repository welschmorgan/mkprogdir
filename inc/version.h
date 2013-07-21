#ifndef __VERSION__H
#define __VERSION__H

#define VERSION_MAJOR_MAX 32767
#define VERSION_MINOR_MAX 32767
#define VERSION_BUILD_MAX 32767


typedef enum e_version_brand{
  e_brand_alpha = 'a',
  e_brand_beta = 'b',
  e_brand_stable = 's',
  e_brand_dev = 'd'
} t_version_brand;

#define CURRENT_VERSION_MAJOR 0
#define CURRENT_VERSION_MINOR 0
#define CURRENT_VERSION_BUILD 1
#define CURRENT_VERSION_BRAND e_brand_dev

typedef struct s_version{
  unsigned short major;
  unsigned short minor;
  unsigned short build;  
  t_version_brand brand;
} t_version;

/*
** Returns a brand name from an enumerable brand
*/
char * get_brand_name( t_version_brand );

/*
** Returns the built-in version of this software
*/
t_version get_current_version( void );

/*
** Initialises a version struct !
*/
int init_version( t_version * vers,
		  unsigned short major, 
		  unsigned short minor,
		  unsigned short build,
		  t_version_brand brand );

/*
** Builds a complete version string
**   - Under the format "MAJOR.MINOR.BUILD-BRAND"
**   - Into the buffer 'buf' of size 'buf_size'
**   - Returns the supplied buffer
*/
char* build_version_string( t_version *vers, 
			    char * buf,
			    unsigned int buf_size );


#endif
