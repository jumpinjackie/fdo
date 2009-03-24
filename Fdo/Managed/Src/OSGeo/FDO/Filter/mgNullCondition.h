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
ref class Identifier;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

BEGIN_NAMESPACE_OSGEO_FDO_FILTER

interface class IFilterProcessor;

/// \ingroup (OSGeoFDOFilter)
/// \brief
/// The NullCondition class derives from SearchCondition and can be used to test
/// if the value of a specified data property is null.
public ref class NullCondition : public NAMESPACE_OSGEO_FDO_FILTER::SearchCondition
{
public:
    /// \brief
    /// Constructs a default instance of an NullCondition.
    /// 
    /// \return
    /// Returns an NullCondition
    /// 
	NullCondition();

    /// \brief
    /// Constructs an instance of an NullCondition using the specified arguments.
    /// 
    /// \param propertyName 
    /// Input name
    /// 
    /// \return
    /// Returns an NullCondition
    /// 
	NullCondition(System::String^ propertyName);

    /// \brief
    /// Constructs an instance of an NullCondition using the specified arguments.
    /// 
    /// \param propertyName 
    /// Input name
    /// 
    /// \return
    /// Returns an NullCondition
    /// 
	NullCondition(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ propertyName);

    /// \brief
    /// Gets the name of the data property.
    /// 
    /// \return
    /// Returns Identifier
    /// 
    /// \brief
    ///  Sets the name of the data property.
    /// 
    /// \param value 
    /// Input name
    /// 
    /// \return
    /// Returns nothing
    /// 
    property NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ PropertyName
    {
        NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ value);
    }

    /// \brief
    /// Overrides Filter.Process to pass NullCondition to the
    /// appropriate filter processor operation.
    /// 
    /// \param processor 
    /// Input filter processor
    /// 
    /// \return
    /// Returns nothing
    /// 
	System::Void Process(NAMESPACE_OSGEO_FDO_FILTER::IFilterProcessor^ processor);

    /// \brief
    /// Constructs a NullCondition object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	NullCondition(System::IntPtr unmanaged, System::Boolean autoDelete);

internal:
	inline FdoNullCondition* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_FILTER


