#ifndef _EXPRESSIONEXCEPTION_H_
#define _EXPRESSIONEXCEPTION_H_
// 

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

#include <FdoStd.h>

/// \brief
/// FdoExpressionException is the exception type thrown from classes in the
/// Expression package. It derives from a language level exception class that is
/// environment specific.
class FdoExpressionException : public FdoException
{
/// \cond DOXYGEN-IGNORE
protected:
    /// Constructs a default instance of an FdoExpressionException.
    FdoExpressionException();

    /// Constructs an instance of an FdoExpressionException using the specified
    /// arguments.
    FdoExpressionException(FdoString* message);

    /// Constructs an instance of an FdoExpressionException using the specified
    /// arguments.
    FdoExpressionException(FdoString* message, FdoException* cause);

    /// Constructs an instance of an FdoExpressionException using the specified
    /// arguments.
    FdoExpressionException(FdoString* message, FdoException* cause, FdoInt64 nativeErrorCode);

    virtual ~FdoExpressionException();

    virtual void Dispose();
/// \endcond

public:
    /// \brief
    /// Constructs a default instance of an FdoExpressionException.
    /// 
    /// \return
    /// Returns FdoExpressionException
    /// 
    FDO_API static FdoExpressionException* Create();

    /// \brief
    /// Constructs an instance of an FdoExpressionException using the specified arguments.
    /// 
    /// \param message 
    /// Input message text
    /// 
    /// \return
    /// Returns FdoExpressionException
    /// 
    FDO_API static FdoExpressionException* Create(FdoString* message);

    /// \brief
    /// Constructs an instance of an FdoExpressionException using the specified arguments.
    /// 
    /// \param message 
    /// Input message text
    /// \param nativeErrorCode 
    /// Input the native error code of the error, which is returned by FDO data source
    /// 
    /// \return
    /// Returns FdoExpressionException
    /// 
    FDO_API static FdoExpressionException* Create(FdoString* message, FdoInt64 nativeErrorCode);

    /// \brief
    /// Constructs an instance of an FdoExpressionException using the specified arguments.
    /// 
    /// \param message 
    /// Input message text
    /// \param cause 
    /// Input cause of exception
    /// 
    /// \return
    /// Returns FdoExpressionException
    /// 
    FDO_API static FdoExpressionException* Create(FdoString* message, FdoException* cause);

    /// \brief
    /// Constructs an instance of an FdoExpressionException using the specified arguments.
    /// 
    /// \param message 
    /// Input message text
    /// \param cause 
    /// Input cause of exception
    /// \param nativeErrorCode 
    /// Input the native error code of the error, which is returned by FDO data source
    /// 
    /// \return
    /// Returns FdoExpressionException
    /// 
    FDO_API static FdoExpressionException* Create(FdoString* message, FdoException* cause, FdoInt64 nativeErrorCode);
};
#endif


