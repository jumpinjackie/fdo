//
// Copyright (C) 2007 Refractions Research, Inc. 
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#include "stdafx.h"

#include "PostGisProvider.h"
#include "SelectCommand.h"
#include "Connection.h"
#include "PostGIS/FdoPostGisOverrides.h"

#include <cassert>

namespace fdo { namespace postgis {

SelectCommand::SelectCommand(Connection* conn) :
    Base(conn),
    mProperties(NULL),
    mOrderingProperties(NULL),
    mOrderingOption(FdoOrderingOption_Ascending)
{
    // idle
}

SelectCommand::~SelectCommand()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void SelectCommand::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoIBaseSelect interface
///////////////////////////////////////////////////////////////////////////////

FdoIdentifierCollection* SelectCommand::GetPropertyNames()
{
    if (NULL == mProperties)
    {
        mProperties = FdoIdentifierCollection::Create();
    }

    FDO_SAFE_ADDREF(mProperties.p);
    return mProperties.p;
}
 	
FdoIdentifierCollection* SelectCommand::GetOrdering()
{
    if (NULL == mOrderingProperties)
    {
        mOrderingProperties = FdoIdentifierCollection::Create();
    }

    FDO_SAFE_ADDREF(mOrderingProperties.p);
    return mOrderingProperties.p;
}

void SelectCommand::SetOrderingOption(FdoOrderingOption option)
{
    mOrderingOption = option;
}

FdoOrderingOption SelectCommand::GetOrderingOption()
{
    return mOrderingOption;
}

///////////////////////////////////////////////////////////////////////////////
// FdoISelect interface
///////////////////////////////////////////////////////////////////////////////

FdoLockType SelectCommand::GetLockType()
{
    throw FdoCommandException::Create(
        NlsMsgGet(MSG_POSTGIS_LOCKING_NOT_SUPPORTED,
        "The PostGIS provider does not support locking (%1$ls).", L"GetLockType"));

    return FdoLockType_None;
}

void SelectCommand::SetLockType(FdoLockType value)
{
    FdoStringP msg(FdoStringP::Format(L"SetLockType($ls)", value));

    throw FdoCommandException::Create(
        NlsMsgGet(MSG_POSTGIS_LOCKING_NOT_SUPPORTED,
        "The PostGIS provider does not support locking (%1$ls).",
        static_cast<FdoString*>(msg)));
}

FdoLockStrategy SelectCommand::GetLockStrategy()
{
    throw FdoCommandException::Create(
        NlsMsgGet(MSG_POSTGIS_LOCKING_NOT_SUPPORTED,
        "The PostGIS provider does not support locking (%1$ls).", L"GetLockStrategy"));

    return FdoLockStrategy_All;
}

void SelectCommand::SetLockStrategy(FdoLockStrategy value)
{
    FdoStringP msg(FdoStringP::Format(L"SetLockStrategy($ls)", value));

    throw FdoCommandException::Create(
        NlsMsgGet(MSG_POSTGIS_LOCKING_NOT_SUPPORTED,
        "The PostGIS provider does not support locking (%1$ls).",
        static_cast<FdoString*>(msg)));
}
    
FdoIFeatureReader* SelectCommand::Execute()
{
    FDOLOG_MARKER("SelectCommand::+Execute");
    
    //
    // Get Logical Schema
    //
    FdoPtr<FdoFeatureSchemaCollection> logicalSchemas;
    logicalSchemas = mConn->GetLogicalSchema();
    
    //
    // Get Feature Class
    //
    FdoPtr<FdoIdentifier> classIdentifier = GetFeatureClassName();
    assert(NULL != classIdentifier);
    FdoStringP className = classIdentifier->GetText();
    FdoPtr<FdoClassDefinition> classDef = NULL;
    
    // Find definition of the feature class
    FdoPtr<FdoIDisposableCollection> featureClasses = NULL;
    featureClasses = logicalSchemas->FindClass(className);
    if (NULL != featureClasses)
    {
        classDef = static_cast<FdoClassDefinition*>(featureClasses->GetItem(0));
        assert(NULL != classDef);
    }
    FDOLOG_WRITE(L"Select feature class: %s", static_cast<FdoString*>(className));
    
    
    return 0;
}
    
FdoIFeatureReader* SelectCommand::ExecuteWithLock()
{
    throw FdoCommandException::Create(
        NlsMsgGet(MSG_POSTGIS_LOCKING_NOT_SUPPORTED,
        "The PostGIS provider does not support locking (%1$ls).", L"ExecuteWithLock"));

    return NULL;
}
    
FdoILockConflictReader* SelectCommand::GetLockConflicts()
{
    throw FdoCommandException::Create(
        NlsMsgGet(MSG_POSTGIS_LOCKING_NOT_SUPPORTED,
        "The PostGIS provider does not support locking (%1$ls).", L"GetLockConflicts"));

    return NULL;
}

}} // namespace fdo::postgis
