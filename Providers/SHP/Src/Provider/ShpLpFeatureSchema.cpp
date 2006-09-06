/*
 * 
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

#include "stdafx.h"
#include <ShpLpFeatureSchema.h>
#include <ShpLpClassDefinition.h>

ShpLpFeatureSchema::ShpLpFeatureSchema(void)
{
    // 0-argument constructor to please FdoPtr::operator-> 
    throw FdoException::Create(NlsMsgGet(SHP_UNEXPECTED_ERROR, "Unexpected error '%1$ls'.", L"zero arg ShpLpFeatureSchema::ShpLpFeatureSchema"));
}

ShpLpFeatureSchema::ShpLpFeatureSchema(
    ShpLpFeatureSchemaCollection * parentLpSchemas,
    ShpConnection * connection,
    ShpPhysicalSchema* physicalSchema,           // physical layer
	FdoFeatureSchema * configLogicalSchema,            // logical layer
	FdoShpOvPhysicalSchemaMapping * configSchemaMapping, // override layer
    bool ignore_states)
:
    m_parentLpSchemas(parentLpSchemas),  // weak reference
    m_physicalSchema(physicalSchema)
{
    VALIDATE_POINTER(connection);

    FDO_SAFE_ADDREF(m_physicalSchema.p);

    // Create the Lp Classes collection:
    m_lpClasses = new ShpLpClassDefinitionCollection();

    // Convert logical to physical (or vice-versa):
    if (m_physicalSchema == NULL)
        ConvertLogicalToPhysical(connection, configLogicalSchema, configSchemaMapping, ignore_states);
    else
        ConvertPhysicalToLogical(connection, configLogicalSchema, configSchemaMapping);

    // if the schema already exists, just add the new classes to the existing one
    FdoPtr<ShpLpFeatureSchema> schema = m_parentLpSchemas->FindItem (GetName ());
    if (schema != NULL)
    {
        FdoPtr<ShpLpClassDefinitionCollection> new_classes = GetLpClasses ();
        FdoPtr<ShpLpClassDefinitionCollection> existing_classes = schema->GetLpClasses ();
        FdoPtr<FdoFeatureSchema> existing_logical_schema = schema->GetLogicalSchema ();
        FdoPtr<FdoClassCollection> existing_logical_classes = existing_logical_schema->GetClasses ();
        FdoPtr<FdoFeatureSchema> new_logical_schema = GetLogicalSchema ();
        FdoPtr<FdoClassCollection> new_logical_classes = new_logical_schema->GetClasses ();
        for (int i = 0; i < new_classes->GetCount (); i++)
        {
            FdoPtr <ShpLpClassDefinition> definition = new_classes->GetItem (i);
            existing_classes->Add (definition);
            definition->SetParent (schema);
            FdoPtr<FdoClassDefinition> cls = new_logical_classes->GetItem (definition->GetName ());
            new_logical_classes->Remove (cls);
            existing_logical_classes->Add (cls);
        }
    }
    else
        // otherwise add this new LpSchema to its parent collection:
        m_parentLpSchemas->Add(this);
}

ShpLpFeatureSchema::~ShpLpFeatureSchema(void)
{
}

FdoString * ShpLpFeatureSchema::GetName() const
{
    return m_logicalSchema->GetName();
}

bool ShpLpFeatureSchema::CanSetName() const
{
    return false;
}

FdoFeatureSchema * ShpLpFeatureSchema::GetLogicalSchema() const
{
    FDO_SAFE_ADDREF(m_logicalSchema.p);
    return m_logicalSchema;
}

ShpLpClassDefinitionCollection* ShpLpFeatureSchema::GetLpClasses()
{
    FDO_SAFE_ADDREF(m_lpClasses.p);
    return m_lpClasses;
}

ShpPhysicalSchema * ShpLpFeatureSchema::GetPhysicalSchema() const
{
    FDO_SAFE_ADDREF(m_physicalSchema.p);
    return m_physicalSchema;
}

// Use the Physical schema to populate the Logical/Physical and Logical classes.
void ShpLpFeatureSchema::ConvertPhysicalToLogical(
	ShpConnection * connection,
	FdoFeatureSchema * configLogicalSchema,             // logical layer
	FdoShpOvPhysicalSchemaMapping * configSchemaMapping)  // overrides layer
{
    if (m_physicalSchema == NULL)
        m_physicalSchema = connection->GetPhysicalSchema ();

    // Create the logical schema:
    //////////////////////////////////////////////////////////////////////

    FdoStringP pDefaultSchName = NlsMsgGet(SHP_DEFAULT_SCHEMA_NAME, "Default");
    if(((FdoString*)pDefaultSchName)[pDefaultSchName.GetLength()-1] == L' ')
        pDefaultSchName = pDefaultSchName.Mid(0, pDefaultSchName.GetLength()-1);

    FdoString * logicalSchemaName = (NULL != configSchemaMapping) ? 
        configSchemaMapping->GetName() : 
        (NULL != configLogicalSchema) ?
            configLogicalSchema->GetName () :
                (FdoString*)pDefaultSchName;

    FdoStringP pDefaultSchDesc = NlsMsgGet(SHP_DEFAULT_SCHEMA_DESCRIPTION, "Default schema.");
    if(((FdoString*)pDefaultSchDesc)[pDefaultSchDesc.GetLength()-1] == L' ')
        pDefaultSchDesc = pDefaultSchDesc.Mid(0, pDefaultSchDesc.GetLength()-1);

    FdoString * logicalSchemaDescription =
        (NULL != configLogicalSchema) ?
            configLogicalSchema->GetDescription () :
                (FdoString*)pDefaultSchDesc;

    m_logicalSchema = FdoFeatureSchema::Create(logicalSchemaName, logicalSchemaDescription);

    if (NULL != configLogicalSchema)
    {
        // base the list of logical-physical classes on the configuration file
        FdoPtr<FdoClassCollection> classes = configLogicalSchema->GetClasses ();
        FdoInt32 class_count = classes->GetCount ();
        for (FdoInt32 i = 0; i < class_count; i++)
        {
            FdoPtr<FdoClassDefinition> cls = classes->GetItem (i);
            FdoStringP name = cls->GetName ();
            ShpFileSet* set = NULL;
		    FdoPtr<FdoShpOvClassDefinition> configClassMapping;
            if (NULL != configSchemaMapping)
            {
                configClassMapping = configSchemaMapping->FindByClassName (name);
                if (configClassMapping != NULL)
                {
                    FdoString* shp = configClassMapping->GetShapeFile ();
                    if ((NULL == shp) || (0 == wcslen (shp)))
                        set = m_physicalSchema->GetFileSet (name);
                    else
                    {
                        FdoInt32 set_count = m_physicalSchema->GetFileSetCount ();
                        for (FdoInt32 i = 0; i < set_count; i++)
                        {
                            ShpFileSet* _set = m_physicalSchema->GetFileSet (i);
                            if (0 == wcscmp (_set->GetShapeFile ()->FileName (), shp))
                            {
                                set = _set;
                                break;
                            }
                        }
                    }
                    if (NULL == set)
                        throw FdoException::Create (NlsMsgGet(SHP_NOT_FOUND, "Shp file '%1$ls' was not found.", shp));

                }
                else
                {
                    set = m_physicalSchema->GetFileSet (name);
                    if (NULL == set)
                    {
                        wchar_t* shp = (wchar_t*)alloca (sizeof (wchar_t) * (wcslen (name) + wcslen (SHP_EXTENSION) + 1));
                        wcscpy (shp, name);
                        wcscat (shp, SHP_EXTENSION);
                        throw FdoException::Create (NlsMsgGet(SHP_NOT_FOUND, "Shp file '%1$ls' was not found.", shp));
                    }
                }
            }
            else
            {
                set = m_physicalSchema->GetFileSet (name);
                if (NULL == set)
                {
                    wchar_t* shp = (wchar_t*)alloca (sizeof (wchar_t) * (wcslen (name) + wcslen (SHP_EXTENSION) + 1));
                    wcscpy (shp, name);
                    wcscat (shp, SHP_EXTENSION);
                    throw FdoException::Create (NlsMsgGet(SHP_NOT_FOUND, "Shp file '%1$ls' was not found.", shp));
                }
            }
			FdoPtr<ShpLpClassDefinition> pLpClassDefinition =
				new ShpLpClassDefinition (this, connection, set, cls, configClassMapping);
        }
    }
    else
    {
        // base the list of logical-physical classes on the filesets
        for (FdoInt32 i=0; i < m_physicalSchema->GetFileSetCount(); i++) 
        {
            ShpFileSet* pPhysicalFileSet = m_physicalSchema->GetFileSet(i);
            VALIDATE_POINTER(pPhysicalFileSet);

		    FdoPtr<FdoShpOvClassDefinition> configClassMapping;
            FdoClassDefinitionP logicalClassDef;

		    // If there is a configuration to use
		    if (NULL != configSchemaMapping)
		    {
			    // Look for a mapping that points to this shapefile:
                ShapeFile* shpFile = pPhysicalFileSet->GetShapeFile();
                VALIDATE_POINTER(shpFile);  //TODO: what if there is no SHP file and only a DBF file?
                configClassMapping = configSchemaMapping->FindByShapefile(shpFile->FileName());
                if (configClassMapping != NULL)
                {
                    // Look for class mapped to this table.
                    FdoClassesP pLogicalClassDefs = configLogicalSchema->GetClasses();
                    VALIDATE_POINTER(pLogicalClassDefs);
                    logicalClassDef = pLogicalClassDefs->FindItem(configClassMapping->GetName());
                }
		    }

		    // If there is no configuration, or a configuration with mapping
            // TODO: this will quietly throw away classes that don't have a mapping, this may be wrong w.r.t other providers).
		    if ( (NULL == configLogicalSchema) ||
			    ((NULL != configLogicalSchema) && (configClassMapping != NULL) && (logicalClassDef != NULL)) )
		    {
			    // Use this FileSet to add a class definition.
                // (adds itself automatically to its parent collection):
			    FdoPtr<ShpLpClassDefinition> pLpClassDefinition =
				    new ShpLpClassDefinition(this, connection, pPhysicalFileSet, logicalClassDef, configClassMapping);
		    }
        }
    }

    // Add the new logical schema to its parent collection:
    FdoPtr<FdoFeatureSchemaCollection> logicalSchemas = m_parentLpSchemas->GetLogicalSchemas();
    logicalSchemas->Add(m_logicalSchema);
}


// Convert the Logical schema to a Physical schema:
void ShpLpFeatureSchema::ConvertLogicalToPhysical(
	ShpConnection * connection,
	FdoFeatureSchema * configLogicalSchema,             // logical layer
	FdoShpOvPhysicalSchemaMapping * configSchemaMapping,  // overrides layer
    bool ignore_states)
{
    // 'Create' the physical schema (really just get the singleton physical schema):
    m_physicalSchema = connection->GetPhysicalSchema();

    // create a logical schema (and add it to its parent collection, if not already there):
    VALIDATE_POINTER(configLogicalSchema);
    m_logicalSchema = FdoFeatureSchema::Create (configLogicalSchema->GetName (), configLogicalSchema->GetDescription ());
    FdoPtr<FdoFeatureSchemaCollection> logicalSchemas = m_parentLpSchemas->GetLogicalSchemas();
    bool bAlreadyInCollection = false;
    for (FdoInt32 i=0; i<logicalSchemas->GetCount() && !bAlreadyInCollection; i++)
    {
        FdoPtr<FdoFeatureSchema> logicalSchema = logicalSchemas->GetItem(i);
        if (0 == wcscmp (logicalSchema->GetName (), m_logicalSchema->GetName ()))
            bAlreadyInCollection = true;
    }
    if (!bAlreadyInCollection)
        logicalSchemas->Add(m_logicalSchema);

    // Get logical classes and mappings:
    FdoPtr<FdoClassCollection> logicalClasses = configLogicalSchema->GetClasses ();
    FdoPtr<FdoShpOvClassCollection> classesMappings;
    if (configSchemaMapping != NULL)
        classesMappings = configSchemaMapping->GetClasses();

    // Create new Lp Classes (and thereby create new physical shapefiles):
    for (int i = 0, size = logicalClasses->GetCount (); i < size; i++)
    {
        FdoPtr<FdoClassDefinition> logicalClass = logicalClasses->GetItem (i);
        FdoPtr<FdoShpOvClassDefinition> configClassMapping;
        if (classesMappings != NULL)
            configClassMapping = classesMappings->GetItem(logicalClass->GetName());

        // Handle ignore_states flag:
        FdoSchemaElementState state = logicalClass->GetElementState ();
        if (ignore_states)
        {
           FdoPtr<ShpLpClassDefinition> lpExistingClass = m_lpClasses->FindItem(logicalClass->GetQualifiedName());
           if (lpExistingClass == NULL)
               state = FdoSchemaElementState_Added;
           else
               state = FdoSchemaElementState_Modified;
        }

        switch (state)
        {
            // Indicates the schema element has been added.
            case FdoSchemaElementState_Added:
                add_class (connection, logicalClass, configClassMapping);
                break;

            // Indicates the schema element is marked for deletion.
            case FdoSchemaElementState_Deleted:
                // if we're adding a schema, complain about deleted classes
                throw FdoException::Create (NlsMsgGet(SHP_SCHEMA_DELETED_CLASS_IN_ADDED_SCHEMA, "Conflicting schema element state: a class marked for deletion was found in a schema to be added."));
                break;

            // Indicates the schema element is not associated with an FdoFeatureSchema.
            case FdoSchemaElementState_Detached:
                // duh, waddo I do
                break;

            // Indicates the schema element has been modified in some way.
            case FdoSchemaElementState_Modified:
                // if we're adding a schema, complain about modified classes
                throw FdoException::Create (NlsMsgGet(SHP_SCHEMA_MODIFIED_CLASS_IN_ADDED_SCHEMA, "Conflicting schema element state: a class marked for modification was found in a schema to be added."));
                break;

            // Indicates the schema element has not been changed.
            case FdoSchemaElementState_Unchanged:
                // if we're adding a schema, complain about unchcanged classes
                throw FdoException::Create (NlsMsgGet(SHP_SCHEMA_UNCHANGED_CLASS_IN_ADDED_SCHEMA, "Conflicting schema element state: a class marked unchanged was found in a schema to be added."));
                break;

            default:
                throw FdoException::Create (NlsMsgGet(SHP_SCHEMA_UNSUPPORTED_ELEMENT_STATE, "Schema element state '%1$d' is not supported.", (int)logicalClass->GetElementState()));
        }
    }

    // Add physical schema to its parent collection:
    // NOTE: nothing to do since physical schemas dont exist
}




//  Modify the schema according to given information:
void ShpLpFeatureSchema::Modify(ShpConnection * connection, FdoFeatureSchema* configLogicalSchema, FdoShpOvPhysicalSchemaMapping* configSchemaMapping, bool ignore_states)
{
    FdoString* name;
    FdoSchemaElementState state;

    // Get given logical class collection and its mappings:
    FdoPtr<FdoClassCollection> logicalClasses = configLogicalSchema->GetClasses ();
    FdoPtr<FdoShpOvClassCollection> classesMappings;
    if (configSchemaMapping != NULL)
        classesMappings = configSchemaMapping->GetClasses();

    /*todo: check for modified schema description or attributes and update all tables.*/
    /*todo: how to determine what's changed, i.e. this is protected: schema->m_descriptionCHANGED*/

    // Process the class:
    for (int i = 0, size = logicalClasses->GetCount (); i < size; i++)
    {
        FdoPtr<FdoClassDefinition> logicalClass = logicalClasses->GetItem (i);
        name = logicalClass->GetName();
        FdoPtr<FdoShpOvClassDefinition> configClassMapping;
        if (classesMappings != NULL)
            configClassMapping = classesMappings->GetItem(name);

        // Handle IgnoreStates flag:
        state = logicalClass->GetElementState ();
        if (ignore_states)
        {
            FdoPtr<ShpLpClassDefinitionCollection> existing_classes = GetLpClasses ();
            FdoPtr <ShpLpClassDefinition> definition = existing_classes->FindItem (name);
            if (definition == NULL)
                state = FdoSchemaElementState_Added;
            else
                state = FdoSchemaElementState_Modified;
        }

        switch (state)
        {
            // Indicates the schema element has been added.
            case FdoSchemaElementState_Added:
                add_class (connection, logicalClass, configClassMapping);
                break;

            // Indicates the schema element is marked for deletion.
            case FdoSchemaElementState_Deleted:
                delete_class (connection, logicalClass);
                break;

            // Indicates the schema element is not associated with an FdoFeatureSchema.
            case FdoSchemaElementState_Detached:
                // duh, waddo I do
                break;

            // Indicates the schema element has been modified in some way.
            case FdoSchemaElementState_Modified:
                modify_class (connection, logicalClass, configClassMapping);
                break;

            // Indicates the schema element has not been changed.
            case FdoSchemaElementState_Unchanged:
                break;

            default:
//                throw FdoException::Create (NlsMsgGet(SHP_SCHEMA_UNSUPPORTED_ELEMENT_STATE, "Schema element state '%1$d' is not supported.", (int)logicalClass->GetElementState()));
                ;
        }
    }
}


