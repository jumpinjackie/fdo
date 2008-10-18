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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpDescribeSchemaCommand.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#ifndef FDORFPDESCRIBESCHEMACOMMAND_H
#define FDORFPDESCRIBESCHEMACOMMAND_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "FdoRfpCommand.h"

class FdoRfpDescribeSchemaCommand : public FdoRfpCommand<FdoIDescribeSchema>
{
	friend class FdoRfpConnection;
private:
	FdoStringP	m_schemaName;
    FdoStringCollection* m_classNames;

	FdoRfpDescribeSchemaCommand(FdoIConnection* connection);
	virtual ~FdoRfpDescribeSchemaCommand(void);

	FdoClassDefinitionP _cloneClass(const FdoClassDefinitionP& classDef);
	FdoFeatureSchemaP _cloneSchema(const FdoFeatureSchemaP& featureSchema);
public:

    /// <summary>Gets the name of the schema to describe. This function is optional;
    /// if not specified, execution of the command will describe all schemas.</summary>
    /// <returns>Returns the schema name</returns> 
    virtual const wchar_t* GetSchemaName ();

    /// <summary>Sets the name of the schema to describe. This function is optional; if not
    /// specified, execution of the command will describe all schemas.</summary>
    /// <param name="value">Input the schema name</param> 
    /// <returns>Returns nothing</returns> 
    virtual void SetSchemaName (const wchar_t* value);

    /// <summary>Gets the names of the classes to retrieve. This is optional,
    /// if not specified execution of the command will describe all classes.
    /// If the class name is not qualified, and the schema name is not specified,
    /// the requested class from all schemas will be described.
    /// The class names specified serve only as a hint.  Use of the hint
    /// during command execution is provider dependent.  Providers that 
    /// will not use the hint will describe the schema for all classes.</summary>
    /// <returns>Returns the collection of class names</returns>
    virtual FdoStringCollection* GetClassNames();

    /// <summary>Sets the name of the classes to retrieve. This is optional, if not
    /// specified execution of the command will describe all classes.
    /// If the class name is not qualified, and the schema name is not specified,
    /// the requested class from all schemas will be described.
    /// The class names specified serve only as a hint.  Use of the hint
    /// during command execution is provider dependent.  Providers that 
    /// will not use the hint will describe the schema for all classes.</summary>
    /// <param name="value">Input the collection of class names</parm>
    /// <returns>Returns nothing</returns>
    virtual void SetClassNames(FdoStringCollection* value);

    /// <summary>Executes the DescribeSchema command and returns a 
    /// FdoFeatureSchemaCollection. If a schema name is given that has 
    /// references to another schema, the dependent schemas will 
    /// be returned as well. If the specified schema name does not exist,
    /// the Execute method throws an exception.</summary>
    /// <returns>Returns the schema collection representing the schema created.</returns> 
    virtual FdoFeatureSchemaCollection* Execute ();


};

#endif
