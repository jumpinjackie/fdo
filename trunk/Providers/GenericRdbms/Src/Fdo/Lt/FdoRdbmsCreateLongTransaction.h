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
// | The file contains the definition of the class FdoRdbmsCreateLongTransac-
// | tion. The class creates a new long transaction using the given parameters.
// | Input to the command is the name of the long transaction to be created and
// | an optional description.
// =============================================================================

#ifndef FdoRdbmsCreateLongTransaction_h
#define FdoRdbmsCreateLongTransaction_h 1

// The links to the header files to be included.

#include "FdoRdbms.h"
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsCommand.h"


class FdoRdbmsCreateLongTransaction : public FdoRdbmsCommand<FdoICreateLongTransaction>
{

  friend class FdoRdbmsConnection;

  public:

    // Class Functions:
    //   Execute        : Executes the command.
    //   GetDescription : Returns the description of the long transaction to be
    //                    created, set for the command
    //   GetName        : Returns the name of the long transaction to be created,
    //                    set for the command.
    //   SetDescription : Sets the description for the long transaction to be
    //                    created when the command is executed.
    //   SetName        : Sets the name of the long transaction to be created
    //                    when the command is executed.

    FdoString *GetDescription ();
    FdoString *GetName        ();
    void      SetDescription  (FdoString *value);
    void      SetName         (FdoString *value);
    void      Execute         ();

  protected:

    // Class Functions:

            FdoRdbmsCreateLongTransaction  ();
            FdoRdbmsCreateLongTransaction  (FdoIConnection *connection);
    virtual ~FdoRdbmsCreateLongTransaction ();

  private:

    // Class Enumerations:

    // Class Variables:
    //   lt_name              : Holds the name of the long transaction provided
    //                          to this command.
    //   lt_description       : Holds the description of the long transaction
    //                          provided to this command.
    //   dbi_connection       : References the DBI connection.
    //   fdo_rdbms_connection : References the FDO RDBMS DBI connection.

    FdoString          *lt_name,
                       *lt_description;

    DbiConnection      *dbi_connection;
    FdoRdbmsConnection *fdo_rdbms_connection;

    // Class Functions:
    //   ClearMemory : The function frees memory used by the class variables.
    //   SetToZero   : Initializes the class variables.

    void ClearMemory ();
    void SetToZero   ();

};  //  class FdoRdbmsCreateLongTransaction

#endif
