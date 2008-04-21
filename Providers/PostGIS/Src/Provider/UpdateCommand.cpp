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
    FDOLOG_MARKER("UpdateCommand::+Execute");
    assert(NULL != mConn);

    if (NULL == mPropertyValues)
    {
        return 0;
    }

    //
    // Collect schema details required to build SQL command
    //
    // NOTE - Eric Barby: Faster way of describing schema.
    // XXX - mloskot: Why not wrapped with FdoPtr, SchemaDescription::Ptr?
    SchemaDescription*  schemaDesc = mConn->DescribeSchema();
    if (!schemaDesc || !schemaDesc->IsDescribed()) 
    {
        throw FdoCommandException::Create(L"[PostGIS] InsertCommand can not find schema definition");
    }

    FdoPtr<FdoIdentifier> classIdentifier(GetFeatureClassName());
    FdoPtr<FdoClassDefinition> classDef(schemaDesc->FindClassDefinition(mClassIdentifier));    
    ov::ClassDefinition::Ptr phClass(schemaDesc->FindClassMapping(mClassIdentifier));
    if (!classDef || !phClass) 
    {
        throw FdoCommandException::Create(L"[PostGIS] UpdateCommand can not find class definition");
    }

    FdoStringP tablePath(phClass->GetTablePath());

    //
    // Collect column & value paris
    //
    std::string sep;
    std::string sqlValues;

    ExpressionProcessor::Ptr expProc(new ExpressionProcessor());
    FdoPtr<FdoPropertyDefinitionCollection>     propsDefinition(classDef->GetProperties());
    FdoPtr<FdoDataPropertyDefinitionCollection> propsIdentity(classDef->GetIdentityProperties());
    FdoInt32 currentSrid = GetSRID(propsDefinition);

    FdoInt32 const propsSize = mPropertyValues->GetCount();
    for (FdoInt32 i = 0; i < propsSize; i++)
    {
        FdoPtr<FdoPropertyValue> propVal(mPropertyValues->GetItem(i));
        FdoStringP   pName(propVal->GetName()->GetName());
        FdoPtr<FdoPropertyDefinition> propDef(GetPropDefinition(propsDefinition, pName));
        if (propDef)
        {
            pName = propDef->GetName();
        }
        else
        {
            FDOLOG_WRITE(L"can not find porpertyDefinition '%s'", static_cast<FdoString*>(propVal->GetName()->GetName()));
        }

        FdoPtr<FdoValueExpression> expr(propVal->GetValue());
        expr->Process(expProc);
        std::string value = expProc->ReleaseBuffer();
        if (propDef)
        {
            if (FdoPropertyType_DataProperty == propDef->GetPropertyType())
            {
                FdoDataPropertyDefinition* dataDef = static_cast<FdoDataPropertyDefinition*>(propDef.p);

                // NOTE - Eric Barby: Need to parse date properly
                // XXX - mloskot: Is the code below the solution?
                if (FdoDataType_DateTime == dataDef->GetDataType())
                {
                    FdoDateTimeValue* dateValuePtr = dynamic_cast<FdoDateTimeValue*>(propVal->GetValue());
                    if (!dateValuePtr->IsNull())
                    {
                        FDOLOG_WRITE(L"convert Date:", dateValuePtr->ToString());
                        value = static_cast<char const*>(FdoStringP(dateValuePtr->ToString()));
                    }
                }
            } 

            // TODO - bscott: this geom is not needed,maybe we should delete these lines 
            else
            {
                if (FdoPropertyType_GeometricProperty == propDef->GetPropertyType())
                {
                    // TODO: Eric Barby - It won't work with multiple-geometric properties
                    FdoGeometricPropertyDefinition* geom = NULL;
                    geom = static_cast<FdoGeometricPropertyDefinition*>(propDef.p);
                }
            }
        }

        sqlValues += sep + static_cast<char const*>(pName);
        sqlValues += " = ";
        sqlValues += value;

        sep = ",";
    }

    //
    // Build WHERE clause
    //
    FilterProcessor::Ptr filterProc(new FilterProcessor(currentSrid));
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
