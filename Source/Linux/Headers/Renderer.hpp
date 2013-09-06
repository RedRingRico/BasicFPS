#ifndef __BASICFPS_RENDERER_HPP__
#define __BASICFPS_RENDERER_HPP__

#include <stdint.h>

const uint32_t	BPFS_COLOUR_BIT		= 0x00000001;
const uint32_t	BFPS_DEPTH_BIT		= 0x00000002;
const uint32_t	BFPS_STENCIL_BIT	= 0x00000004;


namespace BasicFPS
{
	class Renderer
	{
	public:
		Renderer( );
		~Renderer( );

		uint32_t Initialise( const uint32_t p_Width, const uint32_t p_Height );

		void ClearColour( const char p_Red, const char p_Blue,
			const char p_Green );

		void Clear( const uint32_t p_Flags );

		void Swap( );

		inline void Width( const uint32_t p_Width ){ m_Width = p_Width; }
		inline void Height( const uint32_t p_Height ){ m_Height = p_Height; }

	private:
		void CreateBuffers( );
		void DeleteBuffers( );

		uint32_t	m_Width;
		uint32_t	m_Height;
		char		**m_ppBuffer;
		uint32_t	m_BackBufferCount;
		uint32_t	m_ActiveBuffer;

		char		m_ClearColour[ 3 ];
	};
}

#endif