// create the shp/dbf/shx file
void ShpLpFeatureSchema::add_class (ShpConnection * connection, FdoClassDefinition* logicalClass, FdoShpOvClassDefinition* configClassMapping)
{
    // Verify the class doesn't already exist:
    FdoPtr<ShpLpClassDefinition> oldLpClass = m_lpClasses->FindItem(logicalClass->GetName());
    if (oldLpClass != NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_126_CLASS_ALREADY_EXISTS), logicalClass->GetName()));

    // Create a new LpClass from the given logical class & mappings (and by doing so, create the new SHP/DBF/SHX files)
    // (adds itself automatically to its parent collection):
    FdoPtr<ShpLpClassDefinition> newLpClass = new ShpLpClassDefinition(this, connection, NULL, logicalClass, configClassMapping);
}

void ShpLpFeatureSchema::delete_class (ShpConnection * connection, FdoClassDefinition* logicalClass)
{
    // Determine if the class contains any data:
    FdoPtr<FdoISelect> select = (FdoISelect*)connection->CreateCommand(FdoCommandType_Select);
    select->SetFeatureClassName(logicalClass->GetQualifiedName());
    FdoPtr<FdoIFeatureReader> reader = select->Execute();
    bool bClassHasData = reader->ReadNext();
    reader->Close();

    if (bClassHasData)
        throw FdoException::Create(NlsMsgGet(SHP_CANT_DELETE_NON_EMPTY_CLASS, "Can't delete class '%1$ls' because it contains data.", (FdoString *) logicalClass->GetQualifiedName() ));

    FdoPtr<ShpLpClassDefinition> lpClass = ShpSchemaUtilities::GetLpClassDefinition(connection, logicalClass->GetQualifiedName());
    lpClass->Delete(connection);
}

