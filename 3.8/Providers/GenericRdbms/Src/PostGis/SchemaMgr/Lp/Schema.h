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
#ifndef FDOSMLPPOSTGISSCHEMA_H
#define FDOSMLPPOSTGISSCHEMA_H

#include <Sm/Lp/SchemaCollection.h>
#include "../../../SchemaMgr/Lp/Schema.h"


/// Defines LogicalPhysical schema collection for PostGIS provider.
///
class FdoSmLpPostGisSchema : public FdoSmLpGrdSchema
{
public:

    /// Creates a schema from a schema reader.
    ///
    /// \param
    /// reader [in] - get the schema attributes from this schema reader.
    /// \param
    /// mgr [in] - Physical Schema Manager.
    /// \param
    /// schemas [in] - containing schema collection for this schema.
    ///
    FdoSmLpPostGisSchema(FdoSmPhSchemaReaderP reader, FdoSmPhMgrP mgr,
        FdoSmLpSchemaCollection* schemas);

    /// Creates an instance of a LogicalPhysicalSchema from FDO Feature Schema.
	///
    /// \param
    /// featSchema [in] - the FDO feature schema.
    /// \param
    /// ignoreStates [in] - true if the element state on the FDO schema
    /// is to be ignored.
    /// \param
	/// mgr [in] - Physical Schema Manager. Used to retrieve schema classes.
    ///
    FdoSmLpPostGisSchema(FdoFeatureSchema* featSchema, bool ignoreStates,
		FdoSmPhMgrP mgr, 
		FdoSmLpSchemaCollection* schemas);
	
    /// Generates the physical mappings for this feature schema.
    /// \todo To be documented
    virtual FdoPhysicalSchemaMappingP GetSchemaMappings(
        bool bincludeDefaults) const;
	
    /// Copy updates from corresponding FDO feature schema.
	///
    /// \param
    /// featSchema [in] - the FDO feature schema.
    /// \param
    /// state [in] - modification state for this feature schema.
    /// \param
    /// overrides [in] - FDO to physical mapping overrides.
    /// \param
    /// ignoreStates [in] - true if the element state on the FDO feature schema
    /// is to be ignored.
    ///
	virtual void Update(FdoFeatureSchema* featSchema, FdoSchemaElementState state,
        FdoPhysicalSchemaMapping* overrides,
        bool ignoreStates);

    /// \todo To be documented
    virtual FdoSmLpPropertyP CreateDataProperty(
        FdoSmPhClassPropertyReaderP reader,
        FdoSmLpClassDefinition* parent);
    
    /// \todo To be documented
    virtual FdoSmLpPropertyP CreateGeometricProperty(
        FdoSmPhClassPropertyReaderP reader,
        FdoSmLpClassDefinition* parent);
    
    /// \todo To be documented
    virtual FdoSmLpPropertyP CreateObjectProperty(
        FdoSmPhClassPropertyReaderP reader,
        FdoSmLpClassDefinition* parent);
    
    /// \todo To be documented
    virtual FdoSmLpPropertyP CreateAssociationProperty(
        FdoSmPhClassPropertyReaderP reader,
        FdoSmLpClassDefinition* parent);

    /// \todo To be documented
    virtual FdoSmLpPropertyP CreateDataProperty(
        FdoDataPropertyDefinition* fdoProp,
        bool ignoreStates,
        FdoSmLpClassDefinition* parent);

    /// \todo To be documented
    virtual FdoSmLpPropertyP CreateGeometricProperty(
        FdoGeometricPropertyDefinition* fdoProp, 
        bool ignoreStates,
        FdoSmLpClassDefinition* parent
    );

    /// \todo To be documented
    virtual FdoSmLpPropertyP CreateGeometricProperty(
        FdoGeometricPropertyDefinition* fdoProp, 
        FdoString* columnNameX,
        FdoString* columnNameY,
        FdoString* columnNameZ,
        bool ignoreStates,
        FdoSmLpClassDefinition* parent);

    /// \todo To be documented
    virtual FdoSmLpPropertyP CreateObjectProperty(
        FdoObjectPropertyDefinition* fdoProp, 
        bool ignoreStates,
        FdoSmLpClassDefinition* parent);

    /// \todo To be documented
    virtual FdoSmLpPropertyP CreateAssociationProperty(
        FdoAssociationPropertyDefinition* fdoProp, 
        bool ignoreStates,
        FdoSmLpClassDefinition* parent);

protected:

    /// \todo To be documented
    virtual ~FdoSmLpPostGisSchema();

    /// Various class definition creator implementors.
    /// \todo To be documented
    virtual FdoSmLpClassDefinitionP CreateFeatureClass(FdoSmPhClassReaderP reader);

    /// \todo To be documented
    virtual FdoSmLpClassDefinitionP CreateClass(FdoSmPhClassReaderP reader);

    /// \todo To be documented
    virtual FdoSmLpClassDefinitionP CreateFeatureClass(
        FdoFeatureClass* fdoClass,
        bool ignoreStates);

    /// \todo To be documented
    virtual FdoSmLpClassDefinitionP CreateClass(FdoClass* fdoClass,
        bool ignoreStates);

    virtual void CreatePhysicalSchema(
        FdoSmPhOwnerP owner
    );

    virtual void DeletePhysicalSchema(
        FdoSmPhOwnerP owner
    );

private:
};

/// \brief
/// Declaration of smart-pointer type for FdoSmLpPostGisSchema.
/// Provided for convenience.
///
typedef FdoPtr<FdoSmLpPostGisSchema> FdoSmLpPostGisSchemaP;

#endif // FDOSMLPPOSTGISSCHEMA_H
