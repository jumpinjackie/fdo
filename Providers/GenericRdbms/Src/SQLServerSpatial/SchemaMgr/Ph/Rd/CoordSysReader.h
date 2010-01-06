#ifndef FdoSmPhRdSqsCoordSysReader_H
#define FdoSmPhRdSqsCoordSysReader_H        1
/*
 * Copyright (C) 2004-2008  Autodesk, Inc.
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
#include <Sm/Ph/Rd/CoordSysReader.h>

// This class is used retrieve coordinate systems defined for SqlServer
//

class FdoSmPhRdSqsCoordSysReader : public FdoSmPhRdCoordSysReader
{
public:
    // Create and execute the Coordinate System reader.
    //
    // Parameters:
    //      owner: retrieve coordinate systems for this datastore.
    //      csysName: if L"" then retrieve all coordinate systems.
    //                if alphanumeric, retrieve a single coordinate system by name.
    //                if numeric, retrieve a single coordinate system by SRID.
    FdoSmPhRdSqsCoordSysReader(
        FdoSmPhOwnerP owner,
        FdoStringP csysName = L""
    );

    // Create and execute the Coordinate System reader to retrieve by SRID.
    // Allows SRID to be passed in as Int64, rather than numeric string,
    //
    // Parameters:
    //      owner: retrieve coordinate systems for this datastore.
    //          (however, Oracle SRS's are actually instance-wide.
    //      srid: retrieve the coordinate system with this srid.
    //            if -1 then all coordinate systems are retrieved.
    FdoSmPhRdSqsCoordSysReader(
        FdoSmPhOwnerP owner,
        FdoInt64 srid
    );

    // Deactivates the reader.
    ~FdoSmPhRdSqsCoordSysReader(void);

protected:
    // Constructs the underlying query reader. Note that csysName takes
    // precedence over srid. When csysName=L"" and srid=-1,
    // all coordinates systems are retrieved.
    // 
    // 
    //
    // Parameters:
    //      owner: retrieve coordinate systems for this datastore.
    //          (however, Oracle SRS's are actually instance-wide.
    //      csysName: if L"" then retrieve is done by srid.
    //                otherwise, retrieve a single coordinate system by name.
    //      srid: retrieve the coordinate system with this srid.
    //            if -1 then retrieval is done by csysName.
    FdoSmPhReaderP MakeQueryReader(
        FdoSmPhOwnerP owner,
        FdoStringP csysName = L"",
        FdoInt64 srid = -1
    );

    // Create the bind variables for underlying query reqder
    FdoSmPhRowP MakeBinds( 
        FdoSmPhMgrP mgr, 
        FdoStringP objectName,
        FdoInt64 srid
    );

};

typedef FdoPtr<FdoSmPhRdSqsCoordSysReader> FdoSmPhRdSqsCoordSysReaderP;

#endif
