#ifndef FDO_XML_SCHEMAMANAGER_H_
#define FDO_XML_SCHEMAMANAGER_H_
//

//
// Copyright (C) 2004-2006  Autodesk, Inc.
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

#ifdef _WIN32
#pragma once
#endif

#include <Std.h>

#include "LpSchemaElement.h"


class FdoXmlLpSchema;
class FdoXmlLpSchemaCollection;
class FdoFeatureSchemaCollection;
class FdoXmlFlags;

// This Schema Manager handles the Logical to Physical schema correspondences
// for GML Schemas. In this case:
//
//      Logical Schema: refers to the FDO Feature Schemas deserialized 
//      from a set of GML Schemas.
//
//      Physical Schema: refers to the GML Schemas. This class does not
//      handle the GML Schemas directly, but does handle the GML Schema Mappings
//      which contain relevant GML Schema information that gets lost when they
//      are deserialized to FDO Feature Schemas.

class FdoXmlSchemaManager: public FdoXmlLpSchemaElement {
    friend class FdoXmlLpSchema;
public:
    // Create a Schema Manager for a corresponding set of Feature Schemas and GML Schema Mappings.
    // The Schema Mappings are passed in via the FdoXmlFlags.
	static FdoXmlSchemaManager* Create( 
        FdoFeatureSchemaCollection* schemas, //can be NULL
        FdoXmlFlags* flags // can be NULL
    );

    // Given a GML URI name, return the LogicalPhysical schema object, which contains the
    // Feature Schema and corresponding Schema Mapping set.
    // Returns NULL if the schema is not present.
    FdoXmlLpSchema* UriToSchema( FdoString* schemaUri );

    // Given an FDO Feature Schema name, return the LogicalPhysical schema object
    // Returns NULL if the schema is not present.
    FdoXmlLpSchema* NameToSchema( FdoString* schemaName );

    virtual FdoString* GetName() { return L""; }

protected:
	FdoXmlSchemaManager();
	FdoXmlSchemaManager( FdoFeatureSchemaCollection* schemas, FdoXmlFlags* flags);
	virtual ~FdoXmlSchemaManager();

	virtual void Dispose ();

private:
    FdoPtr<FdoFeatureSchemaCollection> m_fdoSchemas;
    FdoPtr<FdoXmlFlags> m_flags;

    FdoPtr<FdoXmlLpSchemaCollection> m_lpSchemas;

// helper
private:
    FdoXmlLpSchemaCollection* _schemas();

};

typedef FdoPtr<FdoXmlSchemaManager> FdoXmlSchemaManagerP;

#endif


