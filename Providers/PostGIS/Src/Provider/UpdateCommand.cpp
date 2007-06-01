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
#include "UpdateCommand.h"
#include "FeatureCommand.h"
#include "Connection.h"
#include "FilterProcessor.h"
// std
#include <cassert>
#include <string>

namespace fdo { namespace postgis {

UpdateCommand::UpdateCommand(Connection* conn) : Base(conn)
{
    assert(NULL != mConn);
}

UpdateCommand::~UpdateCommand()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIUpdate interface
///////////////////////////////////////////////////////////////////////////////

FdoPropertyValueCollection* UpdateCommand::GetPropertyValues()
{
    if (NULL == mPropertyValues)
    {
        mPropertyValues = FdoPropertyValueCollection::Create();
    }

    FDO_SAFE_ADDREF(mPropertyValues.p);
    return mPropertyValues.p;
}
 	
FdoInt32 UpdateCommand::Execute()
{
    FDOLOG_MARKER("DeleteCommand::+Execute");
    assert(NULL != mConn);

    if (NULL == mPropertyValues)
    {
        return 0;
    }

    //
    // Collect schema details required to build SQL command
    //
    SchemaDescription::Ptr schemaDesc(SchemaDescription::Create());
    schemaDesc->DescribeSchema(mConn, NULL);

    FdoPtr<FdoIdentifier> classIdentifier(GetFeatureClassName());
    FdoPtr<FdoClassDefinition> classDef(schemaDesc->FindClassDefinition(mClassIdentifier));    
    if (!classDef) 
    {
        throw FdoCommandException::Create(L"[PostGIS] UpdateCommand can not find class definition");
    }

    ov::ClassDefinition::Ptr phClass(schemaDesc->FindClassMapping(mClassIdentifier));
    FdoStringP tablePath(phClass->GetTablePath());

    //
    // Collect column & value paris
    //
    std::string sep;
    std::string sqlValues;

    ExpressionProcessor::Ptr expProc(new ExpressionProcessor());

    FdoInt32 const propsSize = mPropertyValues->GetCount();
    for (FdoInt32 i = 0; i < propsSize; i++)
    {
        FdoPtr<FdoPropertyValue> propValue(mPropertyValues->GetItem(i));
        FdoPtr<FdoIdentifier> propId(propValue->GetName());

        FdoPtr<FdoValueExpression> expr(propValue->GetValue());
        expr->Process(expProc);

        sqlValues += sep + static_cast<char const*>(FdoStringP(propId->GetName()));
        sqlValues += " = ";
        sqlValues += expProc->ReleaseBuffer();

        sep = ",";
    }

    //
    // Build WHERE clause
    //
    FilterProcessor::Ptr filterProc(new FilterProcessor());
    std::string sqlWhere;

    if (NULL != mFilter)
    {
        mFilter->Process(filterProc);

        std::string sqlFilter(filterProc->GetFilterStatement());
        if (!sqlFilter.empty())
        {
            sqlWhere = " WHERE " + sqlFilter;
        }
    }

    //
    // Build and execute SQL command
    //
    std::string sql("UPDATE ");
    sql += static_cast<char const*>(tablePath);
    sql += " SET ";
    sql += sqlValues;
    sql += sqlWhere;

    FdoSize affected = 0;
    mConn->PgExecuteCommand(sql.c_str(), affected);

    return static_cast<FdoInt32>(affected);
}
 	
FdoILockConflictReader* UpdateCommand::GetLockConflicts()
{
    FDOLOG_MARKER("DeleteCommand::+GetLockConflicts");
    FDOLOG_WRITE("*** NOT IMPLEMENTED ***");

    return NULL;
}

}} // namespace fdo::postgis
