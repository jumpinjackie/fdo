#ifndef FDOSMPHRDODBCORABASEOBJECTREADER_H
#define FDOSMPHRDODBCORABASEOBJECTREADER_H        1
/*
 * (C) Copyright 2007 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 *
 */
#include <Sm/Ph/Rd/BaseObjectReader.h>
#include <Sm/Ph/Rd/TableJoin.h>

// This class is used by the Physical Schema object to query the
// database for schema information.
//
// To perform the query, the caller creates this object with a
// GQL statement, which is parsed and executed. The caller then
// can call ReadNext() to get each row returned by the query and
// the various Get functions to get the row data.
//
// The query is initially positioned before the first row, so
// ReadNext must be called before data can be retrieved.

class FdoSmPhRdOdbcOraBaseObjectReader : public FdoSmPhRdBaseObjectReader
{
public:
    // Create and execute a query
    //
    // Parameters:
    //      sStatement: GQL statement to execute
    //      physicalSchema: holds the connection for the Danube database to query.
    FdoSmPhRdOdbcOraBaseObjectReader(
        FdoSmPhOwnerP owner
    );

    FdoSmPhRdOdbcOraBaseObjectReader(
        FdoSmPhDbObjectP    dbObject
    );

    FdoSmPhRdOdbcOraBaseObjectReader(
        FdoSmPhOwnerP owner,
        FdoStringsP objectNames
    );

    FdoSmPhRdOdbcOraBaseObjectReader(
        FdoSmPhOwnerP owner,
        FdoSmPhRdTableJoinP join
    );

    // Deactivates the query.
    ~FdoSmPhRdOdbcOraBaseObjectReader(void);

protected:
    // unused constructor needed only to build on Linux
    FdoSmPhRdOdbcOraBaseObjectReader() {}

    FdoSmPhReaderP MakeQueryReader(
        FdoSmPhOwnerP owner,
        FdoStringsP objectNames,
        FdoSmPhRdTableJoinP join = (FdoSmPhRdTableJoin*) NULL
    );

private:
};

typedef FdoPtr<FdoSmPhRdOdbcOraBaseObjectReader> FdoSmPhRdOdbcOraBaseObjectReaderP;

#endif
