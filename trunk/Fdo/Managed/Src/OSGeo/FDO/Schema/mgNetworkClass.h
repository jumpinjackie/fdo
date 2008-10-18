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

#include "FDO\Schema\mgClassDefinition.h"
#include "FDO\Schema\mgClassType.h"

class FdoNetworkClass;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __gc class NetworkLayerClass;

/// \ingroup (OSGeoFDOSchema)
/// \brief
/// The NetworkClass derives from ClassDefinition and can be used to represent a 
/// linear network. The NetworkClass has 2 pre-defined base properties. The first is a Boolean 
/// type called "Directed". And the second is a ObjectPropertyDefinition of an ordered collection called "Layers".
/// The "Directed" property is used to indicate if the network is directed or undirected. 
/// The network is directed if the Directed property is set to true. Otherwise, it is undirected. The Layers property
/// is used to contain the ordered list of layers. The class of the layers object property must be of type NetworkLayer.
/// <p><b>Note:</b> <span class="red_text">This Help topic is provided for informational use only. There is
/// no interface or support provided. OSGeo reserves the right to change
/// the software related to the content herein.</span>
public __gc class NetworkClass : public NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition
{
public:
    /// \brief
    /// Constructs a default instance of an NetworkClass.
    /// 
	NetworkClass();

    /// \brief
    /// Constructs an instance of an NetworkClass using the specified arguments.
    /// 
    /// \param name 
    /// Input name
    /// \param description 
    /// Input description
    /// 
	NetworkClass(System::String* name, System::String* description);

    /// \brief
    /// Gets the concrete class type.
    /// 
    /// \return
    /// Returns class type
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::ClassType get_ClassType();

    /// \brief
    /// Gets a reference to the layer class.
    /// 
    /// \return
    /// Returns a network layer class definition
    /// 
	__property  NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLayerClass* get_LayerClass();

    /// \brief
    /// Sets a reference to the layer class. The layer class is used to define the system
    /// layers ordered collection object  property.
    /// 
    /// \param value 
    /// Input an network layer class
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property  System::Void set_LayerClass(NAMESPACE_OSGEO_FDO_SCHEMA::NetworkLayerClass* value);

    /// \brief
    /// Constructs a NetworkClass object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	NetworkClass(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition(unmanaged, autoDelete)
	{

	}

public private:
	inline FdoNetworkClass* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


