/*
 * 
* Copyright (C) 2004-2006  Autodesk, Inc.
* 
* This library is free software; you can redistribute it and/or
* modify it under the terms of version 2.1 of the GNU Lesser
* General Public License as published by the Free Software Foundation.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
* 
*/


#include <FdoCommon.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <locale.h>
#include <ctype.h>
#include <wctype.h>
#include "utf8_.h"

FdoStringP::Tab FdoStringP::tab[] =
{
  0x80,  0x00,   0*6,    0x7F,           0,              /* 1 byte sequence */
  0xE0,  0xC0,   1*6,    0x7FF,          0x80,           /* 2 byte sequence */
  0xF0,  0xE0,   2*6,    0xFFFF,         0x800,          /* 3 byte sequence */
  0xF8,  0xF0,   3*6,    0x1FFFFF,       0x10000,        /* 4 byte sequence */
  0xFC,  0xF8,   4*6,    0x3FFFFFF,      0x200000,       /* 5 byte sequence */
  0xFE,  0xFC,   5*6,    0x7FFFFFFF,     0x4000000,      /* 6 byte sequence */
  0,                                                     /* end of table    */
};

const wchar_t* FdoStringP::mEmptyString = L"";

static const long* GetAttachedRefCount()
{
    static const long attachedRefCount = -1;
    return &attachedRefCount;
}

static const wchar_t* GetEmptyString()
{
    static const wchar_t* EmptyString = L"";
    return EmptyString;
}

FdoStringP::FdoStringP(void) :
	mwString(NULL),
	msString(NULL),
	mBuffer(NULL)
{
	SetString(GetEmptyString(), true);
}


FdoStringP::FdoStringP( FdoString* wValue, FdoBoolean bAttach ) :
	mwString(NULL),
	msString(NULL),
	mBuffer(NULL)
{
	SetString(wValue, bAttach);
}

FdoStringP::FdoStringP( const char* sValue) :
	mwString(NULL),
	msString(NULL),
	mBuffer(NULL)
{
	SetString(sValue);
}

FdoStringP::FdoStringP(const FdoStringP& oValue) :
	mwString(NULL),
	msString(NULL),
	mBuffer(NULL)
{
	SetString(oValue);
}

FdoStringP::~FdoStringP(void)
{
	Release();
}

FdoStringP& FdoStringP::operator=( const FdoStringP& oString )
{
	SetString( oString );

	return(*this);
}

FdoStringP& FdoStringP::operator=( const char* sString )
{
	SetString( sString );

	return(*this);
}

FdoStringP& FdoStringP::operator=( FdoString* wString )
{
	SetString( wString );

	return(*this);
}

const FdoStringP FdoStringP::operator+( FdoString* str2 ) const
{
    FdoString*  values[3];

    values[0] = mwString;
    values[1] = str2;
    values[2] = NULL;

    return(FdoStringP(values));
}

FdoStringP  FdoStringP::operator+=( FdoString* str2 )
{
    // nothing to do if second string null.
    if ( str2 ) {
        size_t len1 = wcslen( mwString );
        size_t len2 = wcslen( str2 );

        if ( mBuffer && ((len1 + len2) <= mBuffer->mBufSize) ) {
            // This object has an allocated buffer that is big enough to hold the result,
            // simply append the second string.
            memcpy((void*)(mwString + len1), (void*) str2, (len2 + 1 ) * sizeof(wchar_t) );
        }
        else {
            // Otherwise delegate to "+" operator, which will allocate new buffer.
            (*this) = (*this) + str2;
        }
    }

    return( *this );
}

int FdoStringP::ICompare( const FdoStringP str2 ) const
{
#ifdef _WIN32
    return( _wcsicmp(mwString, str2) );
#else
	return(	wcscasecmp(mwString, str2) ); 
#endif
}


FdoStringP::operator FdoString*( ) const
{
	// when const, it is "safe" to return the internal buffer
	return( mwString );
}

wchar_t* FdoStringP::_copyAsWChar( ) const
{
	// when not const, return a copy of the buffer

	wchar_t* wString = new wchar_t[wcslen(mwString)+1];

	wcscpy( wString, mwString );

	return(wString);
}

