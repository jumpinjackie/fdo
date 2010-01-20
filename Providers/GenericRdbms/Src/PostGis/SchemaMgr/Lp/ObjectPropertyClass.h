//
// Copyright (C) 2006 Refractions Research, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
#ifndef FDORDBMSLPPOSTGISOBJECTPROPERTYCLASS_H
#define FDORDBMSLPPOSTGISOBJECTPROPERTYCLASS_H

#include "../../../SchemaMgr/Lp/ObjectPropertyClass.h"
#include "ClassDefinition.h"

/// The Object Property Class is a special wrapper class for
/// an object property.
/// It wraps around the table containing the object property values.
/// This class is used by PostGIS provider.
///
class FdoSmLpPostGisObjectPropertyClass :
    public FdoSmLpGrdObjectPropertyClass,
    public FdoSmLpPostGisClassDefinition
{
public:

	/// Create an Object Property Class.
    /// \todo To be documented.
	FdoSmLpPostGisObjectPropertyClass(FdoSmLpObjectPropertyDefinition* parent,
		FdoSmLpClassDefinition* parentType,
		FdoSmLpPropertyMappingDefinition* mapping,
		FdoSmLpPropertyMappingType mappingType,
        FdoPhysicalClassMapping* overrides);

	/// Create an Object Property Class from a base class.
    /// This is called when an object property is inherited.
    /// \todo To be documented.
	FdoSmLpPostGisObjectPropertyClass(
    	FdoSmLpObjectPropertyClassP pBase,
		FdoSmLpObjectPropertyDefinition* pParent, 
		FdoSmLpClassDefinition* pParentType,
		FdoSmLpPropertyMappingDefinition* pMapping,
		FdoSmLpPropertyMappingType mappingType, 
        FdoPhysicalClassMapping* pOverrides);

    /// Apply updates to this object property class.
    /// \todo To be documented.
	virtual void Update(FdoClassDefinition* fdoClass,
        FdoSchemaElementState state,
        FdoPhysicalSchemaMapping* overrides,
        bool ignoreStates);

protected:

    /// \todo To be documented.
	virtual ~FdoSmLpPostGisObjectPropertyClass();

    /// \todo To be documented.
	virtual void PostFinalize();

private:

};

#endif // FDORDBMSLPPOSTGISOBJECTPROPERTYCLASS_H