void ShpLpFeatureSchema::modify_class (ShpConnection * connection, FdoClassDefinition* logicalClass, FdoShpOvClassDefinition* configClassMapping)
{
    // Determine if the class contains any data:
    FdoPtr<FdoISelect> select = (FdoISelect*)connection->CreateCommand(FdoCommandType_Select);
    select->SetFeatureClassName(logicalClass->GetQualifiedName());
    FdoPtr<FdoIFeatureReader> reader = select->Execute();
    bool bClassHasData = reader->ReadNext();
    reader->Close();

    // If the class contains data, throw an exception; otherwise, delete/recreate the class:
    if (bClassHasData)
        throw FdoException::Create(NlsMsgGet(SHP_CANT_MODIFY_NON_EMPTY_CLASS, "Can't modify class '%1$ls' because it contains data.", (FdoString *) logicalClass->GetQualifiedName() ));
 
	// Delete the original class:
    delete_class(connection, logicalClass);

    // Create the new class in its place:
    add_class(connection, logicalClass, configClassMapping);
}


// Permanently delete the Logical/LogicalPhysical/Physical counterparts to this schema, and all the classes it contains:
void ShpLpFeatureSchema::Delete(ShpConnection * connection)
{
    FdoPtr<FdoISelect> select = (FdoISelect*)connection->CreateCommand(FdoCommandType_Select);
	FdoPtr<FdoIFeatureReader> reader;

	FdoPtr<FdoFeatureSchema>	logicalSchema = this->GetLogicalSchema();
	FdoPtr<FdoClassCollection>  logicalClasses = logicalSchema->GetClasses();

	// Check in advance all the classes are empty
	for (int i = 0; i < logicalClasses->GetCount (); i++)
    {
        FdoPtr<FdoClassDefinition> logicalClass = logicalClasses->GetItem (i);
		select->SetFeatureClassName(logicalClass->GetQualifiedName());
		reader = select->Execute();

		if ( reader->ReadNext() )
			throw FdoException::Create(NlsMsgGet(SHP_CANT_DELETE_NON_EMPTY_SCHEMA, "Can't delete schema '%1$ls' because class '%2$ls' contains data.", (FdoString *)logicalSchema->GetQualifiedName(), (FdoString *)logicalClass->GetQualifiedName() ));

		reader->Close();
    }

    // Delete all Lp Classes contained by this Lp Schema:
    FdoPtr<ShpLpClassDefinitionCollection> lpClasses = this->GetLpClasses();
    for (int i = 0, size = lpClasses->GetCount (); i < size; i++)
    {
        FdoPtr<ShpLpClassDefinition> lpClass = lpClasses->GetItem (0);
        lpClass->Delete(connection);
    }

    // Remove this LpSchema from the LpSchemas collection:
    m_parentLpSchemas->Remove(this);

    // Delete the physical counterpart:
    // NOTE: there is no physical counterpart to delete (physical schemas don't exist)

    // Delete the logical counterpart:
    FdoPtr<FdoFeatureSchemaCollection> logicalSchemas = m_parentLpSchemas->GetLogicalSchemas();
    logicalSchemas->Remove(m_logicalSchema);
}


