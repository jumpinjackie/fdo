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
 */

#ifndef ARCSDEDESCRIBESCHEMACOMMAND_H
#define ARCSDEDESCRIBESCHEMACOMMAND_H
#ifdef _WIN32
#pragma once
#endif // _WIN32

class ArcSDEDescribeSchemaCommand :
    public ArcSDECommand<FdoIDescribeSchema>
{
    friend class ArcSDEConnection;

private:
    wchar_t* mSchemaName;
    FdoStringsP mClassNames;
	FdoPtr<FdoIdentifier> mFdoClassIdToLoad;

private:
    //
    // Prevent the use of the copy constructor by defining it and not implementing it.
    // DO NOT IMPLEMENT
    ArcSDEDescribeSchemaCommand (const ArcSDEDescribeSchemaCommand &right);

    // Constructs an instance of a DescribeSchema command using the given connection.
    ArcSDEDescribeSchemaCommand (FdoIConnection* connection, FdoIdentifier* fdoClassIdToLoad=NULL);

protected:
    // Default destructor for DescribeSchema command.
    virtual ~ArcSDEDescribeSchemaCommand (void);

    ////////////////////////////////////////////
    // Misc protected functions:
    ////////////////////////////////////////////

    void GetArcSDEMetadata(const SE_METADATAINFO &metadata, CHAR* classname, CHAR* property, CHAR* value, CHAR* description, CHAR* metadata_qual_table);
    void addClass (ArcSDEConnection* connection, FdoFeatureSchema* schema, wchar_t* class_name, wchar_t* class_description,
        CHAR* qualified_table_name, SE_REGINFO registration, SE_METADATAINFO *metadata_list, LONG num_records);
    void addTable (ArcSDEConnection* connection, FdoFeatureSchemaCollection* schemas, SE_REGINFO registration);
    FdoFeatureSchema* findOrCreateSchema (FdoFeatureSchemaCollection* schemas, const wchar_t* name, const wchar_t* description);

    // Converts a Metadata value string to FDO datatype enum:
    FdoDataType MetadataValueToFDOType(CHAR* value);

    // Converts a Metadata value string to boolean:
    bool MetadataValueToBoolean(CHAR* value);


public:
    //
    // Prevent the use of the Assignment Operation by defining it and not implementing it.
    // DO NOT IMPLEMENT
    ArcSDEDescribeSchemaCommand & operator= (const ArcSDEDescribeSchemaCommand &right);

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

#endif // ARCSDEDESCRIBESCHEMACOMMAND_H


