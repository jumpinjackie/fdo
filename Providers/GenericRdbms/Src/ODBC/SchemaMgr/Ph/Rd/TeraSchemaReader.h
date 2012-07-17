//
// Copyright (C) 2012 Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#ifndef FdoSmPhRdTeraSchemaReader_H
#define FdoSmPhRdTeraSchemaReader_H

#include <Sm/Ph/Rd/SchemaReader.h>

/// Defines master reader for schemas form a database.
/// This class reads all owners, also called schemas, for the given
/// database instance.
///
class FdoSmPhRdTeraSchemaReader : public FdoSmPhRdSchemaReader
{
public:
	
    /// Create and execute the reader.
    /// \param
    /// 
    /// database [in] - get the owners for this database instance.
    /// ownerName [in] - if set, the reader retrieves a single row for this owner.
    /// If empty L"" all objects in the database instance are read.
    ///
	FdoSmPhRdTeraSchemaReader(FdoSmPhRowsP rows, FdoSmPhDatabaseP database,
        FdoSmPhOwnerP owner);

    /// Destructor deactivates the reader instance.
	~FdoSmPhRdTeraSchemaReader();

    virtual FdoStringP GetString( FdoStringP tableName, FdoStringP fieldName );

private:
};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmPhRdTeraSchemaReader> FdoSmPhRdTeraSchemaReaderP;

#endif // FdoSmPhRdTeraSchemaReader_H
