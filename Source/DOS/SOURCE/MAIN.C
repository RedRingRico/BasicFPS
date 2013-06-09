#include <dos.h>
#include <stdlib.h>
#include <Renderer.h>

int main( int p_Argc, char p_ppArgv )
{
	unsigned long i = 0L;
	unsigned long x = 0L;
	unsigned long y = 0L;
	int Colour = 0;

	if( StartVideoMode( ) != 0 )
	{
		printf( "Failed to initialise display...\n" );
		return 1;
	}

	for( i = 0L; i < ( 320L*200L ); ++i )
	{
		DrawPixel( x, y, Colour );
		++x;

		if( ( x % 320L ) == 0L )
		{
			++y;
			x = 0;
			Colour++;
		}
	}
	UpdateBuffer( );
	getch( );
	EndVideoMode( );
	return 0;
}
