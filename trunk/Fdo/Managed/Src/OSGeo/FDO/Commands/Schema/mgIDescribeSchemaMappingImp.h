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
#include "FDO\Commands\Schema\mgIDescribeSchemaMapping.h"

class FdoIDescribeSchemaMapping;

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA
ref class PhysicalSchemaMappingCollection;

/// \ingroup (OSGeoFDOCommandsSchema)
/// \brief
/// The IDescribeSchemaMappingImp class is a concrete implementation of IDescribeSchemaMapping.
/// The IDescribeSchemaMapping interface defines the DescribeSchemaMapping command, which
/// describes the logical to physical schema mappings for feature schemas 
/// available from the connection. The DescribeSchemaMapping
/// command can describe the mappings for a single schema or all schemas available from
/// the connection. The Execute operation returns an PhysicalSchemaMappingCollection
/// object.
private ref class IDescribeSchemaMappingImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, 
                                               public NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::IDescribeSchemaMapping
{
public:
    /// \brief
    /// Gets the name of the schema for which to describe
    /// schema mappings.
    /// 
    /// \return
    /// Returns the schema name
    /// 
    /// \brief
    /// Sets the name of the schema to describe. This function is optional; if not
    /// specified, execution of the command will describe the mappings for
    /// all schemas.
    /// 
    /// \param value 
    /// Input the schema name
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual property System::String^ SchemaName
    {
        System::String^ get();
        System::Void set(System::String^ value);
    }

    /// \brief
    /// Gets the current "include default mappings" setting.
    /// 
    /// \return
    /// Returns True if Execute() will include default mappings.
    /// 
    /// \brief
    /// Sets the "include default mappings" setting.
    /// 
    /// \param includeDefaults 
    /// Input True: Execute() will return
    /// all mappings for the feature schema(s).
    /// False: Execute() will not include default logical to physical
    /// mappings, only those mappings that have been overridden will 
    /// be returned.
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual property System::Boolean IncludeDefaults
    {
        System::Boolean get();
        System::Void set(System::Boolean includeDefaults);
    }

    /// \brief
    /// Executes the DescribeSchemaMapping command and returns a 
    /// PhysicalSchemaMappingCollection. If the specified schema name does not exist,
    /// the Execute method throws an exception.
    /// 
    /// \return
    /// Returns the schema mapping collection for the requested feature schemas.
    /// 
	virtual NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA::PhysicalSchemaMappingCollection^ Execute();

internal:
	IDescribeSchemaMappingImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIDescribeSchemaMapping* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_SCHEMA


