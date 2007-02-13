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

#include "FDO\Schema\mgPropertyDefinition.h"

class FdoTopoFeaturePropertyDefinition;

BEGIN_NAMESPACE_OSGEO_FDO_SCHEMA

/// \brief
/// TopoFeaturePropertyDefinition derives PropertyDefinition and its
/// values can be used to relate topological primitives to one or more
/// application domain features. One can use the short form "TopoFeature" to
/// refer to these properties. The semantics of this property are similar to
/// Object properties. One uses IFeatureReader::GetObject() to retrieve
/// values.
/// <p><b>Note:</b> <span class="red_text">This Help topic is provided for informational use only. There is
/// no interface or support provided. OSGeo reserves the right to change
/// the software related to the content herein.</span>
/// \remarks
/// TopoFeaturePropertyDefinition and TopoGeometryPropertyDefinition are
/// the matching roles of many:many relationships between topological primitives
/// and application domain features. FDO Association properties are not used for
/// these roles because topological relationships are not between fixed classes.
/// The relationships are populated only from the TopoGeometry role, so
/// TopoFeature properties need never be subject to an insertion or update
/// operation. 
public __gc class TopoFeaturePropertyDefinition : public NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition
{
public:
    /// \brief
    /// Constructs an instance of an TopoFeaturePropertyDefinition using the
    /// specified arguments.
    /// 
	TopoFeaturePropertyDefinition(System::String* name, System::String* description);
	TopoFeaturePropertyDefinition(System::String* name, System::String* description, System::Boolean system);

    /// \brief
    /// Constructs a TopoFeaturePropertyDefinition object based on an unmanaged instance of the object
    /// 
    /// \param unmanaged 
    /// Input A Pointer to the unmanaged object.
    /// 
    /// \param autoDelete 
    /// Input Indicates if the constructed object should be automatically deleted 
    /// once it no longer referenced.
    /// 
	TopoFeaturePropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(unmanaged, autoDelete)
	{

	}

public private:
	inline FdoTopoFeaturePropertyDefinition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


