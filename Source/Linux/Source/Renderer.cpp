#include <Renderer.hpp>
#include <unistd.h>

namespace BasicFPS
{
	Renderer::Renderer( )
	{
		m_ppBuffer = nullptr;
		m_Width = 0;
		m_Height = 0;
		m_BackBufferCount = 0;
		m_ActiveBuffer = 0;
	}

	Renderer::~Renderer( )
	{
		this->DeleteBuffers( );
	}

	uint32_t Renderer::Initialise( const uint32_t p_Width,
		const uint32_t p_Height )
	{
		return 0;
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

