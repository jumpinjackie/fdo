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

#ifndef ARCSDESQLDATAREADER_H
#define ARCSDESQLDATAREADER_H

#ifdef _WIN32
#pragma once
#endif // _WIN32


class ArcSDESQLDataReader :
    public virtual ArcSDEReader
{
    typedef ArcSDEReader superclass;

public:
    ArcSDESQLDataReader (ArcSDEConnection *connection, SE_STREAM stream);
    virtual ~ArcSDESQLDataReader (void);

    // Prevent the use of the Assignment Operation by definning it and not implemeting it.
    // DO NOT IMPLEMENT
    ArcSDESQLDataReader & operator= (const ArcSDESQLDataReader &right);


    //
    // FdoISQLDataReader interface specific
    //

    /// <summary>Gets the number of columns in the result set.</summary>
    /// <returns>Returns the number of columns.</returns> 
    virtual FdoInt32 GetColumnCount ();

    /// <summary>Gets the name of the column at the given ordinal position.</summary>
    /// <param name="index">Input the position of the column.</param> 
    /// <returns>Returns the column name</returns> 
    virtual FdoString* GetColumnName (FdoInt32 index);

    /// <summary>Gets the data type of the column with the specified name.</summary>
    /// <param name="columnName">Input the column name.</param> 
    /// <returns>Returns the type of the column.</returns> 
    virtual FdoDataType GetColumnType (FdoString* columnName);

    /// <summary>Gets the FDO property type of the column with the specified name. This is used
    /// to indicate if a given column is a geometric property or a data property. If the column is
    /// a FdoPropertyType_DataProperty, then GetColumnType can be used to find the data type of the column.</summary>
    /// <param name="propertyName">Input the column name.</param> 
    /// <returns>Returns the FDO property type of the column.</returns> 
    virtual FdoPropertyType GetPropertyType(FdoString* columnName);


protected:
    ArcSDESQLDataReader::ColumnDefinition* createColumnDef(int column, SE_COLUMN_DEF* definition);
    virtual ArcSDESQLDataReader::ColumnDefinition* getColumnDef (const wchar_t* identifier);
    virtual void getColumnDefs ();
};

#endif // ARCSDESQLDATAREADER_H

