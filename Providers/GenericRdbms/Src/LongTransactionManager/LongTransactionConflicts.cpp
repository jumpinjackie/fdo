//
//  Copyright (C) 2001-2003 Autodesk, Inc. All Rights Reserved.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
//  CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
//  IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
//  DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
//  DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject
//  to restrictions set forth in FAR 52.227-19 (Commercial Computer
//  Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
//  (Rights in Technical Data and Computer Software), as applicable.

// The links to the header files to be included.

// =============================================================================
// | The file contains the implementation of the class FdoRdbmsLongTransaction-
// | Conflicts. The class implements the long transaction conflict collection.
// =============================================================================


#include "stdafx.h"
#include "LongTransactionConflicts.h"


// ----------------------------------------------------------------------------
// --                            Public Interfaces                           --
// ----------------------------------------------------------------------------

FdoRdbmsLongTransactionConflicts *FdoRdbmsLongTransactionConflicts::Create ()

// +---------------------------------------------------------------------------
// | The function creates a new long transaction conflict collection and re-
// | turns a reference to it back to the calling routine.
// +---------------------------------------------------------------------------

{

    return new FdoRdbmsLongTransactionConflicts();

}  //  Create ()

