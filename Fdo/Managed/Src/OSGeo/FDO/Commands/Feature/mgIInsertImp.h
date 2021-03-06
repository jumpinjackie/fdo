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

#include "FDO\Commands\mgICommandImp.h"
#include "FDO\Commands\Feature\mgIInsert.h"

class FdoIInsert;

BEGIN_NAMESPACE_OSGEO_FDO_EXPRESSION
ref class Identifier;
END_NAMESPACE_OSGEO_FDO_EXPRESSION

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS
ref class PropertyValueCollection;
ref class BatchParameterValueCollection;
END_NAMESPACE_OSGEO_FDO_COMMANDS

BEGIN_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE
interface class IFeatureReader;

/// \ingroup (OSGeoFDOCommandsFeature)
/// \brief
/// The IInsertImp class is a concrete implementation class for interface IInsert.
/// The IInsert interface defines the Insert command, which inserts a new
/// instance of a given class. Input to the insert command includes the name of
/// the class and a collection of property values. Properties that are
/// not specified and do not have a default value will be assigned a null value
/// or an exception will be thrown if the property is required. The insert
/// command can insert instances at global scope or instances nested within an
/// object collection property. Instances at global scope are referred to simply
/// by the class name. Instances at a nested scope (i.e. instances within a
/// object collection property) are referred to by the containing class name,
/// followed by a '.', followed by the object collection property name.
private ref class IInsertImp : public NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp, 
                                public NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IInsert
{
public:
    /// \brief
    /// Gets the name of the class to be operated upon as an Identifier.
    /// 
    /// \return
    /// Returns the class name.
    /// 
    /// \brief
    /// Sets the name of the class to be operated upon as an Identifier.
    /// 
    /// \param value 
    /// Input the identifier for the class.
    /// 
    /// \return
    /// Returns nothing
    /// 
    virtual property NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ FeatureClassName
    {
        NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ get();
        System::Void set(NAMESPACE_OSGEO_FDO_EXPRESSION::Identifier^ value);
    }

    /// \brief
    /// Sets the name of the class to be operated upon as an Identifier.
    /// 
    /// \param value 
    /// Input the class name.
    /// 
    /// \return
    /// Returns nothing
    /// 
	virtual System::Void SetFeatureClassName(System::String^ value);

    /// \brief
    /// Gets the PropertyValueCollection that specifies the names and values
    /// of the properties for the instance to be inserted.
    /// 
    /// \return
    /// Returns the list of properties and their values.
    /// 
    property NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection^ PropertyValues
    {
        virtual NAMESPACE_OSGEO_FDO_COMMANDS::PropertyValueCollection^ get();
    }

    /// \brief
    /// Gets the BatchParameterValueCollection that can be used for optimized
    /// batch inserts of multiple features with a single insert command. Batch
    /// inserts can be performed by using Parameters for each of the property
    /// values, then adding collections of parameter values to the
    /// BatchParameterValueCollection. Each ParameterValueCollection in the
    /// BatchParameterValueCollection should contain one ParameterValue for each
    /// of the parameters specified for property values.
    /// 
    /// \return
    /// Returns BatchParameterValueCollection
    /// 
    property NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection^ BatchParameterValues
    {
        virtual NAMESPACE_OSGEO_FDO_COMMANDS::BatchParameterValueCollection^ get();
    }

    /// \brief
    /// Executes the insert command and returns a reference to an
    /// IFeatureReader. Some feature providers can generate automatic identity
    /// values for features. This will happen automatically as the features are
    /// inserted. The returned IFeatureReader allows the client to obtain the
    /// automatic identity property value(s) of newly inserted object(s). The
    /// returned feature reader at a minimum will read the unique identity
    /// properties of the objects just inserted. Multiple objects will be
    /// returned through the reader in the case of a batch insert.
    /// 
    /// \return
    /// Returns an IFeatureReader
    /// 
	virtual NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE::IFeatureReader^ Execute();

internal:
	IInsertImp(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_COMMANDS::ICommandImp(unmanaged, autoDelete)
	{

	}

	inline FdoIInsert* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_COMMANDS_FEATURE


