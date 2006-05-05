/*
 * 
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

#include "stdafx.h"

#include <malloc.h>
#include <sdeerno.h>

ArcSDESQLDataReader::ArcSDESQLDataReader (ArcSDEConnection *connection, SE_STREAM stream) :
    ArcSDEReader(connection, NULL, NULL)
{
    mStream = stream;
}

ArcSDESQLDataReader::~ArcSDESQLDataReader (void)
{
}

/** Do not implement the copy constructor. **/
//ArcSDESQLDataReader::ArcSDESQLDataReader (const ArcSDESQLDataReader &right) { }


ArcSDESQLDataReader::ColumnDefinition* ArcSDESQLDataReader::createColumnDef (int column, SE_COLUMN_DEF* columnDef)
{
    ColumnDefinition *retColumnDef = new ArcSDESQLDataReader::ColumnDefinition();

    // Set column name/number/type:
    wchar_t* wColumnName = NULL;
    multibyte_to_wide(wColumnName, columnDef->column_name);
    wcscpy (retColumnDef->mColumnName, wColumnName);
    retColumnDef->mColumnNumber = column;
    retColumnDef->mColumnType = columnDef->sde_type;

    // Set property name/type/length:
    // NOTE: SQLDataReader knows only about column names/types, not FDO names/types.
    wcscpy(retColumnDef->mPropertyName, L"");
    retColumnDef->mPropertyType = SDEType2FdoType(columnDef->sde_type);
    retColumnDef->mDataLength = columnDef->size;  //NOTE: column's size does NOT include room for the null terminator

    // Set leftover stuff:
    retColumnDef->mBindIsNull = SE_IS_NULL_VALUE;
    retColumnDef->mValuePointer = NULL;
    retColumnDef->mValuePointerSize = 0;

    return retColumnDef;
}


// Here, identifier is interpreted as an *ArcSDE column* name
ArcSDESQLDataReader::ColumnDefinition* ArcSDESQLDataReader::getColumnDef (const wchar_t* identifier)
{
    ColumnDefinition* columnDef = NULL;

    if (0 == mColumnCount)
        getColumnDefs ();

    for (int i = 0; ((i < mColumnCount) && (NULL == columnDef)); i++)
    {
        if (0 == wcscmp (identifier, mColumnDefs[i]->mColumnName))
        {
            columnDef = mColumnDefs[i];
            break;
        }
    }

    if (NULL==columnDef)
        throw FdoCommandException::Create(NlsMsgGet1(ARCSDE_COLUMN_NOT_IN_RESULT, "The column '%1$ls' is not in the query result.", identifier));

    return (columnDef);
}



void ArcSDESQLDataReader::getColumnDefs ()
{
    LONG result = 0L;
    SHORT count = 0;
    SE_COLUMN_DEF column;

    if (NULL == mColumnDefs)
    {
        if (NULL == mStream)
            throw FdoException::Create (NlsMsgGet(ARCSDE_STREAM_NOT_INITIALIZED, "Stream not initialized."));

        result = SE_stream_num_result_columns (mStream, &count);
        handle_sde_err<FdoCommandException>(mStream, result, __FILE__, __LINE__, ARCSDE_STREAM_GENERAL_ERROR, "Error encountered while manipulating stream.");


        mColumnDefs = new ColumnDefinition*[count];
        for (int i = 0; i < count; i++)
        {
            result = SE_stream_describe_column (mStream, i + 1, &column);
            if (SE_SUCCESS == result)
                mColumnDefs[i] = createColumnDef (i + 1, &column);
        }
        mColumnCount = count;
    }
}




/// <summary>Gets the number of columns in the result set.</summary>
/// <returns>Returns the number of columns.</returns> 
FdoInt32 ArcSDESQLDataReader::GetColumnCount ()
{
    if (0 == mColumnCount)
        getColumnDefs ();

    return (mColumnCount);
}

/// <summary>Gets the name of the column at the given ordinal position.</summary>
/// <param name="index">Input the position of the column.</param> 
/// <returns>Returns the column name</returns> 
FdoString* ArcSDESQLDataReader::GetColumnName (FdoInt32 index)
{
    return ((ArcSDEReader::getColumnDef((int)index))->mColumnName);
}

/// <summary>Gets the data type of the column with the specified name.</summary>
/// <param name="columnName">Input the column name.</param> 
/// <returns>Returns the type of the column.</returns> 
FdoDataType ArcSDESQLDataReader::GetColumnType (FdoString* columnName)
{
    return ((getColumnDef (columnName))->mPropertyType);
}


/// <summary>Gets the FDO property type of the column with the specified name. This is used
///  to indicate if a given column is a geometric property or a data property. If the column is
///  a FdoPropertyType_DataProperty, then GetColumnType can be used to find the data type of the column.</summary>
/// <param name="propertyName">Input the column name.</param> 
/// <returns>Returns the FDO property type of the column.</returns> 
FdoPropertyType ArcSDESQLDataReader::GetPropertyType(FdoString* columnName)
{
    // Since ArcSDE doesn't let us read geometry values via SQL queries,
    // we only support Data property types in ArcSDESQLDataReader:
    return FdoPropertyType_DataProperty;
}


