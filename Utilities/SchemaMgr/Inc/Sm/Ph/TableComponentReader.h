#ifndef FDOSMPHTABLECOMPONENTREADER_H
#define FDOSMPHTABLECOMPONENTREADER_H		1
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
 *         $Id: //providers_reliant/SchemaMgr/inc/Sm/Ph/TableComponentReader.h#1 $
 *     $Author: br $
 *   $DateTime: 2006/05/09 17:53:47 $
 *     $Change: 12802 $
 * 
 */

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Ph/GroupReader.h>
#include <Sm/Ph/Rd/IndexReader.h>

class FdoSmPhTableComponentReader : public FdoSmPhGroupReader
{
public:
    /// Attaches to a query reader. 
    /// Call ReadNext() to read each row.
	//
    /// Parameters:
    /// 	groupName: the name of the group of rows to retrieve.
    /// 		only rows whose ordering columns equal this value 
    /// 		will be retrieved. Must be a concatenation of the 
    /// 		of the ordering column values for the rows to retrieve.
    /// 	pReader: the reader to attach to. The reader must return
    /// 		rows in ascending order by group name. Note that if this
    /// 		reader is currently positioned after the group to retrieve
    /// 		then this reader returns no rows.
	FdoSmPhTableComponentReader(
        FdoStringP groupName, 
        FdoStringP groupTableName, 
        FdoStringP groupFieldName, 
        FdoSmPhReaderP reader
    );

	virtual ~FdoSmPhTableComponentReader(void);

    /// This function determines the group name for the attached reader's
    /// current row. Classes derived from this class can implement this 
    /// function in any way, as long as the reader retrieves rows in 
    /// ascending order by group name.
	virtual FdoStringP GetGroupName();

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhTableComponentReader() {}

private:
	FdoStringP mGroupTableName;
	FdoStringP mGroupFieldName;
};

typedef FdoPtr<FdoSmPhTableComponentReader> FdoSmPhTableComponentReaderP;

class FdoSmPhTableIndexReader : public FdoSmPhTableComponentReader
{
public:
    /// Attaches to a query reader. 
    /// Call ReadNext() to read each row.
	//
    /// Parameters:
    /// 	groupName: the name of the group of rows to retrieve.
    /// 		only rows whose ordering columns equal this value 
    /// 		will be retrieved. Must be a concatenation of the 
    /// 		of the ordering column values for the rows to retrieve.
    /// 	pReader: the reader to attach to. The reader must return
    /// 		rows in ascending order by group name. Note that if this
    /// 		reader is currently positioned after the group to retrieve
    /// 		then this reader returns no rows.
	FdoSmPhTableIndexReader(
        FdoStringP groupFieldName, 
        FdoPtr<FdoSmPhRdIndexReader> reader
    );

	virtual ~FdoSmPhTableIndexReader(void);

    /// Returns the type of the current index (Scalar or Spatial)
    virtual FdoSmPhIndexType GetIndexType();

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmPhTableIndexReader() {}

private:
	FdoPtr<FdoSmPhRdIndexReader> mIndexReader;
};

typedef FdoPtr<FdoSmPhTableIndexReader> FdoSmPhTableIndexReaderP;

#endif


