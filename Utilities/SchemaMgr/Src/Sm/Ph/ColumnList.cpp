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
#include "stdafx.h"
#include <Sm/Ph/ColumnList.h>


FdoSmPhColumnList* FdoSmPhColumnList::Create( FdoSmPhMgrP mgr)
{
    return new FdoSmPhColumnList(mgr);
}

FdoSmPhColumnList* FdoSmPhColumnList::Create( FdoSmPhMgrP mgr, const FdoStringCollection* src)
{
    return new FdoSmPhColumnList(mgr, src);
}

FdoSmPhColumnList* FdoSmPhColumnList::Create( FdoSmPhMgrP mgr, const FdoStringCollection& src)
{
    return new FdoSmPhColumnList(mgr, src);
}

FdoSmPhColumnList* FdoSmPhColumnList::Create( FdoSmPhMgrP mgr, const FdoStringP& inString, const wchar_t* delimiters )
{
    return new FdoSmPhColumnList(mgr, inString, delimiters);
}

FdoSmPhColumnList::FdoSmPhColumnList( FdoSmPhMgrP mgr) :
    FdoStringCollection()
{
    mMgr = mgr;
}

FdoSmPhColumnList::FdoSmPhColumnList( FdoSmPhMgrP mgr, const FdoStringCollection* src) :
    FdoStringCollection(src)
{
    mMgr = mgr;
}

FdoSmPhColumnList::FdoSmPhColumnList( FdoSmPhMgrP mgr, const FdoStringCollection& src) :
    FdoStringCollection(src)
{
    mMgr = mgr;
}

FdoSmPhColumnList::FdoSmPhColumnList(FdoSmPhMgrP mgr, const FdoStringP& inString, const wchar_t* delimiters ) :
    FdoStringCollection()
{
    mMgr = mgr;

    FdoStringsP quoteTokens = FdoStringCollection::Create( inString, L"\"", true );
    FdoInt32 idx;
    bool inQuote = false;

    for ( idx = 0; idx < quoteTokens->GetCount(); idx++ ) {
        FdoStringP token = quoteTokens->GetString(idx);
        if ( token.GetLength() > 0 ) {
            if ( inQuote ) {
                Add( token );
            }
            else {
                FdoStringsP nonQuoteTokens = FdoStringCollection::Create( token, delimiters, false );
                Append( *(nonQuoteTokens) );
            }
        }

        inQuote = !inQuote;
    }
}

FdoSmPhColumnList::~FdoSmPhColumnList()
{
}

FdoSmPhMgrP FdoSmPhColumnList::GetManager()
{
    return mMgr;
}

FdoStringP FdoSmPhColumnList::GetDbString( FdoInt32 idx )
{
    FdoStringP delim;

    if ( mMgr->SupportsAnsiQuotes() )
        delim = "\"";

    return delim + GetString(idx) + delim;
}

FdoStringP FdoSmPhColumnList::ToString( const wchar_t* separator )
{
    FdoStringP retVal;
    FdoStringP sep;

    for ( int i = 0; i < GetCount(); i++ ) {
        retVal += sep + 
            (mMgr->IsRdbObjNameAscii7() ? 
                FdoStringP(GetString(i)) : GetDbString(i));
        sep = separator;
    }

    return(retVal);
}


#if 0
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

#endif
