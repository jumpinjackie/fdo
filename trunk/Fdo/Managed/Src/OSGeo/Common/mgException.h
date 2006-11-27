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

[System::SerializableAttribute]
public __gc class Exception : public System::Exception
{
public:
	static NAMESPACE_OSGEO_COMMON::Exception* Create(System::IntPtr ex);

public:
	NAMESPACE_OSGEO_COMMON::Exception() : System::Exception()
	{
		
	}

    /// \brief
    /// Returns an instance of a FdoException using the specified arguments.
    /// 
    /// \param message 
    /// Input the error message
    /// 
    /// \return
    /// Returns the FdoException object
    /// 
	NAMESPACE_OSGEO_COMMON::Exception(String* msg) : System::Exception(msg)
	{
		
	}

    /// \brief
    ///  Returns an instance of a FdoException using the specified arguments.
    /// 
    /// \param message 
    /// Input the error message
    /// \param cause 
    /// Input the cause of the error
    /// 
    /// \return
    /// Returns the FdoException object
    /// 
	NAMESPACE_OSGEO_COMMON::Exception(System::String* msg, System::Exception* cause) : System::Exception(msg, cause)
	{
		
	}

protected:
	NAMESPACE_OSGEO_COMMON::Exception(System::Runtime::Serialization::SerializationInfo* info, System::Runtime::Serialization::StreamingContext context) : System::Exception(info, context)
	{
		
	}
};
END_NAMESPACE_OSGEO_COMMON


