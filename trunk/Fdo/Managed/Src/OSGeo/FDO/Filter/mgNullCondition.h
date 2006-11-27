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


#include "FDO\Filter\mgSearchCondition.h"

class FdoNullCondition;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
public __gc class Identifier;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

BEGIN_NAMESPACE_OSGEO_FDO_FILTER

public __gc __interface IFilterProcessor;

/// \brief
/// The FdoNullCondition class derives from FdoSearchCondition and can be used to test
/// if the value of a specified data property is null.
public __gc class NullCondition : public NAMESPACE_OSGEO_FDO_FILTER::SearchCondition
{
public:
    /// \brief
    /// Constructs a default instance of an FdoNullCondition.
    /// 
    /// \return
    /// Returns an FdoNullCondition
    /// 
	NullCondition();

    /// \brief
    /// Constructs an instance of an FdoNullCondition using the specified arguments.
    /// 
    /// \param propertyName 
    /// Input name
    /// 
    /// \return
    /// Returns an FdoNullCondition
    /// 
	NullCondition(System::String* propertyName);

    /// \brief
    /// Constructs an instance of an FdoNullCondition using the specified arguments.
    /// 
    /// \param propertyName 
    /// Input name
    /// 
    /// \return
    /// Returns an FdoNullCondition
    /// 
	NullCondition(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* propertyName);

    /// \brief
    /// Gets the name of the data property.
    /// 
    /// \return
    /// Returns FdoIdentifier
    /// 
	__property NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* get_PropertyName();

    /// \brief
    ///  Sets the name of the data property.
    /// 
    /// \param value 
    /// Input name
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_PropertyName(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier* value);

    /// \brief
    /// Overrides FdoFilter.Process to pass the FdoNullCondition to the
    /// appropriate filter processor operation.
    /// 
    /// \param processor 
    /// Input filter processor
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Process(NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessor* processor);

	NullCondition(System::IntPtr unmanaged, System::Boolean autoDelete);

public private:
	inline FdoNullCondition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_FILTER


