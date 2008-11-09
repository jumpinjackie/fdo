#ifndef FDORDBMSPVCUPDATEHANDLER_H
#define FDORDBMSPVCUPDATEHANDLER_H    1
/*
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
#include "Fdo/Pvc/FdoRdbmsPvcHandler.h"

class GdbiStatement;

//
// Defines an abstract interface for the Property Value Collection handlers
class FdoRdbmsPvcUpdateHandler: FdoRdbmsPvcHandler
{
private:
	char mTmpStringValue[GDBI_MAXIMUM_STRING_SIZE+1];

protected:
	FdoRdbmsConnection *mFdoConnection;

	virtual void AdditionalCriteria( FdoPropertyValueCollection  *propValCollection, FdoStringP & updateString, int &bindIndex );

	virtual void AdditionalBinds( GdbiStatement *statement, FdoRdbmsPvcBindDef  *values, int & bindIndex  );

public:
	FdoRdbmsPvcUpdateHandler( );

	FdoRdbmsPvcUpdateHandler( FdoRdbmsConnection *connection );

	virtual ~FdoRdbmsPvcUpdateHandler();

    /// <summary>Given a PVC operation, execute the handler.</summary>
    /// <returns>Returns the number of affected objects</returns> 
    virtual long Execute( const FdoSmLpClassDefinition *classDefinition, FdoPropertyValueCollection  *propValCollection, bool revNumberUpdate, bool handleForeignAutoincrementedId );
};

#endif

