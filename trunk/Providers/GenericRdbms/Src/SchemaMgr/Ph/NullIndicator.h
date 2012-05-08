#ifndef FDOSMPHGRDNULLINDICATOR_H
#define FDOSMPHGRDNULLINDICATOR_H		1
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


#include <Sm/Ph/NullIndicator.h>
#include "../../Gdbi/GdbiConnection.h"
#include "../../Gdbi/GdbiCommands.h"

// Represents an RDBMS provider NULL indicator, which indicates whether an 
// accompanying value is NULL. Wraps provider-specific null indicators, 
// which are "bound" into SQL statements.

class FdoSmPhGrdNullIndicator : public FdoSmPhNullIndicator
{
public:

    /// Creates a new null indicator
	//
    /// Parameters:
    /// 	connection: RDBMS connection object. Provides RDBMS-specific
    ///         null indicator handling.
    FdoSmPhGrdNullIndicator( GdbiConnection* connection);

    virtual ~FdoSmPhGrdNullIndicator(void);

    /// Get the provider-specific null indicator
    virtual _NI_TYPE* GetDbIndicator();

    // Set this indicator to NULL.
    virtual void SetNull();

    // Set this indicator to not NULL.
    virtual void SetNNull();

    // Get current indicator value.
    virtual bool IsNull();

protected:
    /// unused constructor only for building on Linux
    FdoSmPhGrdNullIndicator() {}

private:
    _NI_TYPE* mNullInd;             // RDBMS specific NULL indicatory
    GdbiConnection* mConnection;    // RDBMS connection.
};

typedef FdoPtr<FdoSmPhGrdNullIndicator> FdoSmPhGrdNullIndicatorP;

#endif


