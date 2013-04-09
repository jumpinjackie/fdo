#ifndef FDOSMPHRDSYNONYMREADER_H
#define FDOSMPHRDSYNONYMREADER_H        1
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
#include <Sm/Ph/Rd/BaseObjectReader.h>
#include <Sm/Ph/Rd/TableJoin.h>

// Reads information about a synonym ( such as its referenced database object).
//
// Base class for RDBMS-specific synonym readers
class FdoSmPhRdSynonymReader : public FdoSmPhRdBaseObjectReader
{
public:
    // Create and execute the synonym reader
    FdoSmPhRdSynonymReader(
        FdoSmPhReaderP reader,      // inner reader created by implementing class
        FdoSmPhOwnerP owner        // get the base objects for this owner
    );

    // Deactivates the query.
    ~FdoSmPhRdSynonymReader(void);

protected:
    // unused constructor needed only to build on Linux
    FdoSmPhRdSynonymReader() {}

};

typedef FdoPtr<FdoSmPhRdSynonymReader> FdoSmPhRdSynonymReaderP;

#endif
