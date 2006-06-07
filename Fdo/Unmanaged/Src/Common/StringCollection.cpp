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
#include <FdoCommon.h>


FdoStringCollection* FdoStringCollection::Create()
{
    return new FdoStringCollection();
}

FdoStringCollection* FdoStringCollection::Create( const FdoStringCollection* src)
{
    return new FdoStringCollection(src);
}

FdoStringCollection* FdoStringCollection::Create( const FdoStringCollection& src)
{
    return new FdoStringCollection(src);
}

FdoStringCollection* FdoStringCollection::Create( const FdoStringP& inString, const wchar_t* delimiters, bool bNullTokens )
{
    return new FdoStringCollection(inString, delimiters, bNullTokens);
}

FdoStringCollection::FdoStringCollection()
{
}

FdoStringCollection::FdoStringCollection( const FdoStringCollection* src)
{
    if ( src ) {
        Append(*src);
    }
}

FdoStringCollection::FdoStringCollection( const FdoStringCollection& src)
{
    Append(src);
}

FdoStringCollection::FdoStringCollection(const FdoStringP& inString, const wchar_t* delimiters, bool bNullTokens )
{
    wchar_t* buffer = new wchar_t[inString.GetLength()+1];
    wcscpy(buffer, (const wchar_t*)inString);

    wchar_t* pToken = buffer;
    FdoSize  stringLen = inString.GetLength();
    FdoSize  delimLen = wcslen( delimiters );

    // The following does a custom tokenize, rather than using wcstok. This 
    // function considers adjacent delimiters to have a zero-length token
    // between them. wcstok does not generate a token between adjacent delimiters.

    // scan the input string for the delimiters
    for ( FdoSize idx = 0; idx < stringLen; idx++ ) {
        // scan the delimiters to see if any match the current input char.
        for ( FdoSize idlm = 0; idlm < delimLen; idlm++ ) {
            if ( buffer[idx] == delimiters[idlm] ) {
                // delimiter found, terminate the current token here.
                buffer[idx] = 0;

                // If the token is zero length, add it only if generating
                // zero-length tokens.
                if ( bNullTokens || (wcslen(pToken) > 0) ) 
                    Add( pToken );

                // Bump up current token start to after current character.
                pToken = &buffer[idx+1];
                break;
            }
        }
    }

    // Handle last token.
    if ( bNullTokens || (wcslen(pToken) > 0) ) 
        Add( pToken );

    delete [] buffer;
}

FdoStringCollection::~FdoStringCollection()
{
}

FdoString* FdoStringCollection::GetString(int index) const
{
    FdoStringElementP elem = FdoCollection<FdoStringElement,FdoException>::GetItem(index);
    return elem->GetString();
}

FdoInt32 FdoStringCollection::IndexOf(FdoStringP value, FdoBoolean caseSensitive) const
{
    FdoInt32    i, index = -1;

    for (i = 0; i < GetCount(); i++) {
        if ( (caseSensitive && (value == FdoStringP(GetString(i)))) ||
             ((!caseSensitive) && (value.ICompare(GetString(i)) == 0)) 
        ) {
            index = i;        
            break;
        }
    }

    return index;
}


void FdoStringCollection::Append( const FdoStringCollection& src)
{
    for ( int i = 0; i < src.GetCount(); i++ ) 
        Add( src.GetString(i) );
}

int FdoStringCollection::Add( FdoStringP string )
{
    FdoStringElementP pElem = FdoStringElement::Create(string);

    return FdoCollection<FdoStringElement,FdoException>::Add(pElem);
}

FdoStringP FdoStringCollection::ToString( const wchar_t* separator )
{
    FdoStringP retVal;
    FdoStringP sep;

    for ( int i = 0; i < GetCount(); i++ ) {
        retVal += sep + GetString(i);
        sep = separator;
    }

    return(retVal);
}

/*
FdoStringCollection& FdoStringCollection::operator=( const FdoStringCollection& src )
{
    Clear();
    Append(src);

    return(*this);
}

FdoStringCollection FdoStringCollection::operator+( const FdoStringCollection& coll2 )
{
    FdoStringsP dest = FdoStringCollection::Create(this);
    dest->Append(coll2);
    
    return(*dest.p);
}

FdoStringCollection FdoStringCollection::operator+( const FdoStringP& str2 )
{
    FdoStringsP dest(this);
    dest->Add(str2);
    
    return(*dest.p);
}

FdoStringCollection FdoStringCollection::operator+=( const FdoStringCollection& coll2 )
{
    Append(coll2);
    return( *this );
}

FdoStringCollection FdoStringCollection::operator+=( const FdoStringP& str2 )
{
    Add(str2);
    return( *this );
}
*/

FdoStringCollection* FdoStringsP::operator=( FdoStringCollection* src )
{
    FDO_SAFE_RELEASE(p);
    p = src;
    return p;
}

const FdoStringsP FdoStringsP::operator+( const FdoStringsP coll2 ) const
{
    FdoStringsP dest = FdoStringCollection::Create(this->p);
    dest->Append(*(coll2.p));
    
    return(dest);
}

const FdoStringsP FdoStringsP::operator+( const FdoStringP str2 ) const
{
    FdoStringsP dest = FdoStringCollection::Create(*(this->p));
    dest->Add(str2);
    
    return(dest);
}

FdoStringsP FdoStringsP::operator+=( FdoStringsP coll2 )
{
    p->Append(*(coll2.p));
    return( *this );
}

FdoStringsP FdoStringsP::operator+=( FdoStringP str2 )
{
    p->Add(str2);
    return( *this );
}

