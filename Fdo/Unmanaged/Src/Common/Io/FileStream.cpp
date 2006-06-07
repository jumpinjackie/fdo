// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include <stdio.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

#include <fcntl.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <Common.h>

FdoIoFileStream::FdoIoFileStream(FdoString* fileName, FdoString* accessModes ) :
    mbMyFp(true),
    mbTextMode(false)
{
    FdoStringP lModes(accessModes);

    // Fdo streams are binary by default, so open in binary mode if no mode 
    // explicitly specified.
    if ( (!lModes.Contains(L"b")) && (!lModes.Contains(L"t")) ) 
	lModes += L"b";

#ifdef _WIN32
    mFp = _wfopen(fileName, lModes);
#else
    //Todo: check if Linux has a wide-char file open.
    //convert to single char for now.

    char* sFileName = new char[wcslen(fileName) * 6 + 1];
    char* sModes = new char[wcslen(fileName) * 6 + 1];

    sprintf( sFileName, "%ls", fileName );
    sprintf( sModes, "%ls", (FdoString*) lModes );
    mFp = fopen( sFileName, sModes );

    delete[] sFileName;
    delete[] sModes;

#endif

    if ( mFp == NULL ) 
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_18_FILEOPENFAILURE),
                fileName,
                accessModes
            )
        );

    InitFileStatus();
}

FdoIoFileStream::FdoIoFileStream(FILE* fp) :
    mbMyFp(false),
    mFp(fp),
    mbTextMode(false)
{
    InitFileStatus();
}

FdoIoFileStream::~FdoIoFileStream(void)
{
    // Close the file pointer of this object opened it.
    if ( mbMyFp && mFp ) 
        fclose(mFp);
}

FdoIoFileStream* FdoIoFileStream::Create(FdoString* fileName, FdoString* accessModes )
{
    if ( !fileName ) 
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_30_BADPARAM),
                L"fileName",
                L"(NULL)",
                L"FdoIoFileStream::Create"
            )
        );

    if ( !accessModes ) 
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_30_BADPARAM),
                L"accessModes",
                L"(NULL)",
                L"FdoIoFileStream::Create"
            )
        );

    return new FdoIoFileStream(fileName, accessModes);
}

FdoIoFileStream* FdoIoFileStream::Create(FILE* fp)
{
    if ( !fp ) 
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_30_BADPARAM),
                L"fp",
                L"(NULL)",
                L"FdoIoFileStream::Create"
            )
        );

    return new FdoIoFileStream(fp);
}

FdoSize FdoIoFileStream::Read( FdoByte* buffer, FdoSize count )
{
    if ( !buffer ) 
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_30_BADPARAM),
                L"buffer",
                L"(NULL)",
                L"FdoIoFileStream::Read"
            )
        );

    if ( !CanRead() ) 
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_19_STREAMREADERROR),
                count
            )
        );
 
    if ( count == 0 ) 
        return(0);

    if ( fflush(mFp) != 0 )
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_37_FILEFLUSH),
                L"FdoIoFileStream::Read"
            )
        );

#ifdef _WIN32
    return _read( mFd, buffer, count );
#else
    return read( mFd, buffer, count );
#endif
}

void FdoIoFileStream::Write( FdoByte* buffer, FdoSize count )
{
    if ( !buffer ) 
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_30_BADPARAM),
                L"buffer",
                L"(NULL)",
                L"FdoIoFileStream::Write"
            )
        );

    if ( !CanWrite() ) 
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_20_STREAMWRITEERROR),
                count
            )
        );

    if ( fflush(mFp) != 0 )
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_37_FILEFLUSH),
                L"FdoIoFileStream::Write"
            )
        );

#ifdef _WIN32
    FdoSize writeCount = _write( mFd, buffer, count );
#else
    FdoSize writeCount = write( mFd, buffer, count );
#endif

    if ( writeCount != count ) 
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_21_STREAMWRITEERROR),
                count,
                writeCount
            )
        );

}

void FdoIoFileStream::Write( FdoIoStream* stream, FdoSize count )
{
    FdoByte buffer[mBufferSize];
    FdoSize readCount = mBufferSize;
    FdoSize bytesLeft = count;

    if ( !stream ) 
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_30_BADPARAM),
                L"stream",
                L"(NULL)",
                L"FdoIoFileStream::Write"
            )
        );

    // Keep reading from the given stream until its end is reached
    // or the maximum bytes to read has been reached.
    while ( (count == 0) ||(bytesLeft > 0) ) {
        readCount = stream->Read( 
            buffer, 
            ((bytesLeft > 0) && (bytesLeft < mBufferSize)) ? bytesLeft : mBufferSize 
        );
        if ( readCount <= 0 ) 
            break;

        Write( buffer, readCount );

        if ( bytesLeft > 0 ) 
            bytesLeft -= readCount;
    }
}

