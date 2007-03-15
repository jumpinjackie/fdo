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

#include "InsertCommand.h"
#include "Connection.h"

#include <cassert>

namespace fdo { namespace postgis {

InsertCommand::InsertCommand(Connection* conn) :
    Base(conn),
    mClassIdentifier(NULL),
    mProperties(NULL),
    mBatchParameters(NULL)
{
    // idle
}

InsertCommand::~InsertCommand()
{
    // idle
}

///////////////////////////////////////////////////////////////////////////////
// FdoIInsert interface
///////////////////////////////////////////////////////////////////////////////

FdoIdentifier* InsertCommand::GetFeatureClassName()
{
    FDO_SAFE_ADDREF(mClassIdentifier.p);
    return mClassIdentifier.p;
}
    
void InsertCommand::SetFeatureClassName(FdoIdentifier* classIdentifier)
{
    mClassIdentifier = classIdentifier;
    FDO_SAFE_ADDREF(mClassIdentifier.p);
}
    
void InsertCommand::SetFeatureClassName(FdoString* className)
{
    FdoPtr<FdoIdentifier> cid;
    if (NULL != className)
        cid = FdoIdentifier::Create(className);
    else
        cid = NULL;

    SetFeatureClassName(cid);
}
    
FdoPropertyValueCollection* InsertCommand::GetPropertyValues()
{
    if (NULL == mProperties)
    {
        mProperties = FdoPropertyValueCollection::Create();
        assert(NULL != mProperties);
    }

    FDO_SAFE_ADDREF(mProperties.p);
    return mProperties.p;
}
    
FdoBatchParameterValueCollection* InsertCommand::GetBatchParameterValues()
{
    return GetCollection(mBatchParameters);
}
    
FdoIFeatureReader* InsertCommand::Execute()
{
    assert(!"NOT IMPLEMENTED");
    return 0;
}

}} // namespace fdo::postgis

