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
#ifndef FDOSMLPPOSTGISCLASSDEFINITION_H
#define FDOSMLPPOSTGISCLASSDEFINITION_H

#include "../../../SchemaMgr/Lp/ClassDefinition.h"
#include <Rdbms/Override/PostGis/PostGisOvPhysicalSchemaMapping.h>
#include <Fdo/Commands/Schema/PhysicalClassMapping.h>
#include "../Ph/Table.h"

/// Defines type of non-feature class definition.
/// This class is used by PostGIS provider.
///
class FdoSmLpPostGisClassDefinition : public FdoSmLpGrdClassDefinition
{
public:

    /// Destructor.
    virtual ~FdoSmLpPostGisClassDefinition();

	/// Update from class overrides
    ///
    /// \param
    /// fdoClass [in] - instance of the FDO class.
    ///
    /// \param
    /// state [in] - modification state for this class definition.
    ///
    /// \param
    /// classOverrides [in] - FDO to physical mapping overrides.
    ///
    /// \param
    /// ignoreStates [in] - true if the element state on the FDO class
    /// is to be ignored.
    ///
    virtual void Update(FdoClassDefinition* fdoClass,
        FdoSchemaElementState state,
        FdoPhysicalClassMapping* classOverrides,
        bool ignoreStates);

    virtual FdoStringP DefaultDbObjectName();

    // The following return versions of the db object or root object names
    // that are good for building other names such as for object property
    // tables and columns. 
    // The db object or root name minus the user name part is returned.
    virtual FdoStringP  GetSubstDbObjectName( FdoStringP dbObjectName = L"" ) const;
    virtual FdoStringP  GetSubstRootDbObjectName() const;

protected:

    /// Constructs instance of a ClassDefinition from a class reader.
    ///
    /// \todo To be documented
    ///
    FdoSmLpPostGisClassDefinition(FdoSmPhClassReaderP classReader,
        FdoSmLpSchemaElement* parent);

    /// Constructs an instance of a ClassDefinition from an FDO element
    /// and Fdo to physical mapping overrides.
    ///
    /// Parameters:
	///		pFdoClass: the FDO class.
    ///      bIgnoreStates: true if the element state on the FDO class is 
    ///          to be ignored.
	///		parent: the parent element ( usually an FdoSmLpSchema ).
    ///
    /// \todo To be documented
    ///
    FdoSmLpPostGisClassDefinition(FdoClassDefinition* fdoClass, 
        bool ignoreStates, FdoSmLpSchemaElement* parent);

	/// Constructs an instance of a ClassDefinition from the given attributes.
	///
	/// Parameters:
	///		name: class name
	///		description: class description
	///		parent: class objects parent ( usually an FdoSmLpSchema object )
	///    pSrcClass: When this is an object property class, pSrcClass represents the 
	///			object property class for the associated object property's base property.
	///		tableMapping: property to table mapping type
	///		tableName: class table name
	///		table: class table. Null if class has no table or table
	///			does not exist.
    ///
    /// \todo To be documented
    ///
	FdoSmLpPostGisClassDefinition(FdoString* name, FdoString* description,
        FdoSmLpSchemaElement* parent,
        FdoPtr<FdoSmLpClassDefinition> srcClass,
        FdoSmOvTableMappingType tableMapping,
        FdoString* tableName,
        FdoSmPhDbObjectP table,
        FdoSchemaElementState state);

    /// Add the physical mappings for this class to the given schema override set
    ///
    /// \todo To be documented
    ///
    virtual bool AddSchemaMappings(FdoPhysicalSchemaMappingP schemaMapping,
        bool bIncludeDefaults) const;

    /// Generate the physical mappings for this class
    ///
    /// \todo To be documented
    ///
    virtual FdoPhysicalClassMappingP GetSchemaMappings(
        bool includeDefaults) const;

    /// Sets the PostGIS-specific physical mappings.
    ///
    /// \todo To be documented
    ///
    virtual bool SetSchemaMappings(FdoPhysicalClassMappingP classMapping,
        bool bIncludeDefaults) const;

    /// Get a writer for adding this class to the MetaSchema
    ///
    /// \todo To be documented
    ///
    virtual void SetPhysicalAddWriter(FdoSmPhClassWriterP writer);

    /// \todo To be documented
    virtual FdoSmPhDbObjectP NewTable(FdoString* tableName, FdoString* keyName);

private:

}; // clas FdoSmLpPostGisClassDefinition


/// \brief
/// Declaration of smart-pointer type for FdoSmLpPostGisClassDefinition.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmLpPostGisClassDefinition> FdoSmLpPostGisClassDefinitionP;

#endif // FDOSMLPPOSTGISCLASSDEFINITION_H