void FdoIoFileStream::SetLength( FdoInt64 length )
{
    CheckContext( L"SetLength" );

    if ( length < GetLength() ) {
#ifdef _WIN32
//Todo: support changing length of large files on Windows.
//Haven't been able to find an int64 version of chsize yet.
        if ( length > LONG_MAX )
            throw FdoException::Create(
                FdoException::NLSGetMessage(
                    FDO_NLSID(FDO_51_SETFILESIZE),
                    length,
                    LONG_MAX
                )
            );

        if ( _chsize(mFd,(FdoSize) length) != 0 )
#else
        if ( ftruncate( mFd, length ) != 0 ) 
#endif
            throw FdoException::Create(
                FdoException::NLSGetMessage(
                    FDO_NLSID(FDO_25_STREAMSIZEFAILURE),
                    length
                )
            );
    }
}

FdoInt64 FdoIoFileStream::GetLength()
{
    CheckContext( L"GetLength" );

    if ( fflush(mFp) != 0 )
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_37_FILEFLUSH),
                L"FdoIoFileStream::GetLength"
            )
        );

#ifdef _WIN32
    FdoInt64 len = _filelengthi64( mFd );
#else
    struct stat fileStat;

    if ( fstat(mFd, &fileStat) != 0 ) 
        throw FdoException::Create(
            FdoException::NLSGetMessage(FDO_NLSID(FDO_24_STREAMBADFP)));


    FdoInt64 len = fileStat.st_size;

#endif

    if ( mbTextMode ) 
        len--;

    return( len );
}

FdoInt64 FdoIoFileStream::GetIndex()
{
    CheckContext( L"GetIndex" );

    if ( fflush(mFp) != 0 )
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_37_FILEFLUSH),
                L"FdoIoFileStream::GetIndex"
            )
        );

#ifdef _WIN32
    FdoInt64 index = _telli64( mFd );
#else
    FdoInt64 index = ftello( mFp );
#endif
    // ftell returns 0 when at the start but
    // a 1-based position when in text mode and not at start. It seems to treat start
    // as being just before the first byte.
    // FdoIoStream always treats start as being the first byte so subtract 1 to adjust.
    
    if ( mbTextMode ) 
        index = (index > 0) ? index - 1 : index;

    return(index);
}

void FdoIoFileStream::Skip( FdoInt64 offset )
{
    CheckContext( L"Skip" );

    if ( fflush(mFp) != 0 )
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_37_FILEFLUSH),
                L"FdoIoFileStream::Skip"
            )
        );

#ifdef _WIN32
    FdoInt64 seekStatus = _lseeki64( mFd, offset, SEEK_CUR);
#else
    FdoInt64 seekStatus = lseek( mFd, offset, SEEK_CUR);
#endif

    if (seekStatus < 0) 
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_23_STREAMSEEKFAILURE)));
}

void FdoIoFileStream::Reset()
{
    CheckContext( L"Reset" );

#ifdef _WIN32
    FdoInt64 seekStatus = _lseeki64( mFd, 0, SEEK_SET);
#else
    FdoInt64 seekStatus = lseek( mFd, 0, SEEK_SET);
#endif

    if (seekStatus < 0) 
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_23_STREAMSEEKFAILURE)));
}

FdoBoolean FdoIoFileStream::CanRead()
{
    return( mbCanRead );
}

FdoBoolean FdoIoFileStream::CanWrite()
{
    return( mbCanWrite );
}

FdoBoolean FdoIoFileStream::HasContext()
{
    return( mbHasContext );
}

void FdoIoFileStream::InitFileStatus()
{
#ifdef _WIN32
    mFd = _fileno(mFp);
#else
    mFd = fileno(mFp);
#endif
    struct stat fileStat;

    if ( fstat(mFd, &fileStat) != 0 ) 
        throw FdoException::Create(
            FdoException::NLSGetMessage(FDO_NLSID(FDO_24_STREAMBADFP)));

#ifdef _WIN32
    struct _iobuf *bufP = mFp;

    // Figure out read/write capabilities
    mbCanRead = (bufP->_flag & (_IOREAD|_IORW)) != 0;
    mbCanWrite = (bufP->_flag & (_IOWRT|_IORW)) != 0;

    //File is contextual only if it is a regular file.
    mbHasContext = ( (fileStat.st_mode & _S_IFREG) != 0 ); 

    // find out if in text mode.
    int oldMode = _setmode( mFd, _O_BINARY );
    if ( oldMode != -1 ) {
        if ( oldMode & _O_TEXT ) {
            mbTextMode = true;
            _setmode(mFd, oldMode);
        }
    }
#else
    _IO_FILE *bufP = mFp;

    mbCanRead = (bufP->_flags & _IO_NO_READS) == 0;
    mbCanWrite = (bufP->_flags & _IO_NO_WRITES) == 0;

    //File is contextual only if it is a regular file.
    mbHasContext = ( (fileStat.st_mode & S_IFREG) != 0 ); 

#endif
}

void FdoIoFileStream::CheckContext( FdoString* caller )
{
    if ( !HasContext() )
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_35_FILENOCONTEXT),
                L"FdoIoFileStream",
                caller
            )
        );
}



