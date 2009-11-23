// 
//  
//  Copyright (C) 2009 Autodesk Inc.
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
#include "SltProvider.h"
#include "SQLiteSchemaMergeContext.h"
#include "SltConversionUtils.h"

SQLiteSchemaMergeContext::SQLiteSchemaMergeContext( 
    SltConnection* connection,
    FdoFeatureSchemaCollection* oldSchemas, 
    FdoFeatureSchema* newSchema, 
    bool ignoreStates  
) : FdoSchemaMergeContext( oldSchemas ),
    m_connection(connection)
{
    SetConnection( connection );
    SetUpdSchema( newSchema );
    SetIgnoreStates( ignoreStates );
}

SQLiteSchemaMergeContext* SQLiteSchemaMergeContext::Create( 
    SltConnection* connection,
    FdoFeatureSchemaCollection* oldSchemas, 
    FdoFeatureSchema* newSchema, 
    bool ignoreStates  
)
{
    return new SQLiteSchemaMergeContext( connection, oldSchemas, newSchema, ignoreStates );
}

bool SQLiteSchemaMergeContext::CanAddProperty( FdoPropertyDefinition* prop )
{
    return true;
}

bool SQLiteSchemaMergeContext::CanDeleteProperty( FdoPropertyDefinition* prop )
{
    if (prop->GetPropertyType() == FdoPropertyType_DataProperty)
    {
        FdoDataPropertyDefinition* dataProp = static_cast<FdoDataPropertyDefinition*>(prop);
        FdoPtr<FdoClassDefinition> pclass = static_cast<FdoClassDefinition*>( prop->GetParent() );
        if(pclass != NULL) {
            FdoPtr<FdoDataPropertyDefinitionCollection> identityProps = pclass->GetIdentityProperties();
            return !identityProps->Contains(dataProp);
        } else {
            return false;
        }
    }
    return true;
}

bool SQLiteSchemaMergeContext::CanModIdProps(FdoClassDefinition* classDef)
{
    return false;
}

bool SQLiteSchemaMergeContext::CanModDataType( FdoDataPropertyDefinition* prop )
{
    FdoPtr<FdoSchemaElement> pclass = prop->GetParent();
    return !TableHasObjects(pclass->GetName());
}

bool SQLiteSchemaMergeContext::CanModDefaultValue( FdoDataPropertyDefinition* prop )
{
    FdoPtr<FdoSchemaElement> pclass = prop->GetParent();
    return !TableHasObjects(pclass->GetName());
}

bool SQLiteSchemaMergeContext::CanModDataNullable( FdoDataPropertyDefinition* prop )
{
    FdoPtr<FdoSchemaElement> pclass = prop->GetParent();
    return !TableHasObjects(pclass->GetName());
}

bool SQLiteSchemaMergeContext::CanModDataLength( FdoDataPropertyDefinition* prop )
{
    FdoPtr<FdoSchemaElement> pclass = prop->GetParent();
    return !TableHasObjects(pclass->GetName());
}

bool SQLiteSchemaMergeContext::CanModDataConstraint( FdoDataPropertyDefinition* prop )
{
    FdoPtr<FdoSchemaElement> pclass = prop->GetParent();
    return !TableHasObjects(pclass->GetName());
}

bool SQLiteSchemaMergeContext::CanRestrictDataConstraint( FdoDataPropertyDefinition* prop )
{
    FdoPtr<FdoSchemaElement> pclass = prop->GetParent();
    return !TableHasObjects(pclass->GetName());
}

bool SQLiteSchemaMergeContext::ClassHasObjects( FdoClassDefinition* classDef )
{
    return TableHasObjects(classDef->GetName());
}

bool SQLiteSchemaMergeContext::TableHasObjects(FdoString* tableName)
{
    bool retVal = false;
    std::map<std::wstring, bool>::iterator it = m_hasObjects.find(tableName);
    if (it == m_hasObjects.end())
    {
        std::string fcname = W2A_SLOW(tableName);
        retVal = (m_connection->GetFeatureCount(fcname.c_str()) > 0);
        m_hasObjects[tableName] = retVal;
    }
    else
        retVal = it->second;
    return retVal;
}

bool SQLiteSchemaMergeContext::CheckDeleteProperty( FdoPropertyDefinition* prop )
{
    return CanDeleteProperty(prop);
}