// Generate a new copy of the schema mappings for the feature schema, optionally including default mappings
FdoShpOvPhysicalSchemaMapping* ShpLpFeatureSchema::GetSchemaMappings(bool bIncludeDefaults)
{
    FdoPtr<FdoShpOvPhysicalSchemaMapping> schemaMappings = FdoShpOvPhysicalSchemaMapping::Create();
    bool bHasMappings = false;

    schemaMappings->SetName(this->GetName());

    FdoPtr<FdoShpOvClassCollection> classMappings = schemaMappings->GetClasses();
    for (int i=0; i<m_lpClasses->GetCount(); i++)
    {
        FdoPtr<ShpLpClassDefinition> lpClass = m_lpClasses->GetItem(i);
        FdoPtr<FdoShpOvClassDefinition> classMapping = lpClass->GetSchemaMappings(bIncludeDefaults);

        if (classMapping != NULL)
        {
            bHasMappings = true;
            FdoPtr<FdoShpOvClassCollection>(schemaMappings->GetClasses())->Add(classMapping);
        }
    }

    return bHasMappings ? FDO_SAFE_ADDREF(schemaMappings.p) : NULL;
}


///////////////////////////////////////////////////////////////////////////////


ShpLpFeatureSchemaCollection::ShpLpFeatureSchemaCollection(void)
{
    m_logicalSchemas = FdoFeatureSchemaCollection::Create (NULL);
}

