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

class FdoParameterValue;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
public __gc class LiteralValue;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS

/// \ingroup (OSGeoFDOCommands)
/// \brief
/// The ParameterValue class specifies a value for a particular parameter.
/// Instances of this class are used to specify a value to bind to a parameter
/// when a command is executed.
public __sealed __gc class ParameterValue : public NAMESPACE_OSGEO_RUNTIME::Disposable
{
public:
    /// \brief
    /// Constructs a default instance of an ParameterValue.
    /// 
	ParameterValue();
	
    /// \brief
    /// Constructs an instance of an ParameterValue using the specified arguments.
    /// 
    /// \param name 
    /// Input the name of the parameter value
    /// 
	ParameterValue(System::String* name);

    /// \brief
    /// Constructs an instance of an ParameterValue using the specified arguments.
    /// 
    /// \param name 
    /// Input the name of the parameter value
    /// \param value 
    /// Input the literal parameter value
    /// 
	ParameterValue(System::String* name, NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValue* value);

    /// \brief
    /// Gets the parameter name the value should bind to as a string.
    /// 
    /// \return
    /// Returns the parameter name
    /// 
	__property System::String* get_Name();

    /// \brief
    /// Sets the parameter name the value should bind to as a string.
    /// 
    /// \param value 
    /// Input the parameter name
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Name(System::String* value);

    /// \brief
    /// Gets the value to bind to the parameter as an LiteralValue.
    /// 
    /// \return
    /// Returns literal value
    /// 
	__property NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValue* get_Value();

    /// \brief
    /// Sets the value to bind to the parameter as an LiteralValue.
    /// 
    /// \param value 
    /// Input the literal value
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Value(NAMESPACE_OSGEO_FDO_EXPRESSION::LiteralValue* value);

    /// \brief
    /// Sets the value to bind to the parameter as a string.
    /// 
    /// \param value 
    /// Input the string value to bind to the parameter
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_Value(System::String* value);

/// \cond DOXYGEN-IGNORE
protected:
	System::Void ReleaseUnmanagedObject();
/// \endcond

public private:
	ParameterValue(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_RUNTIME::Disposable(unmanaged, autoDelete)
	{

	}

	inline FdoParameterValue* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_COMMANDS


