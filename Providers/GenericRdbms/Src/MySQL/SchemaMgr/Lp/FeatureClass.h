#ifndef FDOSMLPMYSQLFEATURECLASS_H
#define FDOSMLPMYSQLFEATURECLASS_H        1
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

#include <Sm/Lp/FeatureClass.h>
#include "../../../SchemaMgr/Lp/ClassDefinition.h"
#include "ClassDefinition.h"

// MySql Provider's LogicalPhysical FeatureClass definition

class FdoSmLpMySqlFeatureClass : public FdoSmLpFeatureClass, public FdoSmLpMySqlClassDefinition
{
public:
    // Construct from a class reader
    FdoSmLpMySqlFeatureClass(FdoSmPhClassReaderP classReader, FdoSmLpSchemaElement* parent);

	// Construct from an FDO feature class
    FdoSmLpMySqlFeatureClass(
        FdoFeatureClass* pFdoClass, 
//        FdoOracleOvClassDefinition* pClassOverrides,        
        bool bIgnoreStates,
        FdoSmLpSchemaElement* parent
    );

	virtual void Update(
        FdoClassDefinition* pFdoClass,
        FdoSchemaElementState elementState,
        FdoPhysicalClassMapping* pClassOverrides,
        bool bIgnoreStates
    );

protected:
    virtual ~FdoSmLpMySqlFeatureClass();

    // Set the given class mappings with the physical mappings for this class.
    virtual bool SetSchemaMappings( FdoPhysicalClassMappingP classMapping, bool bIncludeDefaults ) const;
	
    void PostFinalize()
    {
        FdoSmLpFeatureClass::PostFinalize();
    }

    // Get writer for adding a MetaSchema entry for this feature class.
    virtual FdoSmPhClassWriterP GetPhysicalAddWriter();
};

#endif
