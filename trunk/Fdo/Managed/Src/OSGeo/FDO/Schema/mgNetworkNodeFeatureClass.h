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

#pragma  once

#include "FDO\Schema\mgNetworkFeatureClass.h"
#include "FDO\Schema\mgClassType.h"

class FdoNetworkNodeFeatureClass;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __gc class AssociationPropertyDefinition;

/// \ingroup (OSGeoFDOSchema)
/// \brief
/// NetworkNodeFeature derives from NetworkFeature and can be used to represent a 
/// logical node feature.
/// <p><b>Note:</b> <span class="red_text">This Help topic is provided for informational use only. There is
/// no interface or support provided. OSGeo reserves the right to change
/// the software related to the content herein.</span>
public __gc class NetworkNodeFeatureClass : public NAMESPACE_OSGEO_FDO_SCHEMA::NetworkFeatureClass
{
public:
    /// \brief
    /// Constructs a default instance of an NetworkNodeFeatureClass.
    /// 
	NetworkNodeFeatureClass();

    /// \brief
    /// Constructs an instance of an NetworkNodeFeatureClass using the specified arguments.
    /// 
    /// \param name 
    /// Input name
    /// \param description 
    /// Input description
    /// 
	NetworkNodeFeatureClass(System::String* name, System::String* description);

    /// \brief
    /// Gets the concrete class type.
    /// 
    /// \return
    /// Returns class type
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::ClassType get_ClassType();

    /// \brief
    /// Gets a reference to the layer property.
    /// 
    /// \return
    /// Returns an association property definition
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition* get_LayerProperty();

    /// \brief
    /// Sets a reference to the layer property. The associated class of the layer
    /// association property must be an NetworkLayer.
    /// 
    /// \param value 
    /// Input an association property definition
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_LayerProperty(NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition* value);

    /// \brief
    /// Constructs a NetworkNodeFeatureClass object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	NetworkNodeFeatureClass(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_SCHEMA::NetworkFeatureClass(unmanaged, autoDelete)
	{

	}

public private:
	inline FdoNetworkNodeFeatureClass* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


