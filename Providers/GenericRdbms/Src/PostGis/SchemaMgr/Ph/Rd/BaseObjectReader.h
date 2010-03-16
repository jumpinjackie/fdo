#ifndef FDOSMPHRDPOSTGISBASEOBJECTREADER_H
#define FDOSMPHRDPOSTGISBASEOBJECTREADER_H        1
/*
 * (C) Copyright 2003 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPSQSTE
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

// PostGis database object reader implementation

class FdoSmPhRdPostGisBaseObjectReader : public FdoSmPhRdBaseObjectReader
{
public:
    // Create and execute the base object reader
    //
    // Parameters:
    //      owner: get all base object references for this owner.
    FdoSmPhRdPostGisBaseObjectReader(
        FdoSmPhOwnerP owner
    );

    // Create and execute the base object reader
    //
    // Parameters:
    //      dbObject: get all base object references for this database object.
    FdoSmPhRdPostGisBaseObjectReader(
        FdoSmPhDbObjectP dbObject
    );

    // Create and execute the base object reader
    //
    // Parameters:
    //      objectNames: get all base object references for these database objects.
    FdoSmPhRdPostGisBaseObjectReader(
        FdoSmPhOwnerP owner,
        FdoStringsP objectNames
    );

    // Create and execute the base object reader
    //
    // Parameters:
    //      owner: get base object references for this owner.
    //      join: limit references to retrieve by joining to another table.
    FdoSmPhRdPostGisBaseObjectReader(
        FdoSmPhOwnerP owner,
        FdoSmPhRdTableJoinP join
    );

    // Deactivates the database object reader.
    virtual ~FdoSmPhRdPostGisBaseObjectReader(void);

protected:
    FdoSmPhReaderP MakeQueryReader(
        FdoSmPhOwnerP owner,
        FdoStringsP objectNames,
        FdoSmPhRdTableJoinP join = (FdoSmPhRdTableJoin*) NULL
    );

    // Reader for inherited table to base table relationships
    FdoSmPhReaderP MakeInheritReader(
        FdoSmPhOwnerP owner,
        FdoStringsP objectNames,
        FdoSmPhRdTableJoinP join = (FdoSmPhRdTableJoin*) NULL
    );

    // Reader for view to base table or view relationships.
    FdoSmPhReaderP MakeViewDependReader(
        FdoSmPhOwnerP owner,
        FdoStringsP objectNames,
        FdoSmPhRdTableJoinP join = (FdoSmPhRdTableJoin*) NULL
    );

};

typedef FdoPtr<FdoSmPhRdPostGisBaseObjectReader> FdoSmPhRdPostGisBaseObjectReaderP;

#endif
