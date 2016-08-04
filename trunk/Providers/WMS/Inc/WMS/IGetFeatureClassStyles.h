#ifndef IGETFEATURECLASSSTYLES_H
#define IGETFEATURECLASSSTYLES_H

//
// Copyright (C) 2004-2007  Autodesk, Inc.
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

#ifdef _WIN32
#pragma once
#endif

#include <FdoStd.h>
#include <Fdo/Commands/ICommand.h>
#include <Fdo/Connections/IConnection.h>

/// \brief 
/// The FdoWmsIGetFeatureClassStyles interface defines the 
/// FdoWmsGetFeatureClassStyles command, which
/// retrieves the layer styles available for the specfic FeatureClass. 
/// The Execute operation returns an FdoStringCollection object that 
/// contains the names of the styles.
/// \brief
class FdoWmsIGetFeatureClassStyles : public FdoICommand
{
    friend class FdoIConnection;

public:
    /// \brief
    /// Gets the name of the FeatureClass for which all supported style names
	/// will be retrieved.
    /// 
    /// \return
    /// Returns the FeatureClass name
    /// 
    FDO_API virtual FdoString* GetFeatureClassName() = 0;

    /// \brief
    /// Sets the name of the FeatureClass for which all supported style names
    /// will be retrieved. This function is mandatory; if not specified, 
    /// execution of the command will throw an exception. 
    /// 
    /// \param value 
    /// Input the FeatureClass name
    /// 
    /// \return
    /// Returns nothing
    /// 
    FDO_API virtual void SetFeatureClassName(FdoString* value) = 0;

    /// \brief
    /// Executes the FdoWmsIGetFeatureClassCRSNames command and returns a 
    /// FdoStringCollection, which contains all style names supported by 
    /// the specified FeatureClass.
    /// 
    /// \return
    /// Returns a collection containing the style names that are supported 
	/// by the specified FeatureClass.
    /// 
    FDO_API virtual FdoStringCollection* Execute() = 0;
};
#endif

