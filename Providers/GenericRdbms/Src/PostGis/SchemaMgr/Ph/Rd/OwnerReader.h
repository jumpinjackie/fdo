//
// Copyright (C) 2006 Refractions Research, Inc.
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
#ifndef FDOSMPHRDPOSTGISOWNERREADER_H
#define FDOSMPHRDPOSTGISOWNERREADER_H

#include <Sm/Ph/Rd/OwnerReader.h>

/// Defines master reader for schemas form a database.
/// This class reads all owners, also called schemas, for the given
/// database instance.
///
class FdoSmPhRdPostGisOwnerReader : public FdoSmPhRdOwnerReader
{
public:
	
    /// Create and execute the reader.
    /// \param
    /// 
    /// database [in] - get the owners for this database instance.
    /// ownerName [in] - if set, the reader retrieves a single row for this owner.
    /// If empty L"" all objects in the database instance are read.
    ///
	FdoSmPhRdPostGisOwnerReader(FdoSmPhDatabaseP database,
        FdoStringP ownerName = L"");

    /// Destructor deactivates the reader instance.
	~FdoSmPhRdPostGisOwnerReader();

	/// Get description of the current owner from F_SCHEMAINFO.
    /// \return
    /// String with current owner description.
	FdoStringP GetDescription();

protected:

    /// \todo To be documented.
    FdoSmPhReaderP MakeQueryReader(FdoSmPhDatabaseP database,
        FdoStringP ownerName = L""); 

    virtual FdoSmPhReaderP MakeHasMetaSchemaReader(
        FdoSmPhDatabaseP database,
        FdoStringP ownerName = L""
    ); 

    /// \todo To be documented.
    FdoSmPhRowP MakeBinds(FdoSmPhMgrP mgr, FdoStringP ownerName);

};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmPhRdPostGisOwnerReader> FdoSmPhRdPostGisOwnerReaderP;

#endif // FDOSMPHRDPOSTGISOWNERREADER_H