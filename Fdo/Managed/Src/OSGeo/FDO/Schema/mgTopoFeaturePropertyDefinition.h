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
/// FdoTopoFeaturePropertyDefinition derives from FdoPropertyDefinition and its
/// values can be used to relate topological primitives to one or more
/// application domain features. One can use the short form "TopoFeature" to
/// refer to these properties. The semantics of this property are similar to
/// Object properties. One uses FdoIFeatureReader::GetObject() to retrieve
/// values.
/// <p><b>Note:</b> <span class="red_text">This Help topic is provided for informational use only. There is
/// no interface or support provided. Autodesk reserves the right to change
/// the software related to the content herein.</span>
/// \remarks
/// FdoTopoFeaturePropertyDefinition and FdoTopoGeometryPropertyDefinition are
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
    /// Constructs an instance of an FdoTopoFeaturePropertyDefinition using the
    /// specified arguments.
    /// 
	TopoFeaturePropertyDefinition(System::String* name, System::String* description);

	TopoFeaturePropertyDefinition(System::String* name, System::String* description, System::Boolean system);

	TopoFeaturePropertyDefinition(System::IntPtr unmanaged, System::Boolean autoDelete) : NAMESPACE_OSGEO_FDO_SCHEMA::PropertyDefinition(unmanaged, autoDelete)
	{

	}

public private:
	inline FdoTopoFeaturePropertyDefinition* GetImpObj();
};

END_NAMESPACE_OSGEO_FDO_SCHEMA


