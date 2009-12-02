/***************************************************************************
 * Copyright (C) 2004-2006  Autodesk, Inc.
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
 ***************************************************************************/

// =============================================================================
// | The file contains the definition of the class FdoRdbmsGetLongTransactions.
// | The class provides access to a collection of long transactions. Depending
// | on the given flag the collection either contains only the information for
// | the user's active long transaction or all long transactions currently
// | available in the data store.
// =============================================================================

#ifndef FdoRdbmsGetLongTransactions_h
#define FdoRdbmsGetLongTransactions_h 1

// The links to the header files to be included.

#include "FdoRdbms.h"
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsCommand.h"
#include "LongTransactionDataRequestType.h"
#include "FdoRdbmsLongTransactionReader.h"

class FdoRdbmsGetLongTransactions : public FdoRdbmsCommand<FdoIGetLongTransactions>
{

  friend class FdoRdbmsConnection;

  public:

    // ************************************************************************
    // *                           Public Interfaces                          *
    // ************************************************************************

    //  GetName:
    //      Provides access to the property holding the long transaction name.

    FdoString *GetName ();


    //  SetName:
    //      Sets the value of the property holding the long transaction name.
    //      The function can be used to nullify the property value.

    void SetName  (FdoString *value);


    //  Execute:
    //      Executes the command.

    FdoILongTransactionReader *Execute ();


  protected:

    // ************************************************************************
    // *                         Protected Interfaces                         *
    // ************************************************************************

    //  FdoRdbmsGetLongTransactions:
    //      A class constructor.

    FdoRdbmsGetLongTransactions ();

    //  FdoRdbmsGetLongTransactions:
    //      A class constructor.

    FdoRdbmsGetLongTransactions (FdoIConnection *connection);

    //  ~FdoRdbmsGetLongTransactions:
    //      The class destructor.

    virtual ~FdoRdbmsGetLongTransactions ();

    //  ClearMemory:
    //      Frees all memory allocated by the member variables.

    void ClearMemory ();

    //  SetToZero:
    //      Initializes all class member variables.

    void SetToZero ();

    //  SetValue:
    //      The function allocates memory for a string and initializes it with
    //      the given value.

    wchar_t *SetValue (FdoString *value_buffer);


  private:

    // ************************************************************************
    // *                        Private Member Variables                      *
    // ************************************************************************

    //  lt_name:
    //      Stores the name of the long transaction.

    FdoString *lt_name;


};  //  class FdoRdbmsGetLongTransactions

#endif
