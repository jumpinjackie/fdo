/*
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
*/

#pragma once

#include "FDO\Commands\mgICommandImp.h"
#include "FDO\Commands\Schema\mgIDescribeSchema.h"

class FdoIDescribeSchema;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __gc class FeatureSchemaCollection;
END_NAMESPACE_OSGEO_FDO_SCHEMA

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA

/// \brief
/// The IDescribeSchemaImp class is a concrete implementation of IDescribeSchema.
/// The IDescribeSchema interface defines the DescribeSchema command, which
/// describes the feature schemas available from the connection. The DescribeSchema
/// command can describe a single schema or all schemas available from
/// the connection. The Execute operation returns an FeatureSchemaCollection
/// object.
private __gc class IDescribeSchemaImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, 
                                        public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IDescribeSchema
{
public:
    /// \brief
    /// Gets the name of the schema to describe. This function is optional;
    /// if not specified, execution of the command will describe all schemas.
    /// 
    /// \return
    /// Returns the schema name
    /// 
	__property System::String* get_SchemaName();

    /// \brief
    /// Sets the name of the schema to describe. This function is optional; if not
    /// specified execution of the command will describe all schemas.
    /// 
    /// \param value 
    /// Input the schema name
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_SchemaName(System::String* value);

    /// \brief
    /// Executes the DescribeSchema command and returns a 
    /// FeatureSchemaCollection. If a schema name is given that has 
    /// references to another schema, the dependent schemas will 
    /// be returned as well. If the specified schema name does not exist,
    /// the Execute method throws an exception.
    /// 
    /// \return
    /// Returns the schema collection representing the schema created.
    /// The element states for all elements will be set to SchemaElementState_Unchanged.
    /// Each provider-specific implementation of Execute() can ensure 
    /// that this is the case by 
    /// calling FeatureSchema::AcceptChanges() for each feature schema
    /// in the returned collection.
    /// 
	NAMESPACE_OSGEO_FDO_SCHEMA::FeatureSchemaCollection* Execute();

public private:
	IDescribeSchemaImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIDescribeSchema* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA


