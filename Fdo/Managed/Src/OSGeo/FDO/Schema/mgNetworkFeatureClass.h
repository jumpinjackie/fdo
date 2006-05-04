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

#include "FDO\Schema\mgFeatureClass.h"

class FdoNetworkFeatureClass;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA
public __gc class DataPropertyDefinition;
public __gc class AssociationPropertyDefinition;

/// \brief
/// FdoNetworkFeature is an abstract class that derives from FdoFeatureClass and is a base 
/// for the FdoNetworkNodeFeature and FdoNetworkLinkFeature.
/// <p><b>Note:</b> <span class="red_text">This Help topic is provided for informational use only. There is
/// no interface or support provided. Autodesk reserves the right to change
/// the software related to the content herein.</span>
public __gc class NetworkFeatureClass : public NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass
{
public:
    /// \brief
    /// Gets a reference to the cost property.
    /// 
    /// \return
    /// Returns a property definition
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition* get_CostProperty();

    /// \brief
    /// Sets a reference to the cost property.
    /// 
    /// \param value 
    /// Input a property definition
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_CostProperty(NAMESPACE_OSGEO_FDO_SCHEMA::DataPropertyDefinition* value);

    /// \brief
    /// Gets a reference to the network property.
    /// 
    /// \return
    /// Returns an association property definition
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition* get_NetworkProperty();

    /// \brief
    /// Sets a reference to the network property.
    /// 
    /// \param value 
    /// Input an association property definition
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_NetworkProperty(NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition* value);

    /// \brief
    /// Gets a reference to the referenced feature property.
    /// 
    /// \return
    /// Returns an association property definition
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition* get_ReferencedFeatureProperty();

    /// \brief
    /// Sets a reference to the referenced feature property.
    /// 
    /// \param value 
    /// Input an association property definition
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_ReferencedFeatureProperty(NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition* value);

    /// \brief
    /// Gets a reference to the parent network feature property.
    /// 
    /// \return
    /// Returns an association property definition
    /// 
	__property NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition* get_ParentNetworkFeatureProperty();

    /// \brief
    /// Sets a reference to the parent network feature property.
    /// 
    /// \param value 
    /// Input an association property definition
    /// 
    /// \return
    /// Returns nothing
    /// 
	__property System::Void set_ParentNetworkFeatureProperty(NAMESPACE_OSGEO_FDO_SCHEMA::AssociationPropertyDefinition* value);

	NetworkFeatureClass(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_SCHEMA::FeatureClass(unmanaged, autoDelete)
	{

	}

public private:
	inline FdoNetworkFeatureClass* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


