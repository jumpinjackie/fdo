#ifndef _FDOCLIENTSERVICEEXCEPTION_H_
#define _FDOCLIENTSERVICEEXCEPTION_H_

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
//

#ifdef _WIN32
#pragma once
#endif

// Include definitions
#include <FdoStd.h>

/// \brief
///  
/// An Exception class through which ClientServices errors are transmitted back to
/// calling applications. Derives from base FdoException class. 
///
class FdoClientServiceException : public FdoException
{
/// \cond DOXYGEN-IGNORE
protected:
    /// Constructs a default instance of a FdoClientServiceException.
    FdoClientServiceException(void);

    /// Constructs an instance of a FdoClientServiceException using the exception
    /// message.
    FdoClientServiceException(FdoString* message);

    /// Constructs an instance of a FdoClientServiceException using the exception
    /// message, and the native error code
    FdoClientServiceException(FdoString* message, FdoInt64 nativeErrorCode);

    virtual void Dispose();
/// \endcond

public:

    /// \brief
    /// Default destructor for FdoClientServiceException.
    /// 
    /// \return
    /// Returns nothing.
    /// 
    FDO_API virtual ~FdoClientServiceException(void);

    /// \brief
    /// Creates a default instance of a FdoClientServiceException. 
    /// 
    /// \return
    /// Returns an default FdoClientServicesException object.
    /// 
    FDO_API static FdoClientServiceException* Create();

    /// \brief
    /// Creates an instance of a FdoClientServiceException using the exception message. 
    /// 
    /// \param message 
    /// The Exception message.
    /// 
    /// \return
    /// Returns an FdoClientServicesException object containing the specified error mesage.
    /// 
    FDO_API static FdoClientServiceException* Create(FdoString* message);

    /// \brief
    /// Creates an instance of a FdoClientServiceException using the exception message and the native error code. 
    /// 
    /// \param message 
    /// The Exception message.
    /// \param nativeErrorCode 
    /// Input the native error code of the error, which is returned by FDO data source
    /// 
    /// \return
    /// Returns an FdoClientServicesException object containing the specified error mesage.
    /// 
    FDO_API static FdoClientServiceException* Create(FdoString* message, FdoInt64 nativeErrorCode);

    /// \brief
    /// Gets the message associated with this exception. If no message and
    /// is associated with this exception then a null reference is
    /// returned.
    /// 
    /// \return
    /// Returns the error message.
    /// 
    ///  
    FDO_API virtual FdoString* ToString();
};
#endif