FdoStringP::operator const char*( ) const
{
	SetSingle();

	// return the utf8 buffer
	return(msString);
}

char* FdoStringP::_copyAsChar( ) const
{
    size_t utf8Len = wcslen(mwString) * 6 + 1;
	// allocate a wide string that is big enough 
	// In UTF8 the maximum size for a character is 6 bytes.
	char* sString = new char[utf8Len];

    // convert to wide
	Utf8FromUnicode( mwString, sString, utf8Len );

	// not const so allocate and return a utf8 version of the string
	return( sString );
}

size_t FdoStringP::GetLength() const
{
	return( wcslen(mwString) );
}

FdoStringP FdoStringP::Left( FdoString* delimiter ) const
{
	wchar_t* workString = _copyAsWChar();
	wchar_t* delimStart = NULL;
	FdoStringP leftString;

    delimStart = wcsstr( workString, delimiter ? delimiter : L"" );

	if ( delimStart)
		(*delimStart) = (wchar_t) 0;

	leftString = workString;

	delete[] workString;

	return(leftString);
}

FdoStringP FdoStringP::Right( FdoString* delimiter ) const
{
	wchar_t* workString = _copyAsWChar();
	wchar_t* delimStart = NULL;
	FdoStringP rightString;
	
    delimStart = wcsstr( workString, delimiter ? delimiter : L"" );

	if ( delimStart) {
        wchar_t* delimEnd = delimStart + (delimiter ? wcslen(delimiter) : 0);
		rightString = delimEnd;
	}

	delete[] workString;

	return(rightString);
}

FdoStringP FdoStringP::Mid( size_t first, size_t count, bool useUTF8 )
{
	FdoStringP midString;

	if ( first < 0 ) 
		first = 0;

	if ( useUTF8 ) {
		char* workString = _copyAsChar();

		// negative count means get everything after the first character.
		if ( count < 0 ) 
			count = strlen(workString);

		size_t end = first + count;

		// truncate if not getting the last character.
		if ( end < strlen(workString) )
			workString[first+count] = (char) 0;

		// extract the mid string.
		midString = FdoStringP( &workString[first] );

		delete [] workString;
	}
	else {
		wchar_t* workString = _copyAsWChar();

		// negative count means get everything after the first character.
		if ( count < 0 ) 
			count = wcslen(workString);

		size_t end = first + count;

		// truncate if not getting the last character.
		if ( end < wcslen(workString) )
			workString[first+count] = (wchar_t) 0;

		// extract the mid string.
		midString = FdoStringP( &workString[first] );

		delete [] workString;
	}

	return( midString );
}

FdoStringP FdoStringP::Replace( FdoString* pOld, FdoString* pNew ) const
{
	FdoString* oldString = pOld ? pOld : L"";
	FdoString* newString = pNew ? pNew : L"";

	// Get the string sizes
	size_t stringSize = wcslen(mwString);
	size_t oldSize = wcslen(oldString);
	size_t newSize = wcslen(newString);

	// Nothing to do if no string to replace was provided.
	if ( oldSize == 0 ) 
		return(*this);

	size_t sizeDiff = (newSize > oldSize) ? (newSize - oldSize) : 0;

	// The maximum size of the new string is 
	// (max possible occurences of the old substring ) * 
	// (difference between old and new substrings).
	size_t workSize = stringSize + ((stringSize / oldSize) * sizeDiff) + 1;

	// Set up workstring for holding the new string.
	wchar_t* workString = new wchar_t[workSize+1];

	// Set up pointers to old and new strings.
	wchar_t* inp1 = mwString;
	wchar_t* outp = workString;

	// Find first occurrence of the old substring.
	wchar_t* inp2 = wcsstr( inp1, oldString );

	while ( inp2 ) {
		// copy everything up to the old substring
		wcsncpy( outp, inp1, inp2 - inp1 );
		outp += (inp2 - inp1);

		// Perform the replace, append new substring.
		wcscpy( outp, newString );
		outp += newSize;

		// Move the old string pointer to just past the old substring.
		inp1 = inp2 + oldSize;

		// Find the next occurence of the old substring
		inp2 = wcsstr( inp1, oldString );
	}

	// append everything after the last old substring.
	wcscpy( outp, inp1 );

	FdoStringP outString(workString);

	delete [] workString;

	return outString;
}

