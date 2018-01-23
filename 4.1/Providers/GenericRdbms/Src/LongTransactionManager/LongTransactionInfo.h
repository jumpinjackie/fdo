//
// Copyright (C) 2004-2006  Autodesk, Inc.
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

// =============================================================================
// | The file contains the definition of the class FdoRdbmsLongTransactionInfo.
// | The class is used to store and provide access to long transaction informa-
// | tion.
// =============================================================================


#ifndef FdoRdbmsLongTransactionInfo_h
#define FdoRdbmsLongTransactionInfo_h

#ifdef _WIN32
#pragma once
#endif

#include <FdoCommonThreadMutex.h>
#include <Inc/Nls/fdordbms_msg.h>
#include "FdoRdbmsUtil.h"
#include "FdoRdbmsException.h"
#include "Disposable.h"

class FdoRdbmsLongTransactionInfo : public FdoLtmDisposable
{

    public:

    // ************************************************************************
    // *                           Public Interfaces                          *
    // ************************************************************************

    //  Create:
    //      Function to create a new long transaction info object.

    static FdoRdbmsLongTransactionInfo *Create ();

    //  GetCreationDate:
    //      Functions to get creation date for the current long transaction.

    FdoDateTime GetCreationDate ();

    //  GetDescription:
    //      Function to get the description for the current long transaction.

    FdoString *GetDescription ();

    //  GetFreezeMode:
    //      Function to get the freeze mode for the current long transaction.

    FdoString *GetFreezeMode ();

    //  GetFreezeOwner:
    //      Function to get the freeze owner for the current long transaction.

    FdoString *GetFreezeOwner ();

    //  GetFreezeWriter:
    //      Function to get the freeze writer for the current long transaction.

    FdoString *GetFreezeWriter ();

    //  GetIsActiveFlag:
    //      Function to retrieve the value of the flag indicating whether or
    //      not the long transaction is active.

    bool GetIsActiveFlag ();

    //  GetIsRefreshedFlag:
    //      Function to retrieve the value of the flag indicating whether or
    //      not the current long transaction represents a continously refreshed
    //      one.

    bool GetIsRefreshedFlag ();

    //  GetLtId:
    //      Function to get the long transaction id.

    FdoInt64 GetLtId ();

    //  GetName:
    //      Function to get the name for the current long transaction.

    FdoString *GetName ();

    //  GetOwner:
    //      Functions to get the owner of the current long transaction.

    FdoString *GetOwner ();

    //  Initialize:
    //      The function resets the contents.

    void Initialize ();

    //  SetCreationDate:
    //      Functions to set the creation date for the long transaction.

    void SetCreationDate (FdoDateTime value);

    //  SetDescription:
    //      Function to set the description for the long transaction.

    void SetDescription (FdoString *value);

    //  SetFreezeMode:
    //      Function to set the freeze mode for the long transaction.

    void SetFreezeMode (FdoString *value);

    //  SetFreezeOwner:
    //      Function to set the freeze owner for the long transaction.

    void SetFreezeOwner (FdoString *value);

    //  SetFreezeWriter:
    //      Function to set the freeze writer for the long transaction.

    void SetFreezeWriter (FdoString *value);

    //  SetIsActiveFlag:
    //      Function to set the value of the flag indicating whether or not the
    //      long transaction is active.

    void SetIsActiveFlag (bool value);

    //  SetIsRefreshedFlag:
    //      Function to set the value of the flag indicating whether or not the
    //      long transaction represents a continously refreshed one.

    void SetIsRefreshedFlag (bool value);

    //  SetLtId:
    //      Function to set the long transaction id.

    void SetLtId (FdoInt64 value);

    //  SetName:
    //      Function to set the name for the long transaction.

    void SetName (FdoString *value);

    //  SetOwner:
    //      Functions to set the owner of the long transaction.

    void SetOwner (FdoString *value);


    protected:

    // ************************************************************************
    // *                         Protected Interfaces                         *
    // ************************************************************************

    //  FdoRdbmsLongTransactionInfo:
    //      Class constructor.

    FdoRdbmsLongTransactionInfo ();

    //  ~FdoRdbmsLongTransactionInfo:
    //      Class destructor.

    virtual ~FdoRdbmsLongTransactionInfo ();


    private:

    // ************************************************************************
    // *                        Private Member Variables                      *
    // ************************************************************************

    //  creation_date:
    //      The variable holds the information on when the long transaction
    //      for which the information is provided was created.

    FdoDateTime creation_date;

    //  description:
    //      The variable holds the description for the current long transaction.

    FdoString *description;

    //  freeze_owner:
    //      If the long transaction for which the information is provided is
    //      frozen this variable indicates the FDO user having frozen it.

    FdoString *freeze_owner;

    //  freeze_mode:
    //      If the long transaction for which the information is provided is
    //      frozen, this variable reports the freeze mode.

    FdoString *freeze_mode;

    //  freeze_writer:
    //      A long transaction can be frozen in a way that one user may still
    //      have write privileges on it. In such a case this variable reports
    //      that user.

    FdoString *freeze_writer;

    //  is_active:
    //      The value of this variable indicates whether or not the long trans-
    //      action for which the information is provided is active.

    bool is_active;

    //  is_refreshed:
    //      The variable informs whether or not the long transaction for which
    //      this information is provided is a continously refreshed long trans-
    //      action.

    bool is_refreshed;

    //  lt_id:
    //      The variable reports the unqiue id of the long transaction for which
    //      the information is provided was created. The value is set to -1 if
    //      the number cannot be provided.

    FdoInt64 lt_id;

    //  name:
    //      The variable records the name of the long transaction for which the
    //      the information is provided.

    FdoString *name;

    //  owner:
    //      The variable provides the name of the owner of the long transaction
    //      for which the information is provided.

    FdoString *owner;


    // ************************************************************************
    // *                          Private Interfaces                          *
    // ************************************************************************

    //  ClearMemory:
    //      Function to release occupied memory.

    void ClearMemory ();

    //  SetToZero:
    //      Function to initialize class variables.

    void SetToZero ();


    //  SetValue:
    //      Function to save a given value buffer.

    wchar_t *SetValue (FdoString *value_buffer);


};  //  class FdoRdbmsLongTransactionInfo

#endif

