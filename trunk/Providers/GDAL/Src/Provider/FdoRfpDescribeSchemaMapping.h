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
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpDescribeSchemaMapping.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#ifndef FDORFPDESCRIBESCHEMAMAPPING_H
#define FDORFPDESCRIBESCHEMAMAPPING_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "FdoRfpCommand.h"

//
// Class forward declarations
//
class FdoGrfpClassDefinition;


class FdoRfpDescribeSchemaMapping : public FdoRfpCommand<FdoIDescribeSchemaMapping>
{
	friend class FdoRfpConnection;
//
// Data members
//
private:
	FdoStringP		m_schemaName;
	FdoBoolean		m_includeDefaults;
//
// Constructor(s), desctrucotr, factory function(s)
//
protected:
	FdoRfpDescribeSchemaMapping(FdoIConnection* connection);
	virtual ~FdoRfpDescribeSchemaMapping();

public:
    /// <summary>Gets the name of the schema for which to describe
    /// schema mappings.</summary>
    /// <returns>Returns the schema name</returns> 
    virtual FdoString* GetSchemaName();

    /// <summary>Sets the name of the schema to describe. This function is optional; if not
    /// specified, execution of the command will describe the mappings for
    /// all schemas.</summary>
    /// <param name="value">Input the schema name</param> 
    /// <returns>Returns nothing</returns> 
    virtual void SetSchemaName(FdoString* value);

    /// <summary>Gets the current "include default mappings" setting.</summary>
    /// <returns>Returns True if Execute() will include default mappings.</returns> 
    virtual FdoBoolean GetIncludeDefaults();

    /// <summary>Sets the "include default mappings" setting.</summary>
    /// <param name="includeDefaults">Input True: Execute() will return
    /// all mappings for the feature schema(s).
    /// False: Execute() will not include default logical to physical
    /// mappings, only those mappings that have been overridden will 
    /// be returned.</param> 
    /// <returns>Returns nothing</returns> 
    virtual void SetIncludeDefaults( FdoBoolean includeDefaults );

    /// <summary>Executes the DescribeSchemaMapping command and returns a 
    /// FdoPhysicalSchemaMappingCollection. If the specified schema name does not exist,
    /// the Execute method throws an exception.</summary>
    /// <returns>Returns the schema mapping collection for the requested feature schemas.</returns> 
    virtual FdoPhysicalSchemaMappingCollection* Execute();

//
// internally used helper functions
//
private:
	FdoPtr<FdoGrfpClassDefinition> _cloneClass(const FdoPtr<FdoGrfpClassDefinition>& classDef);

	FdoPtr<FdoPhysicalSchemaMapping> _cloneSchemaMapping(const FdoPtr<FdoPhysicalSchemaMapping>& schemaMapping);

};


#endif
