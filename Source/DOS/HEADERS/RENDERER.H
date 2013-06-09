#ifndef __PHANTASY_RENDERER_H__
#define __PHANTASY_RENDERER_H__


/* Set up and take down Mode 0x13 */
void EnterMode13H( void );
void LeaveMode13H( void );

/* Set up the renderer's canvas */
int StartVideoMode( void );
/* Clean up the mess made by the renderer */
void EndVideoMode( void );
/* Copy the back buffer to the front */
void UpdateBuffer( void );

/* Set a pixel */
void DrawPixel( const int p_X, const int p_Y, const int p_Colour );

/* Get a pixel */
int GetPixel( const int p_X, const int p_Y );


#endif