ShpLpFeatureSchemaCollection::ShpLpFeatureSchemaCollection(
    ShpConnection * connection,
    ShpPhysicalSchema * physicalSchema,
	FdoFeatureSchemaCollection * configLogicalSchemas,
	FdoPhysicalSchemaMappingCollection * configSchemaMappings)
{
    if (NULL == physicalSchema)
        ConvertLogicalToPhysical (connection, configLogicalSchemas, configSchemaMappings);
    else
        ConvertPhysicalToLogical (connection, physicalSchema, configLogicalSchemas, configSchemaMappings);
}


// Convert logical to physical schema collection:
void ShpLpFeatureSchemaCollection::ConvertLogicalToPhysical (ShpConnection* connection,
    FdoFeatureSchemaCollection* configLogicalSchemas, FdoPhysicalSchemaMappingCollection* configSchemaMappings)
{
    // Create the logical schema collection:
    m_logicalSchemas = FdoFeatureSchemaCollection::Create(NULL);

    // If there are any logical schemas from a given configuration
	if (NULL != configLogicalSchemas && configLogicalSchemas->GetCount() > 0)
    {
        // Intersect them with the physical schemas and use given
		// logical->physical mapping
		FdoInt32 schemaCount = configLogicalSchemas->GetCount();
		for (FdoInt32 i=0;  i < schemaCount;  i++)
		{
            // Get one logical schema:
			FdoFeatureSchemaP configLogicalSchema = configLogicalSchemas->GetItem(i);

			FdoPhysicalSchemaMappingP configSchemaMapping =
				configSchemaMappings->GetItem(SHP_PROVIDER_NAME, configLogicalSchema->GetName());
			if (configSchemaMapping != NULL)
            {
			    FdoShpOvPhysicalSchemaMapping * shpSchemaMapping = 
				    static_cast<FdoShpOvPhysicalSchemaMapping *>(configSchemaMapping.p);

                // Create LogicalPhysical schema from logical and mapping layers (adds itself automatically to collection):
			    FdoPtr<ShpLpFeatureSchema> pLpSchema = new ShpLpFeatureSchema(
				    this, connection, NULL, configLogicalSchema, shpSchemaMapping);
            }
            else
            {
                FdoPtr<ShpPhysicalSchema> physical = connection->GetPhysicalSchema ();
                if (physical != NULL)
			        FdoPtr<ShpLpFeatureSchema> pLpSchema = new ShpLpFeatureSchema(
				        this, connection, physical, configLogicalSchema, NULL);
            }
		}
	}
}