FdoStringP FdoStringP::Upper() const
{ 
	wchar_t* workString = _copyAsWChar();

#ifdef _WIN32	
	FdoStringP outString = _wcsupr(workString);
#else
        wint_t  temp;
        for (int i=0;i<wcslen(workString); i++)
        {
                temp = towupper(workString[i]);
                workString[i] = temp;
        }
	FdoStringP outString = workString;
#endif

	delete[] workString;

	return outString;
}

FdoStringP FdoStringP::Lower() const
{
	wchar_t* workString = _copyAsWChar();

#ifdef _WIN32
	FdoStringP outString = _wcslwr(workString);
#else
        wint_t  temp;
        for (int i=0;i<wcslen(workString); i++)
        {
                temp = towlower(workString[i]);
                workString[i] = temp;
        }
	FdoStringP outString = workString;
#endif

	delete[] workString;

	return outString;
}

bool FdoStringP::Contains( FdoString* subString ) const
{
    return( wcsstr( mwString, subString ? subString : L"" ) != NULL );
}

bool FdoStringP::IsNumber() const
{
	SetSingle();

    register bool radix_found;
    register bool digits_found;
    char    new_str[1000];
    unsigned char    *ns;
    char    radix = '\0';
    struct lconv *nls;

    if (msString == '\0')
        return false;

    
    nls = localeconv();
    if (nls)
        radix = nls->decimal_point[0];

    if (radix == '\0')
        radix = '.';

//    if (strlen(msString) < (size_t) WORK_BUF)
//        ut_strstrip(string, new_str);
//    else
//        new_str[0] = '\0';
      strcpy( new_str, msString );
    ns = (unsigned char*) new_str;
    if (*ns == '+' || *ns == '-')
        ns++;

    for (radix_found = digits_found = false; *ns != 0; ns++) {
        if (*ns == '-' || *ns == '+') {
            return false;
        }
        else if (*ns == ((unsigned char)radix)) {
            if (radix_found)
                return false;
            radix_found = true;
        }
        else if (!isdigit(*ns)) {
            return false;
        }
        else {
            digits_found = true;
        }
    }

    return(digits_found);
}

long FdoStringP::ToLong() const
{
    long outVal = 0;

    // Try converting with assumption that string is in decimal format.
    // Performancewise, this function favours decimal strings over hexadecimal strings.
#ifdef _WIN32
	outVal = _wtol( mwString );
#else
	outVal = wcstol(mwString, NULL, 10 );
#endif

    // Check if string is hexadecimal. For performance, skip check for "0x" if decimal
    // conversion was successful.
    if ( (outVal == 0) && (wcscmp(mwString,L"0") != 0) && (Contains(L"0x") || Contains(L"0X")) ) {
        FdoString* str = mwString;
        
        // Reliant and earlier versions of FDO serialized byte values as "\0x...". Support
        // these strings by skipping over any leading \.
        if ( str[0] == '\\' ) 
            str++;

        // Do conversion from hex string.
        swscanf( str, L"%x", &outVal );
    }

    return outVal;
}

FdoDouble FdoStringP::ToDouble() const
{
#ifdef _WIN32
	return ( _wtof(mwString) );
#else
	return ( atof((const char*) (*this)) );
#endif
}

FdoBoolean FdoStringP::ToBoolean( FdoBoolean defaultValue ) const
{
    FdoStringP lower = this->Lower();

    if ( lower == L"true" || lower == L"t" || lower == L"yes" || lower == L"y" || lower == L"1" )
        return(true);

    if ( lower == L"false" || lower == L"f" || lower == L"no" || lower == L"n" || lower == L"0" )
        return(false);

    return(defaultValue);
}

