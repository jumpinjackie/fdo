#ifndef IGETFEATURECLASSCRSNAMES_H
#define IGETFEATURECLASSCRSNAMES_H

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

/// The FdoWmsIGetFeatureClassCRSNames interface defines the 
/// FdoWmsGetFeatureClassCRSNames command, which retrieves the 
/// CRS names available from the specified FeatureClass. 
/// The Execute operation returns an FdoStringCollection 
/// that contains the CRS names.
/// \brief
class FdoWmsIGetFeatureClassCRSNames : public FdoICommand
{
    friend class FdoIConnection;

public:
    /// \brief
    /// Gets the name of the FeatureClass for which all supported CRS names
	/// will be retrieved.
    /// 
    /// \return
    /// Returns the FeatureClass name
    /// 
    FDO_API virtual FdoString* GetFeatureClassName() = 0;

    /// \brief
    /// Sets the name of the FeatureClass for which all supported CRS names
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
    /// FdoStringCollection, which contains all CRS names supported by 
    /// the specified FeatureClass.
    /// 
    /// \return
    /// Returns a collection containing the CRS names that are supported 
	/// by the specified FeatureClass.
    /// 
    FDO_API virtual FdoStringCollection* Execute() = 0;
};
#endif