// Convert physical to logical schema collection:
void ShpLpFeatureSchemaCollection::ConvertPhysicalToLogical(
    ShpConnection * connection,
    ShpPhysicalSchema * physicalSchema,
	FdoFeatureSchemaCollection * configLogicalSchemas,
	FdoPhysicalSchemaMappingCollection * configSchemaMappings)
{
    // Create the logical schema collection:
    m_logicalSchemas = FdoFeatureSchemaCollection::Create(NULL);

    // If there are any logical schemas from a given configuration
	if (NULL != configLogicalSchemas)
    {
        // Intersect them with the physical schemas and use given
		// logical->physical mapping
		FdoInt32 schemaCount = configLogicalSchemas->GetCount();
		for (FdoInt32 i=0;  i < schemaCount;  i++)
		{
            // Get one logical schema:
			FdoFeatureSchemaP configLogicalSchema = configLogicalSchemas->GetItem(i);

            // Find corresponding schema mapping (it is mandatory for a set configuration):
			FdoPhysicalSchemaMappingP configSchemaMapping =
				configSchemaMappings->GetItem(SHP_PROVIDER_NAME, configLogicalSchema->GetName());
            if (connection->IsConfigured () && configSchemaMapping == NULL)
			    throw FdoException::Create(NlsMsgGet(SHP_MISSING_SCHEMA_MAPPING, "Missing schema mapping for feature schema in configuration file."));
			FdoShpOvPhysicalSchemaMapping * shpSchemaMapping = 
				static_cast<FdoShpOvPhysicalSchemaMapping *>(configSchemaMapping.p);

            // Find corresponding physical schema:
			if (NULL == physicalSchema)
			    throw FdoException::Create(NlsMsgGet(SHP_MISSING_PHYSICAL_SCHEMA, "Missing physical schema."));

            // Create LogicalPhysical schema from logical, physical and mapping layers (adds itself automatically to collection):
			FdoPtr<ShpLpFeatureSchema> pLpSchema = new ShpLpFeatureSchema(
				this, connection, physicalSchema, configLogicalSchema, shpSchemaMapping);
		}
	}
	else // No configuration given
	{
		// Use default physical->logical mapping
		VALIDATE_POINTER(physicalSchema);

        // Create LogicalPhysical schema from physical layer only (no logical/mapping info given)  (adds itself automatically to collection):
        FdoPtr<ShpLpFeatureSchema> pLpSchema = new ShpLpFeatureSchema(
			this, connection,	physicalSchema, NULL, NULL);
	}
}

// Return pointer to cached logical schema collection:
FdoFeatureSchemaCollection* ShpLpFeatureSchemaCollection::GetLogicalSchemas()
{
    return FDO_SAFE_ADDREF(m_logicalSchemas.p);
}

