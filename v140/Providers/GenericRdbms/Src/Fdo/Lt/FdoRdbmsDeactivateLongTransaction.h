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
// | The file contains the definition of the class FdoRdbmsDeactivateLongTrans-
// | action. The class deactivates the currently active long transaction and
// | activates the root long transaction. Any attempt to deactivate the root
// | long transaction is ignored.
// =============================================================================

#ifndef FdoRdbmsDeactivateLongTransaction_h
#define FdoRdbmsDeactivateLongTransaction_h 1

// The links to the header files to be included.

#include "FdoRdbms.h"
#include "FdoRdbmsConnection.h"
#include "FdoRdbmsCommand.h"


class FdoRdbmsDeactivateLongTransaction : 
                        public FdoRdbmsCommand<FdoIDeactivateLongTransaction>
{

  friend class FdoRdbmsConnection;

  public:

    // Class Functions:
    //   Execute : Executes the command.

    void Execute  ();

  protected:

    // Class Variables:

    // Class Functions:

            FdoRdbmsDeactivateLongTransaction  ();
            FdoRdbmsDeactivateLongTransaction  (FdoIConnection *connection);
    virtual ~FdoRdbmsDeactivateLongTransaction ();

  private:

    // Class Enumerations:

    // Class Variables:
    //   fdo_rdbms_connection : References the FDO RDBMS DBI connection.

    FdoRdbmsConnection *fdo_rdbms_connection;

    // Class Functions:


};  //  class FdoRdbmsDeactivateLongTransaction

#endif
