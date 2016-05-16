//
// Copyright (C) 2006 Refractions Research, Inc.
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
#ifndef FDOSMPHPOSTGISFKEY_H
#define FDOSMPHPOSTGISFKEY_H

#include "../../../SchemaMgr/Ph/Fkey.h"
#include <Sm/Ph/Table.h>

/// Defines Foreign Key type for PostGIS provider.
///
class FdoSmPhPostGisFkey : public FdoSmPhGrdFkey
{
public:

    /// Constructs an instance of a Foreign Key object.
    ///
    /// \param
    ///  name [in] - name of Foreign Key.
    ///
    /// \param
    /// fkeytable [in] - name of foreign table.
    ///
    /// \param
    ///	pkeyTableName [in] - name of primary table referenced by
    /// this foreign key.
    ///
    /// \param
	/// pkeyTableOwner [in] - name of primary table owner.
    ///
    /// \param
    /// state [in] - indicates whether foreign key is new or already exists.
    ///
    FdoSmPhPostGisFkey(FdoStringP name, const FdoSmPhTable* fkeyTable,
        FdoStringP pkeyTableName,
        FdoStringP pkeyTableOwner,
        FdoSchemaElementState state = FdoSchemaElementState_Added);

    /// Destructor.
    /// Currently it is empty.
    ~FdoSmPhPostGisFkey();

protected:
    virtual FdoSmPhColumnP FindPkeyColumn( FdoSmPhTableP pkTable, FdoStringP columnName );
};

/// \brief
/// Declaration of smart-pointer type.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmPhPostGisFkey> FdoSmPhPostGisFkeyP;

#endif // FDOSMPHPOSTGISFKEY_H
