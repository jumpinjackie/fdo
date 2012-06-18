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

class FdoNetworkLayerClass;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA

/// \ingroup (OSGeoFDOSchema)
/// \brief
/// NetworkLayerClass derives from ClassDefinition and can be used to represent a network layer.
/// <p><b>Note:</b> <span class="red_text">This Help topic is provided for informational use only. There is
/// no interface or support provided. OSGeo reserves the right to change
/// the software related to the content herein.</span>
public ref class NetworkLayerClass : public NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition
{
public:
    /// \brief
    /// Constructs a default instance of an NetworkLayerClass.
    /// 
	NetworkLayerClass();

    /// \brief
    /// Constructs an instance of an NetworkLayerClass using the specified arguments.
    /// 
    /// \param name 
    /// Input name
    /// \param description 
    /// Input description
    /// 
	NetworkLayerClass(System::String^ name, System::String^ description);

    /// \brief
    /// Gets the concrete class type.
    /// 
    /// \return
    /// Returns class type
    /// 
    property NAMESPACE_OSGEO_FDO_SCHEMA::ClassType ClassType
    {
        NAMESPACE_OSGEO_FDO_SCHEMA::ClassType get();
    }

    /// \brief
    /// Constructs a NetworkLayerClass object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	NetworkLayerClass(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_SCHEMA::ClassDefinition(unmanaged, autoDelete)
	{

	}

internal:
	inline FdoNetworkLayerClass* GetImpObj();
public:
    virtual IntPtr GetDisposableObject() override;
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


