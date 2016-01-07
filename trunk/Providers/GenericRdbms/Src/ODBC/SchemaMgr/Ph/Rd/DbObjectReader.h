#ifndef FDOSMPHRDODBCDBOBJECTREADER_H
#define FDOSMPHRDODBCDBOBJECTREADER_H        1
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
#include <Sm/Ph/Rd/DbObjectReader.h>
#include <Inc/Rdbi/context.h>

// Odbc database object reader implementation

class FdoSmPhRdOdbcDbObjectReader : public FdoSmPhRdDbObjectReader
{
public:
    // Create and execute the database object reader
    //
    // Parameters:
    //      owner: get objects owned by this owner.
    //      objectName: If L"" then get all objects for the owner.
    //          Otherwise retrieve only this object.
    FdoSmPhRdOdbcDbObjectReader(
        FdoSmPhOwnerP owner,
        FdoStringP objectName = L""
    );

    // Deactivates the database object reader.
    virtual ~FdoSmPhRdOdbcDbObjectReader(void);

    virtual FdoSmPhDbObjType GetType();

protected:
    bool ReadNext();

private:
    rdbi_context_def    *rdbi_context;
    FdoSmPhDbObjType    m_objectType;
};

typedef FdoPtr<FdoSmPhRdOdbcDbObjectReader> FdoSmPhRdOdbcDbObjectReaderP;

#endif
