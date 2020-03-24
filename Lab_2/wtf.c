#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main( int argc, char* argv[] )
  {
    int c, errflag = 0;

    while( ( c = getopt( argc, argv, "abt:" ) )
      != -1 ) {
      switch( c ) {
        case 'a': printf( "apples\n" );
                  break;
        case 'b': printf( "bananas\n" );
                  break;
        case 't': printf( "tree = %s\n", optarg );
                  break;
        case '?': ++errflag;
                  break;
      }
    }
    return EXIT_SUCCESS;
  }