FdoStringP FdoStringP::Format( FdoString* wValue, ... )
{
	// In the context of schemas, NULL and empty string are the same thing.
	if ( wValue == NULL ) {
		return FdoStringP();
	}

	va_list argList;
	va_start(argList, wValue);

	// format the string. Start with just enough space
	// to hold the format string. This ensures we
	// use minimal space when there are no formatting parms.
	size_t formattedSize = wcslen(wValue) + 2;
	wchar_t* wFormattedValue = new wchar_t[formattedSize];
	
#ifdef _WIN32
	while ( _vsnwprintf( wFormattedValue, formattedSize - 1, wValue, argList ) < 0 ) {
#else
	while ( vswprintf( wFormattedValue, formattedSize - 1, wValue, argList ) < 0 ) {
#endif
		// buffer wasn't big enough, enlarge it and retry 
		// the formatting.
		formattedSize = formattedSize * 2;
		delete [] wFormattedValue;
		wFormattedValue = new wchar_t[formattedSize];
		va_end(argList);
		va_start(argList, wValue);
	}

	wFormattedValue[formattedSize - 1] = '\0';
	va_end(argList);
	
	FdoStringP outString(wFormattedValue);
	delete [] wFormattedValue;

	return( outString );
}

int FdoStringP::Utf8FromUnicode(
    const wchar_t   *       Wtext,
	int         in_size,
    char    *       str_out,
    int         out_max_size,
	bool		thrown_exception )
{
    int ret = ut_utf8_from_unicode( Wtext, in_size, str_out, out_max_size );
	if( thrown_exception && ret == -1 )
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_28_UTF8FAILURE),
                Wtext
            )
        );

	return ret;
}

int FdoStringP::Utf8ToUnicode(
        const char  *   str_in,
		int         in_size,
        wchar_t *   Wtext,
        int         out_size,
		bool		thrown_exception )
{
	int ret = ut_utf8_to_unicode( str_in, in_size, Wtext, out_size );
	if( thrown_exception && ret == -1 )
        throw FdoException::Create(
            FdoException::NLSGetMessage(
                FDO_NLSID(FDO_29_UNICODEFAILURE),
                str_in
            )
        );

	return ret;
}

int FdoStringP::Utf8ToUnicode(
        const char  *   str_in,
        wchar_t *   Wtext,
        int         out_size,
		bool		thrown_exception )
{
	return Utf8ToUnicode( str_in, strlen(str_in), Wtext, out_size, thrown_exception );
}

int FdoStringP::Utf8FromUnicode(
    const wchar_t   *       Wtext,
    char    *       str_out,
    int         out_max_size,
	bool		thrown_exception )
{
    return Utf8FromUnicode( Wtext, wcslen( Wtext), str_out, out_max_size, thrown_exception );
}

int FdoStringP::Utf8Len(const char *utf8String)
{
    long wChar;
    int cFirst, cUtf8, nCount;
    Tab *table_entry;

    if ( utf8String == 0 )
        return 0;

    nCount = 0;
    while (*utf8String)
    {
        cFirst = *utf8String & 0xff;
        wChar = cFirst;

        for ( table_entry = tab; table_entry->cmask; table_entry++ )
        {
            if ( ( cFirst & table_entry->cmask ) == table_entry->cval )
            {
                wChar &= table_entry->lmask;
                if ( wChar < table_entry->lval )
                    return -1; //error
                break;
            }

            utf8String++;
            cUtf8 = ( *utf8String ^ 0x80 ) & 0xFF;
            if ( cUtf8 & 0xC0 )
                return -1; //error
            wChar = ( wChar << 6 ) | cUtf8;
        }
        utf8String++;
        nCount++;
    }
    return nCount;
}

void FdoStringP::SetString(FdoString* wValue, FdoBoolean bAttach)
{
	// Make NULL and empty strings equivalent
	FdoString* workString = wValue ? wValue : L"";

    if ( bAttach ) {
	    // release the current buffer.
	    Release();

        mwString = (wchar_t*) workString;
    }
    else {
        // Nothing to do if identity assignment (assigning this object to it's own string).
        if ( (!mBuffer) || (mwString != workString) ) {
    	    // Create a new buffer from new value
            size_t len = wcslen(workString);
	        AllocateBuffer( len );
	        memcpy( (void*)mwString, (void*)workString, sizeof(wchar_t) * (len + 1) );
        }
    }
}

