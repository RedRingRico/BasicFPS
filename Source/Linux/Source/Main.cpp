#include <ncurses.h>
#include <Renderer.hpp>
#include <unistd.h>

int main( int p_Argc, char **p_ppArgv )
{
	BasicFPS::Renderer Basic;
	Basic.Initialise( 0, 0, 100, 100, 0 );

	Basic.ProcessMessages( );

	return 0;
}

