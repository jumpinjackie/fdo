/*
 * 
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
 */

#include "stdafx.h"

#include <sdeerno.h>

ArcSDEDeactivateLongTransactionCommand::ArcSDEDeactivateLongTransactionCommand (FdoIConnection *connection) :
    ArcSDECommand<FdoIDeactivateLongTransaction> (connection)
{
}

ArcSDEDeactivateLongTransactionCommand::~ArcSDEDeactivateLongTransactionCommand (void)
{
}

/// <summary>Executes the deactivate long transaction command. An exception is
/// thrown if there is no long tranasaction active.</summary>
/// <returns>Returns nothing</returns> 
void ArcSDEDeactivateLongTransactionCommand::Execute ()
{
    FdoPtr<ArcSDEConnection> connection;
    LONG active;

    // verify connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established (NULL)."));

    active = connection->GetActiveVersion ();
    if (SDE_DEFAULT == active)
        throw FdoException::Create (NlsMsgGet (ARCSDE_NO_ACTIVE_TRANSACTION, "No long transaction is active."));

    connection->SetActiveVersion (SDE_DEFAULT);
}

