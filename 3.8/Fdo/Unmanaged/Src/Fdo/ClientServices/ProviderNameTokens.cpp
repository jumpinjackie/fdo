/***************************************************************************
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
****************************************************************************/

#include <Fdo/ClientServices/ProviderNameTokens.h>

// Constructs a default instance of a FdoProviderCollection.
FdoProviderNameTokens* FdoProviderNameTokens::Create( FdoString* name )
{
    return new FdoProviderNameTokens( name );
}

// Constructs a default instance of a FdoProviderCollection.
FdoProviderNameTokens::FdoProviderNameTokens( FdoString* name )
{
    mNameTokens = FdoStringCollection::Create( name, L".", true ); 
}

FdoStringsP FdoProviderNameTokens::GetNameTokens() const
{
    return mNameTokens;
}

FdoVectorP FdoProviderNameTokens::GetVersionTokens() const
{
    int i;
    FdoVectorP versionTokens = FdoVector::Create();

    // 3rd token and onward are the version parts of the provider name.
    for ( i = 2; i < mNameTokens->GetCount(); i++ ) {
        versionTokens->Add( FdoStringP(mNameTokens->GetString(i)).ToDouble() );
    }

    return versionTokens;
}

FdoStringP FdoProviderNameTokens::GetLocalName() const
{
    if ( mNameTokens->GetCount() > 1 ) 
        return( mNameTokens->GetString(1) );
    else
        return( L"" );
}

void FdoProviderNameTokens::Dispose()
{
    delete this;
}

FdoProviderNameTokens* FdoProviderNameTokensP::operator=( FdoProviderNameTokens* src )
{
    FDO_SAFE_RELEASE(p);
    p = src;
    return p;
}


FdoBoolean FdoProviderNameTokensP::operator==( const FdoProviderNameTokensP name2 ) const
{
    if ( Compare(name2 ) )
        return( p->GetVersionTokens() == name2->GetVersionTokens() );
    else
        return(false);
}

FdoBoolean FdoProviderNameTokensP::operator!=( const FdoProviderNameTokensP name2 ) const
{
    if ( Compare(name2) )
        return( p->GetVersionTokens() != name2->GetVersionTokens() );
    else
        return(false);
}

FdoBoolean FdoProviderNameTokensP::operator<( const FdoProviderNameTokensP name2 ) const
{
    if ( Compare(name2) )
        return( p->GetVersionTokens() < name2->GetVersionTokens() );
    else
        return(false);
}

FdoBoolean FdoProviderNameTokensP::operator<=( const FdoProviderNameTokensP name2 ) const
{
    if ( Compare(name2) )
        return( p->GetVersionTokens() <= name2->GetVersionTokens() );
    else
        return(false);
}

FdoBoolean FdoProviderNameTokensP::operator>( const FdoProviderNameTokensP name2 ) const
{
    if ( Compare(name2) )
        return( p->GetVersionTokens() > name2->GetVersionTokens() );
    else
        return(false);
}

FdoBoolean FdoProviderNameTokensP::operator>=( const FdoProviderNameTokensP name2 ) const
{
    if ( Compare(name2) )
        return( p->GetVersionTokens() >= name2->GetVersionTokens() );
    else
        return(false);
}

FdoBoolean FdoProviderNameTokensP::Compare( const FdoProviderNameTokensP name2 ) const
{
    // Determine vector lengths and maximum
    FdoSize len1 = p->GetNameTokens()->GetCount();
    FdoSize len2 = name2->GetNameTokens()->GetCount();
    FdoSize destLen = len1 > len2 ? len1 : len2;
    FdoSize i;

    // Just compare the first 2 tokens ( company and local name )
    if ( destLen > 2 ) 
        destLen = 2;

    // Compare the vendor and local name tokens.
    for ( i = 0; i < destLen; i++ ) {
        // Pad shorter vector with zeros
        FdoStringP tok1 = (i < len1) ? p->GetNameTokens()->GetString( (int) i) : L"";
        FdoStringP tok2 = (i < len2) ? name2->GetNameTokens()->GetString( (int) i) : L"";

        if ( tok1 != tok2 ) 
            return( false );
    }
    
    return(true);
}

