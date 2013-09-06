#ifndef __BASICFPS_RENDERER_HPP__
#define __BASICFPS_RENDERER_HPP__

#include <stdint.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glext.h>
#include <GL/glxext.h>

const uint32_t	BPFS_COLOUR_BIT		= 0x00000001;
const uint32_t	BFPS_DEPTH_BIT		= 0x00000002;
const uint32_t	BFPS_STENCIL_BIT	= 0x00000004;

const uint32_t BFPS_WINDOW_STYLE_ALL			= 0x00000000;
const uint32_t BFPS_WINDOW_STYLE_MINIMISE		= 0x00000001;
const uint32_t BFPS_WINDOW_STYLE_MAXIMISE		= 0x00000002;
const uint32_t BFPS_WINDOW_STYLE_CLOSE			= 0x00000004;
const uint32_t BFPS_WINDOW_STYLE_RESIZE			= 0x00000008;
const uint32_t BFPS_WINDOW_STYLE_TITLEBAR		= 0x00000010;
const uint32_t BFPS_WINDOW_STYLE_MENU			= 0x00000020;
const uint32_t BFPS_WINDOW_STYLE_BORDER			= 0x00000040;
const uint32_t BFPS_WINDOW_STYLE_MOVE			= 0x00000080;
const uint32_t BFPS_WINDOW_STYLE_FULLSCREEN		= 0x00000100;
const uint32_t BFPS_WINDOW_STYLE_NONE			= 0x80000000;


namespace BasicFPS
{
	class Renderer
	{
	public:
		Renderer( );
		~Renderer( );

		uint32_t Initialise( const int p_X, const int p_Y, const int p_Width,
			const int p_Height, const uint32_t p_Style );

		void ProcessMessages( );

		void ClearColour( const char p_Red, const char p_Blue,
			const char p_Green );

		void Clear( const uint32_t p_Flags );

		void Swap( );

		void ToggleFullscreen( );

		inline void Width( const uint32_t p_Width ){ m_Width = p_Width; }
		inline void Height( const uint32_t p_Height ){ m_Height = p_Height; }

	private:
		void CreateBuffers( );
		void DeleteBuffers( );

		int			m_X;
		int			m_Y;
		int			m_Width;
		int			m_Height;
		char		**m_ppBuffer;
		uint32_t	m_BackBufferCount;
		uint32_t	m_ActiveBuffer;

		bool		m_Fullscreen;
		bool		m_Running;

		char		m_ClearColour[ 3 ];

		Display		*m_pXDisplay;
		Window		m_XWindow;
		XVisualInfo	*m_pXVisualInfo;
		GLXContext	m_GLXContext;

		float		m_Near;
		float		m_Far;
	};
}

#endif

