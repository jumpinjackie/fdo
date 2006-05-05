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
// | The file contains the definition of the class FdoRdbmsTableInfo.
// =============================================================================


#ifndef FdoRdbmsTableInfo_h
#define FdoRdbmsTableInfo_h

#ifdef _WIN32
#pragma once
#endif

#include <FdoCommonThreadMutex.h>
#include <Inc/Nls/fdordbms_msg.h>
#include "FdoRdbmsUtil.h"
#include "FdoRdbmsException.h"
#include "Disposable.h"

class FdoRdbmsTableInfo : public FdoLkDisposable
{

    public:

    // Class Functions:
    //  Get*:
    //      Functions to get the values of the class variables.
    //  Set*:
    //      Functions to set the class variable values.


    void                     SetDataStoreName  (FdoString *value);
    void                     SetTableName      (FdoString *value);

    FdoString                *GetDataStoreName ();
    FdoString                *GetTableName     ();

    static FdoRdbmsTableInfo *Create           ();

    protected:

    // Class Functions:
    //  FdoRdbmsTableInfo:
    //      Class constructor.
    //  ~FdoRdbmsTableInfo:
    //      Class destructor.

        FdoRdbmsTableInfo          ();
        virtual ~FdoRdbmsTableInfo ();


    private:

    // Class Variables:
    //  data_store_name:
    //      The variable stores the name of the table host.
    //  table_name:
    //      The variable stores the name of the table.


    FdoString *data_store_name,
              *table_name;


    // Class Functions:
    //  ClearMemory:
    //      Function to release occupied memory.
    //  SetToZero:
    //      Function to initialize class variables.
    //  SetValue:
    //      Function to save a given value buffer.

    void    ClearMemory ();
    void    SetToZero   ();

    wchar_t *SetValue   (FdoString *value_buffer);


};  //  class FdoRdbmsTableInfo

#endif