void FdoStringP::SetString(const char* sValue) 
{
	// allocate a wide string that is big enough
	wchar_t* wValue = NULL;
    
    if ( sValue ) {
        wValue = new wchar_t[strlen(sValue) + 1];
    	// convert to wide
	    Utf8ToUnicode( sValue, wValue, strlen(sValue) + 1 );
    }

	SetString(wValue);

	if ( wValue ) 
		delete [] wValue;
}

void FdoStringP::SetString(const FdoStringP& oValue)
{
    // Nothing to do if assigning object to itself.
    if ( this != &oValue ) {
	    // release current value
	    Release();

	    // point to the buffer and ref counter for input object.
	    // This allows these string objects to share buffers when they
	    // have the same values.
	    mBuffer = oValue.mBuffer;
	    mwString = oValue.mwString;

	    // bump up the ref count.
	    AddRef();
    }
}

void FdoStringP::SetString( FdoString** values )
{
    size_t totalLength = 0;
    int idx;


    // Create buffer big enough to hold concatenated strings.
    for ( idx = 0; values[idx] != NULL; idx++ ) 
        totalLength += wcslen(values[idx]);

    AllocateBuffer( totalLength );

    // Concatenate the strings into this object's buffer
    totalLength = 0;
    for ( idx = 0; values[idx] != NULL; idx++ ) {
        size_t curLength = wcslen(values[idx]);
        memcpy( (void*)&(mwString[totalLength]), (void*) values[idx], curLength * sizeof(wchar_t) );
        totalLength += curLength;
    }

    mwString[totalLength] = 0;
}



void FdoStringP::SetSingle( ) const
{
	// if first time, create a buffer for the utf8 version of the string.
	if ( msString == NULL )
		((FdoStringP*) this)->msString = _copyAsChar();
}

void FdoStringP::AddRef()
{
    if ( (GetRefCount()) >= 0 ) 
    	SetRefCount(GetRefCount() + 1);
}

void FdoStringP::Release()
{
	if ( GetRefCount() >= 0 ) {
    	SetRefCount(GetRefCount() - 1);

		if ( (GetRefCount()) == 0 ) {
			// No more references,
			// get rid of ref counter and buffers
			if ( mBuffer )
				free((void*) mBuffer);
		}
	}

	// single string not refcount.
	// each object has its own so always free it on release.
	if ( msString )
		delete[] msString;

    mBuffer = NULL;
	mwString = NULL;
	msString = NULL;
}

FdoStringP::FdoStringP( FdoString** values ) :
	mwString(NULL),
	msString(NULL),
	mBuffer(NULL)
{
    SetString( values );
}

void FdoStringP::AllocateBuffer( size_t bufSize )
{
    // Re-use current buffer if big enough and no other FdoStringP is referencing it.
    if ( (!mBuffer) || (GetRefCount() != 1) || (bufSize > GetBufSize()) ) {
        // Otherwise, Release curent buffer and create a new one.
        Release();
        
        // Allow space for number of wide characters, descriptor and null terminator
        mBuffer = (Descriptor*) malloc( sizeof(Descriptor) + (sizeof(wchar_t) * (bufSize + 1)) );
        mwString = (wchar_t*)(mBuffer + 1);

        // Initialize descriptor
        SetRefCount(1);
        SetBufSize(bufSize);
    }
    else {
        // Re-using current buffer, just destroy the current UTF8 string.
	    if ( msString )
		    delete[] msString;

        msString = NULL;
    }
}

void FdoStringP::SetRefCount( long refCount )
{
    if ( mBuffer ) {
        mBuffer->mRefCount = refCount;
    }
}

long FdoStringP::GetRefCount()
{
    if ( mBuffer ) 
        return mBuffer->mRefCount;
    else
        return -1;
}

void FdoStringP::SetBufSize( size_t bufSize )
{
    if ( mBuffer ) {
        mBuffer->mBufSize = bufSize;
    }
}

size_t FdoStringP::GetBufSize()
{
    if ( mBuffer ) 
        return mBuffer->mBufSize;
    else
        return 0;
}
