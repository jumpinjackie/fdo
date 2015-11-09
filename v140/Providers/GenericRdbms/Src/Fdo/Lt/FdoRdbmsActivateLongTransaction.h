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
// | The file contains the definition of the class FdoRdbmsActivateLongTrans-
// | action. The class activates a specified long transaction. Input to the
// | command is the name of the long transaction to be activated.
// =============================================================================

#ifndef FdoRdbmsActivateLongTransaction_h
#define FdoRdbmsActivateLongTransaction_h 1

// The links to the header files to be included.

#include "FdoRdbms.h"
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsCommand.h"


class FdoRdbmsActivateLongTransaction : 
                            public FdoRdbmsCommand<FdoIActivateLongTransaction>
{

  friend class FdoRdbmsConnection;

  public:

    // Class Functions:
    //   Execute : Executes the command.
    //   GetName : Returns the name of the long transaction to be activated set
    //             for the command.
    //   SetName : Sets the name of the long transaction to be activated for
    //             this command.

    FdoString *GetName ();
    void      SetName  (FdoString *value);
    void      Execute  ();

  protected:

    // Class Variables:

    // Class Functions:

            FdoRdbmsActivateLongTransaction  ();
            FdoRdbmsActivateLongTransaction  (FdoIConnection *connection);
    virtual ~FdoRdbmsActivateLongTransaction ();

  private:

    // Class Enumerations:

    // Class Variables:
    //   fdo_rdbms_connection : References the FDO RDBMS DBI connection.
    //   lt_name              : Holds the name of the long transaction provided
    //                          to this command.

    FdoString          *lt_name;

    FdoRdbmsConnection *fdo_rdbms_connection;

    // Class Functions:
    //   ClearMemory : The function frees memory used by the class variables.
    //   SetToZero   : Initializes the class variables.

    void ClearMemory ();
    void SetToZero   ();

};  //  class FdoRdbmsActivateLongTransaction

#endif
