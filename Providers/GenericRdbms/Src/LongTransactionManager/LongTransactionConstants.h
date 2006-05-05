/******************************************************************************
 * (C) Copyright 2003 by Autodesk, Inc. All Rights Reserved.
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
  *
 ******************************************************************************/

// =============================================================================
// | The file defines various constants for the Long Transaction Manager.
// =============================================================================


#ifndef FdoRdbmsLongTransactionConstants_h
#define FdoRdbmsLongTransactionConstants_h

#ifdef _WIN32
#pragma once
#endif

//  FDORDBMSLONGTRANSACTIONCONSTANTS_LT_NAME_MAX_LENGTH:
//      The constant defines the maximum length of a long transaction name.

#define FDORDBMSLONGTRANSACTIONCONSTANTS_LT_NAME_MAX_LENGTH   30


//  FDORDBMSLONGTRANSACTIONCONSTANTS_ROOT_LT:
//      The constant represents the identifier used to address the root long
//      transaction.

static const FdoStringP FDORDBMSLONGTRANSACTIONCONSTANTS_ROOT_LT   = L"_ROOT_";

//  FDORDBMSLONGTRANSACTIONCONSTANTS_ACTIVE_LT:
//      The constant represents the identifier used to address the active long
//      transaction if the name is not known or to have a general addressing
//      method independent of names.

static const FdoStringP FDORDBMSLONGTRANSACTIONCONSTANTS_ACTIVE_LT = L"_ACTIVE_";

#endif

