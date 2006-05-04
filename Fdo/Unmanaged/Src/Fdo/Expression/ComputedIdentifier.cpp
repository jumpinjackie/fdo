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
#include <wctype.h>
#include <Fdo/Expression/ComputedIdentifier.h>
#include <Fdo/Expression/IExpressionProcessor.h>
#include <Fdo/Commands/IdentifierCollection.h>
#include "StringUtility.h"


// Constructs a default instance of an Identifier.
FdoComputedIdentifier* FdoComputedIdentifier::Create()
{
    return new FdoComputedIdentifier();
}

// Constructs an instance of an Identifier using the specified arguments.
FdoComputedIdentifier* FdoComputedIdentifier::Create(FdoString* name, FdoExpression* expression)
{
    return new FdoComputedIdentifier(name, expression);
}

FdoComputedIdentifier::~FdoComputedIdentifier()
{
    FDO_SAFE_RELEASE( m_Expression );
}

void FdoComputedIdentifier::Dispose()
{
    delete this;
}

// Constructs a default instance of an Identifier.
FdoComputedIdentifier::FdoComputedIdentifier():
m_Expression( NULL )
{
    
}

// Constructs an instance of an Identifier using the specified arguments.
FdoComputedIdentifier::FdoComputedIdentifier(FdoString* name, FdoExpression* expression):FdoIdentifier( name ),
m_Expression( FDO_SAFE_ADDREF( expression ) )
{
   
}

// Gets the full text of the identifier.
FdoExpression* FdoComputedIdentifier::GetExpression()
{
	return FDO_SAFE_ADDREF( m_Expression );
}

// Sets the full text of the identifier.
void FdoComputedIdentifier::SetExpression(FdoExpression* value)
{
    FDO_SAFE_RELEASE( m_Expression );
    m_Expression = FDO_SAFE_ADDREF( value );
}

// Overrides Expression.Process to pass the Computed Identifier to the appropriate
// expression processor operation.
void FdoComputedIdentifier::Process(FdoIExpressionProcessor* p)
{
    p->ProcessComputedIdentifier(*this); 
}

FdoString* FdoComputedIdentifier::ToStringInternal( FdoIdentifierCollection *pIdCol )
{
    if( pIdCol != NULL )
    {
        if( ! pIdCol->Contains( this ) )
            pIdCol->Add( this );
        return FdoIdentifier::ToString();
    }
    else
        return ToString();
}

// Returns the well defined text representation of this expression.
FdoString* FdoComputedIdentifier::ToString()
{
    wchar_t*       identStr;
    const wchar_t* exprStr = L"null Expression";
    const wchar_t* startStr = L"( ";
    const wchar_t* asStr = L" ) AS ";

    if( m_Expression )
        exprStr = m_Expression->ToString();

    identStr = FdoStringUtility::MakeString( FdoIdentifier::ToString() );

    FdoStringUtility::ClearString(m_toString);

    m_toString = FdoStringUtility::MakeString( startStr, exprStr, asStr,  identStr );

    FdoStringUtility::ClearString( identStr );

    return m_toString;
}

