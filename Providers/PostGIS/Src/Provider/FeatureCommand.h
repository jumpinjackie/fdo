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
#ifndef FDOPOSTGIS_FEATURECOMMAND_H_INCLUDED
#define FDOPOSTGIS_FEATURECOMMAND_H_INCLUDED

#include "Command.h"
#include "Connection.h"

#include <cassert>

//
// Forward declarations
//
class FdoFilter;
class FdoIdentifier;
class FdoParameterValueCollection;

namespace fdo { namespace postgis {

/// Implementation of common interface for commands operating on
/// instances of feature class, in PostGIS provider for FDO.
///
template <typename T>
class FeatureCommand : public Command<T>
{
public:

    //
    // FdoIFeatureCommand interface
    //

    /// Get name of the class to be operated upon as an identifier.
    virtual FdoIdentifier* GetFeatureClassName();
 	
    /// Set name of the class to be operated upon as an identifier.
    virtual void SetFeatureClassName(FdoIdentifier* value);
 	
    /// Set name of the class to be operated upon as an identifier.
    virtual void SetFeatureClassName(FdoString* value);

    /// Get filter as a filter tree.
    virtual FdoFilter* GetFilter();

    /// Set filter as a Filter tree.
    virtual void SetFilter(FdoFilter* value);
 	
    /// Set filter as a Filter tree.
    virtual void SetFilter(FdoString* value);

protected:

    /// Construct instance of a command for the given connection.
    FeatureCommand(Connection* conn);

    /// Destructor.
    virtual ~FeatureCommand();

    //
    // FdoIDisposable interface
    //

    virtual void Dispose();

    //
    // FdoICommand interface
    //
    
    /// Return collection of command parameters and its values.
    virtual FdoParameterValueCollection* GetParameterValues();

private:
	
}; // class FeatureCommand

template <typename T>
FeatureCommand<T>::FeatureCommand(Connection* conn) : Command<T>(conn)
{
}

template <typename T>
FeatureCommand<T>::~FeatureCommand()
{
}

template <typename T>
inline void FeatureCommand<T>::Dispose()
{
    //FDOLOG_MARKER("FeatureCommand::#Dispose()");

    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoICommand interface
///////////////////////////////////////////////////////////////////////////////

template <typename T>
FdoParameterValueCollection* FeatureCommand<T>::GetParameterValues()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// FdoIFeatureCommand interface
///////////////////////////////////////////////////////////////////////////////

template <typename T>
FdoIdentifier* FeatureCommand<T>::GetFeatureClassName()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

template <typename T>
void FeatureCommand<T>::SetFeatureClassName(FdoIdentifier* value)
{
    assert(!"NOT IMPLEMENTED");
}

template <typename T>
void FeatureCommand<T>::SetFeatureClassName(FdoString* value)
{
    assert(!"NOT IMPLEMENTED");
}

template <typename T>
FdoFilter* FeatureCommand<T>::GetFilter()
{
    assert(!"NOT IMPLEMENTED");
    return NULL;
}

template <typename T>
void FeatureCommand<T>::SetFilter(FdoFilter* value)
{
    assert(!"NOT IMPLEMENTED");
}

template <typename T>
void FeatureCommand<T>::SetFilter(FdoString* value)
{
    assert(!"NOT IMPLEMENTED");
}

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_FEATURECOMMAND_H_INCLUDED
