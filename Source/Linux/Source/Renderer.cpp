#include <Renderer.hpp>
#include <unistd.h>
#include <stdio.h>

#define MWM_HINTS_FUNCTIONS		( 1L << 0 )
#define MWM_HINTS_DECORATIONS	( 1L << 1 )
#define MWM_HINTS_INPUT_MODE	( 1L << 2 )
#define MWM_HINTS_STATUS		( 1L << 3 )

#define MWM_FUNC_NONE			0L
#define MWM_FUNC_ALL			( 1L << 0 )
#define MWM_FUNC_RESIZE			( 1L << 1 )
#define MWM_FUNC_MOVE			( 1L << 2 )
#define MWM_FUNC_MINIMISE		( 1L << 3 )
#define MWM_FUNC_MAXIMISE		( 1L << 4 )
#define MWM_FUNC_CLOSE			( 1L << 5 )

#define MWM_DECOR_NONE			0L
#define MWM_DECOR_ALL			( 1L << 0 )
#define MWM_DECOR_BORDER		( 1L << 1 )
#define MWM_RESIZEH				( 1L << 2 )
#define MWM_TITLE				( 1L << 3 )
#define MWM_MENU				( 1L << 4 )
#define MWM_MINIMISE			( 1L << 5 )
#define MWM_MAXIMISE			( 1L << 6 )

namespace BasicFPS
{
	Renderer::Renderer( )
	{
		m_ppBuffer = nullptr;
		m_X = 0;
		m_Y = 0;
		m_Width = 0;
		m_Height = 0;
		m_BackBufferCount = 0;
		m_ActiveBuffer = 0;
		m_Fullscreen = false;
	}

	Renderer::~Renderer( )
	{
		this->DeleteBuffers( );
		if( m_pXDisplay )
		{
			XCloseDisplay( m_pXDisplay );
		}
	}

	uint32_t Renderer::Initialise( const int p_X, const int p_Y,
		const int p_Width, const int p_Height, const uint32_t p_Style )
	{
		m_pXDisplay = XOpenDisplay( "" );
		if( !m_pXDisplay )
		{
			printf( "Failed to open X display\n" );

			return 0;
		}

		int VisualAttributes [ ] =
		{
			GLX_X_RENDERABLE,	True,
			GLX_DRAWABLE_TYPE,	GLX_WINDOW_BIT,
			GLX_RENDER_TYPE,	GLX_RGBA_BIT,
			GLX_X_VISUAL_TYPE,	GLX_TRUE_COLOR,
			GLX_RED_SIZE,		8,
			GLX_GREEN_SIZE,		8,
			GLX_BLUE_SIZE,		8,
			GLX_ALPHA_SIZE,		8,
			None
		};

		int GLXMajor = 0, GLXMinor = 0;

		if( !glXQueryVersion( m_pXDisplay, &GLXMajor, &GLXMinor ) ||
			( ( GLXMajor  == 1 ) && (GLXMinor < 3 ) ) || ( GLXMajor < 1 ) )
		{
			printf( "Failed to get a GLX display\n" );
			return 0;
		}

		int FBCount = 0;

		GLXFBConfig *pFBC = glXChooseFBConfig( m_pXDisplay,
			DefaultScreen( m_pXDisplay ), VisualAttributes, &FBCount );

		if( !pFBC )
		{
			printf( "Failed to get a framebuffer configuration\n" );
			return 0;
		}

		GLXFBConfig FBConfig = pFBC[ 0 ];

		XFree( pFBC );
		pFBC = nullptr;

		m_pXVisualInfo = glXGetVisualFromFBConfig( m_pXDisplay, FBConfig );

		XSetWindowAttributes WindowAttributes;

		WindowAttributes.colormap = XCreateColormap( m_pXDisplay,
			RootWindow( m_pXDisplay, m_pXVisualInfo->screen ),
			m_pXVisualInfo->visual, AllocNone );
		WindowAttributes.background_pixmap = None;
		WindowAttributes.border_pixel = 0;
		WindowAttributes.event_mask = StructureNotifyMask | ExposureMask |
			KeyPressMask | KeyReleaseMask |
			ButtonPressMask | ButtonReleaseMask |
			ResizeRedirectMask | PointerMotionMask |
			FocusChangeMask | EnterWindowMask | LeaveWindowMask;

		m_Fullscreen = false;
		if( p_Style & BFPS_WINDOW_STYLE_FULLSCREEN )
		{
			m_Fullscreen = true;
		}
		else
		{
			m_X = p_X;
			m_Y = p_Y;
			m_Width = p_Width;
			m_Height = p_Height;
		}

		m_XWindow = XCreateWindow( m_pXDisplay,
			RootWindow( m_pXDisplay, m_pXVisualInfo->screen ),
			m_X, m_Y, m_Width, m_Height, 0,
			m_pXVisualInfo->depth, InputOutput, m_pXVisualInfo->visual,
			CWEventMask | CWColormap | CWBorderPixel, &WindowAttributes );

		Atom DeleteMessage = XInternAtom( m_pXDisplay, "WM_DELETE_WINDOW",
			False );
		XSetWMProtocols( m_pXDisplay, m_XWindow, &DeleteMessage, 1 );

		Atom Property = XInternAtom( m_pXDisplay, "_MOTIF_WM_HINTS", False );

		if( Property )
		{
		}
		else
		{
			printf( "Could not get window property \"_WM_MOTIF_HINTS\"\n" );
		}

		XMapWindow( m_pXDisplay, m_XWindow );
		XMapRaised( m_pXDisplay, m_XWindow );
		XMoveWindow( m_pXDisplay, m_XWindow, m_X, m_Y );
		XRaiseWindow( m_pXDisplay, m_XWindow );

		m_Running = true;

		return 1;
	}

	void Renderer::ProcessMessages( )
	{
		while( m_Running )
		{
			XEvent Event;
			int Pending = XPending( m_pXDisplay );

			for( int i = 0; i < Pending; ++i )
			{
				XNextEvent( m_pXDisplay, &Event );
				switch( Event.type )
				{
					case ClientMessage:
					{	
						if( *XGetAtomName( m_pXDisplay,
							Event.xclient.message_type ) == *"WM_PROTOCOLS" )
						{
							m_Running = false;
						}
						break;
					}
					default:
					{
						break;
					}
				}
			}
		}
	}

	void Renderer::ClearColour( const char p_Red, const char p_Green,
		const char p_Blue )
	{
	}

	void Renderer::Clear( const uint32_t p_Flags )
	{
	}

	void Renderer::Swap( )
	{
	}

	void Renderer::CreateBuffers( )
	{
		this->DeleteBuffers( );

		if( m_BackBufferCount == 0 )
		{
			m_ppBuffer = new char*[ 1 ];
			m_ppBuffer[ 0 ] = new char[ m_Width*m_Height ];
		}
		else
		{
			m_ppBuffer = new char*[ m_BackBufferCount ];
			for( size_t i = 0; i < m_BackBufferCount; ++i )
			{
				m_ppBuffer[ i ] = new char[ m_Width*m_Height ];
			}
		}
	}

	void Renderer::DeleteBuffers( )
	{
		if( m_ppBuffer )
		{
			for( size_t i = 0; i < m_BackBufferCount; ++i )
			{
				delete [ ] m_ppBuffer[ i ];
				m_ppBuffer[ i ] = nullptr;
			}
			delete [ ] m_ppBuffer;
			m_ppBuffer = nullptr;
		}
	}
}

