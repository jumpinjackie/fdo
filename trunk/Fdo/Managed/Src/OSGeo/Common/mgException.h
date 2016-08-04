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

#pragma once

class FdoException;

BEGIN_NAMESPACE_OSGEO_COMMON

/// \ingroup (OSGeoFDOCommon)
/// \brief
/// Exception is a root class for the exception type thrown from classes. It derives from a language 
/// level exception class that is environment specific.
[System::SerializableAttribute]
public ref class Exception : public System::Exception
{
public:
    /// \brief
    /// Constructs an Exception managed object based on an unmanaged instance of the object
    /// 
    /// \param ex 
    /// Input A Pointer to the unmanaged object.
    /// 
	static NAMESPACE_OSGEO_COMMON::Exception^ Create(System::IntPtr ex);

    /// \brief
    ///  Returns the native error code of the underlying provider.
    /// 
    ///
    /// \returns
    /// The native error code of the underlying provider
    /// 
    System::Int64 GetNativeErrorCode()
    {
        return m_nativeErrorCode;
    }

public:
    /// \brief
    /// Constructs a new empty Exception object
    /// 
	NAMESPACE_OSGEO_COMMON::Exception() : System::Exception()
	{
		
	}

    /// \brief
    /// Creates an instance of an Exception using the specified arguments.
    /// 
    /// \param msg 
    /// Input the error message
    /// 
	NAMESPACE_OSGEO_COMMON::Exception(String^ msg) : System::Exception(msg), m_nativeErrorCode(0)
	{
		
	}

    /// \brief
    /// Creates an instance of an Exception using the specified arguments.
    /// 
    /// \param msg 
    /// Input the error message
    ///
    /// \param nativeErrroCode 
    /// Input the native error code
    /// 
    NAMESPACE_OSGEO_COMMON::Exception(String^ msg, System::Int64 nativeErrorCode) 
        : System::Exception(msg), m_nativeErrorCode(nativeErrorCode)
    {
    
    }

    /// \brief
    ///  Returns an instance of a Exception using the specified arguments.
    /// 
    /// \param msg 
    /// Input the error message
    ///
    /// \param cause 
    /// Input the cause of the error
    /// 
	NAMESPACE_OSGEO_COMMON::Exception(System::String^ msg, System::Exception^ cause) 
        : System::Exception(msg, cause), m_nativeErrorCode(0)
	{
		
	}

    /// \brief
    ///  Returns an instance of a Exception using the specified arguments.
    /// 
    /// \param msg 
    /// Input the error message
    ///
    /// \param cause 
    /// Input the cause of the error
    ///
    /// \param nativeErrroCode 
    /// Input the native error code
    /// 
    NAMESPACE_OSGEO_COMMON::Exception(System::String^ msg, System::Exception^ cause, System::Int64 nativeErrorCode) 
        : System::Exception(msg, cause), m_nativeErrorCode(nativeErrorCode)
    {

    }

    

/// \cond DOXYGEN-IGNORE
protected:
	NAMESPACE_OSGEO_COMMON::Exception(System::Runtime::Serialization::SerializationInfo^ info, System::Runtime::Serialization::StreamingContext context) : System::Exception(info, context)
	{
		
	}

    System::Int64   m_nativeErrorCode;
/// \endcond
};
END_NAMESPACE_OSGEO_COMMON


