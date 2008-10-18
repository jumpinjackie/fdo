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
#include <Fdo/Filter/NullCondition.h>
#include <Fdo/Filter/IFilterProcessor.h>
#include "StringUtility.h"

// Constructs a default instance of an NullCondition.
FdoNullCondition* FdoNullCondition::Create()
{
	return new FdoNullCondition();
}

// Constructs an instance of a NullCondition using the specified arguments.
FdoNullCondition* FdoNullCondition::Create(FdoString* propertyName)
{
	return new FdoNullCondition(propertyName);
}

// Constructs an instance of a NullCondition using the specified arguments.
FdoNullCondition* FdoNullCondition::Create(FdoIdentifier* propertyName)
{
	return new FdoNullCondition(propertyName);
}

// Constructs a default instance of an NullCondition.
FdoNullCondition::FdoNullCondition()
{
	m_propertyName = NULL;
}

// Constructs an instance of a NullCondition using the specified arguments.
FdoNullCondition::FdoNullCondition(FdoString* propertyName)
{
	m_propertyName = FdoIdentifier::Create(propertyName);
}

// Constructs an instance of a NullCondition using the specified arguments.
FdoNullCondition::FdoNullCondition(FdoIdentifier* propertyName)
{
	m_propertyName = FDO_SAFE_ADDREF(propertyName);
}

FdoNullCondition::~FdoNullCondition()
{
	FDO_SAFE_RELEASE(m_propertyName);
}

void FdoNullCondition::Dispose()
{
    delete this;
}

// Gets the name of the data property.
FdoIdentifier* FdoNullCondition::GetPropertyName()
{
    return FDO_SAFE_ADDREF(m_propertyName);
}

// Sets the name of the data property.
void FdoNullCondition::SetPropertyName(FdoIdentifier* value)
{
    FDO_SAFE_RELEASE(m_propertyName);
	m_propertyName = FDO_SAFE_ADDREF(value);
}

// Overrides Expression.Process to pass the NullCondition to the
// appropriate expression processor operation.
void FdoNullCondition::Process(FdoIFilterProcessor* p)
{
    p->ProcessNullCondition(*this); 
}

// Converts the filter expression to it's well defined text representation.
FdoString* FdoNullCondition::ToStringInternal( FdoIdentifierCollection *pIdCol )
{
    FdoStringUtility::ClearString(m_toString);
    FdoIdentifier*  pIdent = GetPropertyName();
    if (pIdent == NULL)
    {
        FdoFilterException* pExcept = FdoFilterException::Create(FdoException::NLSGetMessage(FDO_NLSID(FILTER_5_INCOMPLETENULLCONDITION)));
        throw pExcept;
    }

    try
    {
        m_toString = FdoStringUtility::MakeString(pIdent->ToStringInternal(pIdCol), L" NULL");
    }
    catch (FdoException* pExcept)
    {
        FDO_SAFE_RELEASE(pIdent);
        throw pExcept;
    }

    pIdent->Release();
    return m_toString;
}

