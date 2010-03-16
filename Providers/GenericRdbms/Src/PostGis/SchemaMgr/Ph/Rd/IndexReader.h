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
#ifndef FDORDBMSPHRDPOSTGISINDEXREADER_H
#define FDORDBMSPHRDPOSTGISINDEXREADER_H


#include <Sm/Ph/Rd/IndexReader.h>

/// Definition of foreign key reader for PostGIS provider.
///
class FdoSmPhRdPostGisIndexReader : public FdoSmPhRdIndexReader
{
public:

    /// Create and execute the index reader.
    FdoSmPhRdPostGisIndexReader(FdoSmPhOwnerP owner, FdoSmPhDbObjectP dbObject);

    /// Create and execute the index reader.
    FdoSmPhRdPostGisIndexReader(FdoSmPhOwnerP owner, FdoStringsP objectNames);

    /// Destructor deactivates the index reader.
    ~FdoSmPhRdPostGisIndexReader();

private:

    FdoSmPhReaderP MakeReader(FdoSmPhOwnerP owner,
        FdoStringsP objectNames);
};

#endif // FDORDBMSPHRDPOSTGISINDEXREADER_H
