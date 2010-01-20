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
#ifndef FDOSMLPPOSTGISFEATURECLASS_H
#define FDOSMLPPOSTGISFEATURECLASS_H

#include <Sm/Lp/FeatureClass.h>
#include "../../../SchemaMgr/Lp/ClassDefinition.h"
#include "ClassDefinition.h"

/// Defines spatial feature class.
/// This class is used by PostGIS provider.
///
class FdoSmLpPostGisFeatureClass : 
    public FdoSmLpFeatureClass,
    public FdoSmLpPostGisClassDefinition
{
public:
    
    /// Construct from a class reader.
    /// \todo To be documented.
    FdoSmLpPostGisFeatureClass(FdoSmPhClassReaderP classReader,
        FdoSmLpSchemaElement* parent);

	/// Construct from an FDO feature class
    /// \todo To be documented.
    FdoSmLpPostGisFeatureClass(FdoFeatureClass* fdoClass, bool ignoreStates,
        FdoSmLpSchemaElement* parent);

    /// \todo To be documented.
	virtual void Update(FdoClassDefinition* fdoClass,
        FdoSchemaElementState state,
        FdoPhysicalClassMapping* classOverrides,
        bool ignoreStates);

protected:

    /// \todo To be documented.
    virtual ~FdoSmLpPostGisFeatureClass();

    /// Set given class mappings with the physical mappings for this class.
    /// \todo To be documented.
    virtual bool SetSchemaMappings(FdoPhysicalClassMappingP classMapping,
        bool includeDefaults ) const;
	
    /// \todo To be documented.
    void PostFinalize();

    /// Get writer for adding a MetaSchema entry for this feature class.
    /// \todo To be documented.
    virtual FdoSmPhClassWriterP GetPhysicalAddWriter();

};

//
// Implementation of inline class member functions
//

inline void FdoSmLpPostGisFeatureClass::PostFinalize()
{
    FdoSmLpFeatureClass::PostFinalize();
}

#endif // FDOSMLPPOSTGISFEATURECLASS_H