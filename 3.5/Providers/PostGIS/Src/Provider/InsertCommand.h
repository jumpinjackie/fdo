//
// Copyright (C) 2007 Refractions Research, Inc. 
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#ifndef FDOPOSTGIS_INSERTCOMMAND_H_INCLUDED
#define FDOPOSTGIS_INSERTCOMMAND_H_INCLUDED

#include "Command.h"
#include <Fdo/Commands/Feature/IInsert.h>

//
// Forward declarations
//
class FdoIdentifier;
class FdoIFeatureReader;
class FdoPropertyValueCollection;
class FdoBatchParameterValueCollection;

namespace fdo { namespace postgis {

// Forward declarations of internal types
class Connection;

/// Implementation of interface of Insert command for PostGIS provider.
/// The Insert command inserts a new instance of a given class to the datastore.
///
class InsertCommand : public Command<FdoIInsert>
{
public:

    /// Create command for given connection instance.
    InsertCommand(Connection* conn);

    //
    // FdoIInsert interface
    //

    /// Get name of the class to be operated upon as an FdoIdentifier.
    virtual FdoIdentifier* GetFeatureClassName();
    
    /// Set name of the class to be operated upon as an FdoIdentifier.
    virtual void SetFeatureClassName(FdoIdentifier* classIdentifier);
    
    /// Set name of the class to be operated upon as an FdoIdentifier.
    virtual void SetFeatureClassName(FdoString* className);
    
    /// Get collection of names and values of properties for the instance to be inserted.
    virtual FdoPropertyValueCollection* GetPropertyValues();
    
    /// Get collection of parameters dedicated for batch insert commands.
    /// It can be used for optimized batch inserts of multiple features
    /// with a single insert command.
    virtual FdoBatchParameterValueCollection* GetBatchParameterValues();
    
    /// Execute the insert command and return pointer to FdoIFeatureReader interface.. 
    virtual FdoIFeatureReader* Execute();

protected:

    /// Destructor.
    virtual ~InsertCommand();

private:

    //
    // Private data members
    //

    typedef Command<FdoIInsert> Base;

    // Identifier of the class to be operated upon.
    FdoPtr<FdoIdentifier> mClassIdentifier;

    FdoPtr<FdoPropertyValueCollection> mProperties;
    FdoPtr<FdoBatchParameterValueCollection> mBatchParameters;
    
    //
    // Private operations
    //
    
    // Utility template function acting as a proxy preparing internal
    // collection to return to a user, also creating new collection if required.
    template <typename T>
    FdoPtr<T> GetCollection(FdoPtr<T> col);

};

template <typename T>
FdoPtr<T> InsertCommand::GetCollection(FdoPtr<T> col)
{
    if (NULL == col)
    {
        col = T::Create();
        assert(NULL != col);
    }

    FDO_SAFE_ADDREF(col.p);
    return col.p;
}

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_INSERTCOMMAND_H_INCLUDED
