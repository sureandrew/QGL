#include "common.h"
#include "GDFStream.h"
#include "GDFFile.h"

GDFStream::GDFStream(GDFFile* gdffile, UInt32 blockIndex, UInt32 size, BOOL IsMapView)
{
	TRACE_ENSURE(gdffile);
    m_gdffile = gdffile;

	m_sd.m_blockIndex			= blockIndex;
	m_sd.m_managerBlockOffset	= 0;
	m_sd.m_fileLength			= size;

	m_BeginOffset	= 0;
	m_FileSize		= size;
	m_EndOffset		= m_BeginOffset + size;
	m_CurPos		= 0;
	m_ViewPos		= 0;

	m_buf			= NULL;

	m_mode = GDFFILE_STATE_GAME;
	if( !IsMapView )
		m_mode = GDFFILE_STATE_PATCH_READ;
}

GDFStream::GDFStream(BYTE* mapView, UInt32 fileSize)
{
	m_FileSize	= fileSize;
	mapView		+= SECTOR_OFFSET;
	m_buf		= mapView;
	m_mode		= GDFFILE_STATE_GAME;
}

GDFStream::~GDFStream()
{
	switch( m_mode )
	{
	case GDFFILE_STATE_GAME:
		if( m_buf )
			UnmapViewOfFile(m_buf);
		break;
	}
}

BOOL GDFStream::Read(BYTE* buf, UInt32 readlen, UInt32& outlen, UInt32 maxLength)
{
	switch( m_mode )
	{
	case GDFFILE_STATE_MASTER_WRITE:
	case GDFFILE_STATE_PATCH_READ:
		{
			if( !m_gdffile->ReadStream(&m_sd, buf, readlen, outlen, maxLength) )
			{
				outlen = 0;
				return FALSE;
			}
			m_CurPos += outlen;
			return TRUE;
		}
		break;
	case GDFFILE_STATE_GAME:
		{
			if( maxLength==0 ) return TRUE;

			outlen				= 0;
			UInt32 readlength;
			UInt32 bufpos		= 0;
			while( bufpos < readlen )
			{
				if((m_buf==NULL || m_ViewPos==m_EndOffset) && bufpos!=readlen)
				{
					if( m_buf )
						UnmapViewOfFile(m_buf);
					m_buf		= (BYTE*)m_gdffile->MoveNextMapView(&m_sd, m_BeginOffset, m_EndOffset, m_CurPos);	// &m_BeginOffset, &m_EndOffset should be < 65536 (MAX_BLOCK_SIZE)
					m_ViewPos	= m_BeginOffset;
				}
				if( m_buf )
				{
					readlength = min(readlen-bufpos, m_EndOffset-m_ViewPos);
					memcpy(&buf[bufpos], &m_buf[m_ViewPos], readlength);
					m_ViewPos	+= readlength;
					m_CurPos	+= readlength;
					bufpos		+= readlength;
					outlen		+= readlength;
				}else
				{
					return TRUE;
				}
			}
			return TRUE;
		}
		break;
	}
	return FALSE;
}

VOID GDFStream::MoveFirst()
{
	switch( m_mode )
	{
	case GDFFILE_STATE_MASTER_WRITE:
	case GDFFILE_STATE_PATCH_READ:
		{
			m_CurPos = 0;
		}
		break;
	case GDFFILE_STATE_GAME:
		{
			if( m_sd.m_blockIndex/MEM_BLOCK_SIZE != m_CurPos/MEM_BLOCK_SIZE )
				UnmapViewOfFile(m_buf);
			else
				m_ViewPos = m_BeginOffset;
			m_CurPos = 0;
		}
		break;
	}
}

VOID GDFStream::MovePos(GDFPos relativePos, UInt32 pos)
{
	switch( m_mode )
	{
	case GDFFILE_STATE_MASTER_WRITE:
	case GDFFILE_STATE_PATCH_READ:
		{
			switch( relativePos )
			{
			case GDF_BEGIN:
				m_CurPos = m_BeginOffset + pos;
				m_sd.m_managerBlockOffset = 4 + pos;
				break;
			case GDF_CURRENT:
				m_CurPos += pos;
				if( m_CurPos > m_EndOffset ) m_CurPos = m_EndOffset;
				m_sd.m_managerBlockOffset += pos;
				break;
			case GDF_END:
				m_CurPos = m_EndOffset;
				m_sd.m_managerBlockOffset = 4 + m_FileSize;
				break;
			}
		}
		break;
	case GDFFILE_STATE_GAME:
		{
			switch( relativePos )
			{
			case GDF_BEGIN:
				m_CurPos = 0 + pos;
				break;
			case GDF_CURRENT:
				if( (m_CurPos + pos)/MEM_BLOCK_SIZE != m_CurPos/MEM_BLOCK_SIZE )
					UnmapViewOfFile(m_buf);
				else
					m_ViewPos = m_CurPos + pos;
				m_CurPos += pos;
				if( m_CurPos > m_FileSize ) m_CurPos = m_FileSize;
				break;
			case GDF_END:
				MoveLast();
				break;
			}
		}
		break;
	}
}

VOID GDFStream::MoveLast()
{
	switch( m_mode )
	{
	case GDFFILE_STATE_MASTER_WRITE:
	case GDFFILE_STATE_PATCH_READ:
		{
			m_CurPos = m_EndOffset;
		}
		break;
	case GDFFILE_STATE_GAME:
		{
			m_CurPos = m_EndOffset;
		}
		break;
	}

}

BOOL GDFStream::IsEnd()
{
	return (m_CurPos == m_FileSize);
}

BOOL GDFStream::IsBegin()
{
	return (m_CurPos == 0);
}
