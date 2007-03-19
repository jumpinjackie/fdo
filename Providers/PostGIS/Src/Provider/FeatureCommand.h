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
    virtual void SetFeatureClassName(FdoIdentifier* classIdentifier);
 	
    /// Set name of the class to be operated upon as an identifier.
    virtual void SetFeatureClassName(FdoString* className);

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

    ////
    //// FdoICommand interface
    ////
    //
    ///// Return collection of command parameters and its values.
    //virtual FdoParameterValueCollection* GetParameterValues();

private:

    // Identifier of the class to be operated upon.
    FdoPtr<FdoIdentifier> mClassIdentifier;

    // Filter expression tree (most likely produced by a query builder).
    FdoPtr<FdoFilter> mFilter;

}; // class FeatureCommand

template <typename T>
FeatureCommand<T>::FeatureCommand(Connection* conn) :
    Command<T>(conn),
    mClassIdentifier(NULL),
    mFilter(NULL)
{
    // idle
}

template <typename T>
FeatureCommand<T>::~FeatureCommand()
{
    // idle
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

//// TODO - REMOVE IT!
//template <typename T>
//FdoParameterValueCollection* FeatureCommand<T>::GetParameterValues()
//{
//    assert(!"NOT IMPLEMENTED");
//    return NULL;
//}

///////////////////////////////////////////////////////////////////////////////
// FdoIFeatureCommand interface
///////////////////////////////////////////////////////////////////////////////

template <typename T>
FdoIdentifier* FeatureCommand<T>::GetFeatureClassName()
{
    FDOLOG_MARKER("FeatureCommand::GetFeatureClassName");

    FDO_SAFE_ADDREF(mClassIdentifier.p);
    return mClassIdentifier.p;
}

template <typename T>
void FeatureCommand<T>::SetFeatureClassName(FdoIdentifier* classIdentifier)
{
    FDOLOG_MARKER("FeatureCommand::SetFeatureClassName(classIdentifier)");
    if (NULL != classIdentifier)
    {
        FDOLOG_WRITE(L"Class name: %s", classIdentifier->GetName());
    }

    mClassIdentifier = classIdentifier;
    FDO_SAFE_ADDREF(mClassIdentifier.p);
}

template <typename T>
void FeatureCommand<T>::SetFeatureClassName(FdoString* className)
{
    FDOLOG_MARKER("FeatureCommand::SetFeatureClassName");

    FdoPtr<FdoIdentifier> cid;
    if (NULL != className)
        cid = FdoIdentifier::Create(className);
    else
        cid = NULL;
        
    SetFeatureClassName(cid);
}

template <typename T>
FdoFilter* FeatureCommand<T>::GetFilter()
{
    FDOLOG_MARKER("FeatureCommand::GetFilter");
    //FDO_SAFE_ADDREF(mFilter.p);
    //return mFilter.p;
    return NULL;
}

template <typename T>
void FeatureCommand<T>::SetFilter(FdoFilter* filter)
{
    FDOLOG_MARKER("FeatureCommand::SetFilter(filter)");

    mFilter = filter;
    FDO_SAFE_ADDREF(mFilter.p);
}

template <typename T>
void FeatureCommand<T>::SetFilter(FdoString* filterText)
{
    FDOLOG_MARKER("FeatureCommand::SetFilter");

    FdoPtr<FdoFilter> filter(FdoFilter::Parse(filterText));
    mFilter = filter;
    FDO_SAFE_ADDREF(mFilter.p);
}

}} // namespace fdo::postgis

#endif // FDOPOSTGIS_FEATURECOMMAND_H_INCLUDED
