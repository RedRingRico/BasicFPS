#include <dos.h>
#include <malloc.h>
#include <Renderer.h>

/*#const unsigned int INPUT_STATUS_0 = 0x3DA;*/
#define INPUT_STATUS_0 0x3DA

int g_OldMode = 0;

/* The displayed bitmap */
unsigned char far *g_pFrontBuffer;
/* Off-screen second buffer for rendering to */
unsigned char far *g_pBackBuffer;

/*
?
unsigned char far *g_pRenderBuffer[ 2 ];
*/

unsigned int g_ScreenWidth, g_ScreenHeight;
unsigned int g_ScreenSize;


void EnterMode13H( void )
{
	union REGS In, Out;

	/* Get the old video mode for later */
	In.h.ah = 0xF;
	int86( 0x10, &In, &Out );
	g_OldMode = Out.h.al;

	/* Enter Mode 0x13 */
	In.h.ah = 0;
	In.h.al = 0x13;
	int86( 0x10, &In, &Out );
}

void LeaveMode13H( void )
{
	union REGS In, Out;

	/* Change back to the previous video mode */
	In.h.ah = 0;
	In.h.al = g_OldMode;
	int86( 0x10, &In, &Out );
}


int StartVideoMode( void )
{
	g_pBackBuffer = farmalloc( 64000u );

	if( g_pBackBuffer )
	{
		g_pFrontBuffer = MK_FP( 0xA000, 0 );
		g_ScreenWidth = 320;
		g_ScreenHeight = 200;
		g_ScreenSize = 64000u;
		EnterMode13H( );
		_fmemset( g_pBackBuffer, 0, g_ScreenSize );
		return 0;
	}
	else
	{
		/* No memory available =/ */
		if( g_OldMode )
		{
			LeaveMode13H( );
		}

		printf( "No more memory in hell [DOS]\n" );
		return 1;
	}
}

void EndVideoMode( void )
{
	LeaveMode13H( );
	if( g_pBackBuffer )
	{
		farfree( g_pBackBuffer );
	}
	if( g_pFrontBuffer )
	{
		farfree( g_pFrontBuffer );
	}
}

void UpdateBuffer( void )
{
	/* Wait for the V-retrace */
	while( inportb( INPUT_STATUS_0 ) & 8 )
	;
	while( !( inportb( INPUT_STATUS_0 ) & 8 ) )
	;

	/* Copy everything to video memory */
	_fmemcpy( g_pFrontBuffer, g_pBackBuffer, g_ScreenSize );
}

void DrawPixel( const int p_X, const int p_Y, const int p_Colour )
{
	*( g_pBackBuffer + p_Y*g_ScreenWidth + p_X ) = p_Colour;
}

int GetPixel( const int p_X, const int p_Y )
{
	return *( g_pBackBuffer + p_Y*g_ScreenWidth + p_X );
}
