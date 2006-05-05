#ifndef FDOSMLPGRDSCHEMA_H
#define FDOSMLPGRDSCHEMA_H      1
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


#ifdef _WIN32
#pragma once
#endif

#include <Sm/Lp/SchemaCollection.h>

// Oracle Provider's LogicalPhysical schema definition
class FdoSmLpGrdSchema : public FdoSmLpSchema
{
public:
    // Update from generic schema override set
    //
    // Parameters
    //      pFeatSchema: the FDO feature schema.
    //      elementState: the modification state for this feature schema
    //      pOverrides: schema override set.
    //      bIgnoreStates: true if the element state on the FDO feature schema is 
    //          to be ignored.
	virtual void Update(
        FdoFeatureSchema* pFeatSchema, 
        FdoSchemaElementState elementState,
        FdoPhysicalSchemaMapping* pOverrides,
        bool bIgnoreStates
    ); 

protected:
    // unused constructor needed only to build Linux
    FdoSmLpGrdSchema() {}

    // Creates a schema from a schema reader.
    //
    // Parameters:
    //      rdr: get the schema attributes from this schema reader.
    //      mgr: Physical Schema Manager
    //      schemas: containing schema collection for this schema.
    FdoSmLpGrdSchema(
        FdoSmPhSchemaReaderP rdr,
        FdoSmPhMgrP physicalSchema,
        FdoSmLpSchemaCollection* schemas
    );

    // Creates an instance of a LogicalPhysicalSchema from an FDO Feature Schema.
	//
	// Parameters:
	//		FeatSchema: the FDO schema.
    //      pOverrides: Fdo to physical mapping overrides.
    //      bIgnoreStates: true if the element state on the FDO schema is 
    //          to be ignored.
	//		physicalSchema: Physical Schema Manager. Used to retrieve the schema's
    //          classes.
    FdoSmLpGrdSchema(
		FdoFeatureSchema* pFeatSchema,
        bool bIgnoreStates,
		FdoSmPhMgrP physicalSchema, 
		FdoSmLpSchemaCollection* schemas
	);
	
    virtual ~FdoSmLpGrdSchema(void);

    // Get writer for adding this feature schema to the MetaSchema
    virtual FdoSmPhSchemaWriterP GetPhysicalAddWriter();

    // Put the generic physical mappings for this feature class on the
    // the given schema override set.
    bool SetSchemaMappings( FdoPhysicalSchemaMappingP schemaMapping, bool bIncludeDefaults ) const;
};

typedef FdoPtr<FdoSmLpGrdSchema> FdoSmLpGrdSchemaP;

#endif

