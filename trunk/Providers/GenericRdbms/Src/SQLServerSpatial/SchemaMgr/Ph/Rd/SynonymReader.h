#ifndef FDOSMPHRDSQSSYNONYMREADER_H
#define FDOSMPHRDSQSSYNONYMREADER_H        1
/*
 * (C) Copyright 2011 by Autodesk, Inc. All Rights Reserved.
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
#include <Sm/Ph/Rd/SynonymReader.h>
#include <Sm/Ph/Rd/TableJoin.h>

// Reads information about a synonym ( such as its referenced database object).
//
class FdoSmPhRdSqsSynonymReader : public FdoSmPhRdSynonymReader
{
public:
    // Create and execute the synonym reader
    //
    // Parameters:
    //      owner: owning datastore
    //      objectName: name of synonym to read
    //      objectNames: list of  synonyms to read
    FdoSmPhRdSqsSynonymReader(
        FdoSmPhOwnerP owner,
        FdoStringP objectName = L""
    );

    FdoSmPhRdSqsSynonymReader(
        FdoSmPhOwnerP owner,
        FdoStringsP objectNames
    );

    // Deactivates the query.
    ~FdoSmPhRdSqsSynonymReader(void);

    bool ReadNext();

    FdoStringP GetString(FdoStringP tableName, FdoStringP fieldName);

protected:
    // unused constructor needed only to build on Linux
    FdoSmPhRdSqsSynonymReader() {}

    // Create the underlying query reader
    FdoSmPhReaderP MakeQueryReader(
        FdoSmPhOwnerP owner,
        FdoStringsP objectNames
    );

    virtual FdoSmPhRowsP MakeRows( FdoSmPhMgrP mgr);

private:
    FdoStringP mBaseName;
    FdoStringP mBaseOwner;

    FdoSmPhOwnerP mOwner;
};

typedef FdoPtr<FdoSmPhRdSqsSynonymReader> FdoSmPhRdSqsSynonymReaderP;

#endif
