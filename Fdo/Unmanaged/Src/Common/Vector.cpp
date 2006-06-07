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
#include <Common/Vector.h>
#include "StringUtility.h"


// Todo: Basing on FdoCollection is a bit inefficient, since it stores
// an array of pointers to ref-counted wrappers around double numbers.
// Basing on FdoArray would be more space-efficient, since the internal 
// storage would simply be an array of doubles. However, more public functions
// (SetItem(), RemoveAt(), etc.) need to be added to FdoArray first.

FdoStringP FdoVectorElement::GetString()
{
    FdoCharacter buffer[100];
    FdoStringUtility::FormatDouble( mValue, buffer, 99 );
    return buffer;
//      return FdoStringP::Format( L"%lf", mValue );
}

FdoVector* FdoVector::Create()
{
    return new FdoVector();
}

FdoVector* FdoVector::Create( const FdoVector* src)
{
    return new FdoVector(src);
}

FdoVector* FdoVector::Create( const FdoStringP& inString, const wchar_t* delimiters, bool bNullTokens )
{
    return new FdoVector(inString, delimiters, bNullTokens);
}

FdoVector::FdoVector()
{
}

FdoVector::FdoVector( const FdoVector* src)
{
    if ( src ) {
        Append(src);
    }
}

FdoVector::FdoVector(const FdoStringP& inString, const wchar_t* delimiters, bool bNullTokens )
{
    int i;

    // Tokenize the sub-strings.
    FdoStringsP strings = FdoStringCollection::Create( inString, delimiters, bNullTokens );

    // Convert each to a number.
    for ( i = 0; i < strings->GetCount(); i++ ) {
        Add( FdoStringP(strings->GetString(i)).ToDouble() );
    }
}

FdoVector::~FdoVector()
{
}

double FdoVector::GetValue(int index) const
{
    FdoVectorElementP elem = FdoCollection<FdoVectorElement,FdoException>::GetItem(index);
    return elem->GetValue();
}

void FdoVector::Append( const FdoVector* src)
{
    for ( int i = 0; i < src->GetCount(); i++ ) 
        Add( src->GetValue(i) );
}

int FdoVector::Add( double value )
{
    FdoVectorElementP pElem = FdoVectorElement::Create(value);

    return FdoCollection<FdoVectorElement,FdoException>::Add(pElem);
}

FdoStringP FdoVector::ToString( const wchar_t* separator )
{
    FdoStringP retVal;
    FdoStringP sep;

    for ( int i = 0; i < GetCount(); i++ ) {
        retVal += sep + FdoVectorElementP(GetItem(i))->GetString();
        sep = separator;
    }

    return(retVal);
}

FdoVector* FdoVectorP::operator=( FdoVector* src )
{
    FDO_SAFE_RELEASE(p);
    p = src;
    return p;
}

const FdoVectorP FdoVectorP::operator+( const FdoVectorP vec2 ) const
{
    // Output vector length is maximum of input vector lengths.
    // Null vectors are considered to be 0 length.
    FdoSize len1 = p ? p->GetCount() : 0;
    FdoSize len2 = vec2 ? vec2->GetCount() : 0;
    FdoSize destLen = len1 > len2 ? len1 : len2;
    FdoSize i;


    FdoVectorP dest = FdoVector::Create();

    for ( i = 0; i < destLen; i++ ) {
        // Pad shorter input vector with zeros.
        double val1 = (i < len1) ? p->GetValue(i) : 0.0;
        double val2 = (i < len2) ? vec2->GetValue(i) : 0.0;
    
        // Add the two vector elements.
        dest->Add( val1 + val2 );
    }
    
    return(dest);
}

const FdoVectorP FdoVectorP::operator-( const FdoVectorP vec2 ) const
{
    // Output vector length is maximum of input vector lengths.
    // Null vectors are considered to be 0 length.
    FdoSize len1 = p ? p->GetCount() : 0;
    FdoSize len2 = vec2 ? vec2->GetCount() : 0;
    FdoSize destLen = len1 > len2 ? len1 : len2;
    FdoSize i;


    FdoVectorP dest = FdoVector::Create();

    for ( i = 0; i < destLen; i++ ) {
        // Pad shorter input vector with zeros.
        double val1 = (i < len1) ? p->GetValue(i) : 0.0;
        double val2 = (i < len2) ? vec2->GetValue(i) : 0.0;
    
        // Subtract the two vector elements.
        dest->Add( val1 - val2 );
    }
    
    return(dest);
}

FdoVectorP FdoVectorP::operator+=( const FdoVectorP vec2 )
{
    return( *this + vec2 );
}

FdoVectorP FdoVectorP::operator-=( const FdoVectorP vec2 )
{
    return( *this - vec2 );
}

FdoBoolean FdoVectorP::operator==( const FdoVectorP vec2 ) const
{
    return( Compare(vec2, false, true, false) );
}

FdoBoolean FdoVectorP::operator!=( const FdoVectorP vec2 ) const
{
    return( Compare(vec2, true, false, true) );
}

FdoBoolean FdoVectorP::operator<( const FdoVectorP vec2 ) const
{
    return( Compare(vec2, true, false, false) );
}

FdoBoolean FdoVectorP::operator<=( const FdoVectorP vec2 ) const
{
    return( Compare(vec2, true, true, false) );
}

FdoBoolean FdoVectorP::operator>( const FdoVectorP vec2 ) const
{
    return( Compare(vec2, false, false, true) );
}

FdoBoolean FdoVectorP::operator>=( const FdoVectorP vec2 ) const
{
    return( Compare(vec2, false, true, true) );
}

FdoBoolean FdoVectorP::Compare( const FdoVectorP vec2, FdoBoolean lt, FdoBoolean eq, FdoBoolean gt ) const
{
    // Determine vector lengths and maximum
    FdoSize len1 = p ? p->GetCount() : 0;
    FdoSize len2 = vec2 ? vec2->GetCount() : 0;
    FdoSize destLen = len1 > len2 ? len1 : len2;
    FdoSize i;
    FdoBoolean itemLt;
    FdoBoolean itemEq;
    FdoBoolean itemGt;
    FdoBoolean ret = eq;

    FdoVectorP dest = FdoVector::Create();

    // Compare the corresponding elements for each vector
    for ( i = 0; i < destLen; i++ ) {
        // Pad shorter vector with zeros
        double val1 = (i < len1) ? p->GetValue(i) : 0.0;
        double val2 = (i < len2) ? vec2->GetValue(i) : 0.0;

        // Get comparison results for current elements.
        itemLt = (val1 < val2);
        itemEq = (val1 == val2);
        itemGt = (val1 > val2);

        // If we're checking that this vector is < the given one
        // and the current element from this vector is < the element from the
        // given vector then this vector is < the given one. Similar case for >.
        if ( (itemLt && lt) || (itemGt && gt) ) 
            return true;    

        // If we're checking that this vector is not < the given one
        // and the current element from this vector is < the element from the
        // given vector then this vector is < the given one. Similar case for >.
        if ( (itemLt && !lt) || (itemGt && !gt) ) 
            return false;    
    }
    
    return(ret);
}

