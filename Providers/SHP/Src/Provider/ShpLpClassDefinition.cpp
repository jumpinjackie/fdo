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
#include <ShpLpClassDefinition.h>
#include <ShpLpPropertyDefinition.h>
#include <ShpLpFeatureSchema.h>
#include <ShpProvider.h>
#include <ShpFileSet.h>
#include <../ShpRead/ShapeFile.h>
#include <../ShpRead/ColumnInfo.h>
#include <../Overrides/FdoShpXmlGlobals.h>
#include <time.h>
#ifndef _WIN32
#include <locale.h>
#endif

ShpLpClassDefinition::ShpLpClassDefinition(void)
{
}

ShpLpClassDefinition::~ShpLpClassDefinition(void)
{
    delete m_physicalColumns;
}


ShpLpClassDefinition::ShpLpClassDefinition(ShpLpFeatureSchema *parentLpSchema, ShpConnection *connection, ShpFileSet *physicalFileSet, FdoClassDefinition* configLogicalClass, FdoShpOvClassDefinition* configClassMapping)
:
    m_physicalFileSet(physicalFileSet),
    m_parentLpSchema(parentLpSchema),
    m_physicalColumns (NULL)
{
    VALIDATE_ARGUMENT(connection);

    m_connection = connection;  //NOTE: weak reference

    m_LpProperties = new ShpLpPropertyDefinitionCollection();

    // Convert logical to physical (or vice-versa):
    if (NULL == physicalFileSet)
        ConvertLogicalToPhysical(connection, configLogicalClass, configClassMapping);
    else
        ConvertPhysicalToLogical(connection, configLogicalClass, configClassMapping);

    // Add this LpClass to its LpSchema:
    FdoPtr<ShpLpClassDefinitionCollection> lpClasses = m_parentLpSchema->GetLpClasses();
    lpClasses->Add(this);
}


// Use the (Physical) table definition to populate the Logical/Physical and Logical members.
void ShpLpClassDefinition::ConvertPhysicalToLogical(
	ShpConnection * connection,
    FdoClassDefinition * configLogicalClass,   // logical layer
	FdoShpOvClassDefinition * configClassMapping)   // override layer
{
    // Determine logical class name;
	FdoString * logicalClassName = (NULL != configClassMapping) ? configClassMapping->GetName() : m_physicalFileSet->GetBaseName();

    // Create logical class:
    if (configLogicalClass == NULL)
        m_logicalClassDefinition = FdoFeatureClass::Create(logicalClassName, L"");  // default to FdoFeatureClass
    else
    {
        if (configLogicalClass->GetClassType() == FdoClassType_FeatureClass)
            m_logicalClassDefinition = FdoFeatureClass::Create(logicalClassName, configLogicalClass->GetDescription());
        else if (configLogicalClass->GetClassType() == FdoClassType_Class)
            m_logicalClassDefinition = FdoClass::Create(logicalClassName, configLogicalClass->GetDescription());
        else
            throw FdoException::Create(NlsMsgGet(SHP_UNSUPPORTED_CLASSTYPE, "The '%1$ls' class type is not supported by Shp.", FdoCommonMiscUtil::FdoClassTypeToString(configLogicalClass->GetClassType())));
    }
    // Populate the logical class's ClassCapabilities:
    FdoPtr<FdoClassCapabilities> classCapabilities = FdoClassCapabilities::Create(*m_logicalClassDefinition.p);
    classCapabilities->SetSupportsLocking(false);
    classCapabilities->SetSupportsLongTransactions(false);
    m_logicalClassDefinition->SetCapabilities(classCapabilities);
    // Populate the logical class's abstract flag:
    m_logicalClassDefinition->SetIsAbstract(false);


    // Convert physical columns to LogicalPhysical/Logical properties:

    // Cycle through all the properties in the dbf file and add them to the class
    m_physicalColumns = m_physicalFileSet->GetDbfFile()->NewColumnInfo ();
    VALIDATE_POINTER(m_physicalColumns);
    int count = m_physicalColumns->GetNumColumns ();
    for (int i = 0; i < count; i++)
    {
        // Get corresponding property mapping (if any):
        FdoPtr<FdoShpOvPropertyDefinition> configPropertyMapping; 
        FdoPtr<FdoPropertyDefinition> configLogicalProperty;
        if (NULL != configClassMapping)
        {
            configPropertyMapping = configClassMapping->FindByColumnName(m_physicalColumns->GetColumnNameAt (i));
            if ((NULL != configLogicalClass) && (configPropertyMapping != NULL))
            {
                FdoPtr<FdoPropertyDefinitionCollection> configLogicalProperties = configLogicalClass->GetProperties();
                configLogicalProperty = configLogicalProperties->GetItem(configPropertyMapping->GetName());
            }
        }

        // TODO: this quietly throws away properties that don't have explicit overrides; this seems wrong w.r.t other providers.
        if (NULL == configClassMapping || (configClassMapping != NULL && configPropertyMapping != NULL))
        {
            FdoPtr<ShpLpPropertyDefinition> pDataPropertyDefinition =
                new ShpLpPropertyDefinition(this, i, configLogicalProperty, configPropertyMapping, true);
        }
    }
    // update the column offsets
    int coloff = 1;
    for (int i = 0; i < count; i++)
    {
        m_physicalColumns->SetColumnOffset (i, coloff);
        coloff += m_physicalColumns->GetColumnWidthAt (i);
    }

    FdoPtr<FdoPropertyDefinitionCollection> logicalProperties = m_logicalClassDefinition->GetProperties();

    // Create & add the geometry property (always exactly one geometry property):
    FdoPtr<FdoGeometricPropertyDefinition> logicalGeomProp = ConvertPhysicalToLogicalGeometryProperty(connection, configLogicalClass, m_physicalFileSet);
    if (logicalGeomProp != NULL)
    {
        logicalProperties->Add(logicalGeomProp);
        if (m_logicalClassDefinition->GetClassType() == FdoClassType_FeatureClass)
        {
            FdoFeatureClass *logicalFeatureClass = static_cast<FdoFeatureClass*>(m_logicalClassDefinition.p);
            logicalFeatureClass->SetGeometryProperty(logicalGeomProp);
        }
    }

    // Create & add the identity property (always exactly one identity property):
    FdoPtr<FdoDataPropertyDefinition> pLogicalIdentityProperty = ConvertPhysicalToLogicalIdentityProperty(configLogicalClass);
    FdoPtr<FdoDataPropertyDefinitionCollection> pIdentityProperties = m_logicalClassDefinition->GetIdentityProperties();
    VALIDATE_POINTER(pIdentityProperties);
    logicalProperties->Add(pLogicalIdentityProperty);
    pIdentityProperties->Add(pLogicalIdentityProperty);


    // Add the logical class to its logical schema:
    FdoPtr<FdoFeatureSchema> logicalSchema = m_parentLpSchema->GetLogicalSchema();
    FdoPtr<FdoClassCollection> logicalClasses = logicalSchema->GetClasses();
    logicalClasses->Add(m_logicalClassDefinition);
}



ShpLpPropertyDefinitionCollection* ShpLpClassDefinition::GetLpProperties() const
{
    return FDO_SAFE_ADDREF(m_LpProperties.p);
}


// Get physical fileset:
ShpFileSet* ShpLpClassDefinition::GetPhysicalFileSet(void)
{
    return m_physicalFileSet;
}

ColumnInfo* ShpLpClassDefinition::GetPhysicalColumnInfo (void)
{
    return m_physicalColumns;
}

// Get logical class:
FdoClassDefinition* ShpLpClassDefinition::GetLogicalClass(void)
{
    return FDO_SAFE_ADDREF(m_logicalClassDefinition.p);
}

void ShpLpClassDefinition::SetParent (ShpLpFeatureSchema* parent)
{
    m_parentLpSchema = parent; // weak reference
}


FdoString* ShpLpClassDefinition::GetName()
{
    return m_logicalClassDefinition == NULL ? L"" : m_logicalClassDefinition->GetName();
}


void ShpLpClassDefinition::ConvertLogicalToPhysical(
	ShpConnection * connection,
    FdoClassDefinition * configLogicalClass,   // logical layer
	FdoShpOvClassDefinition * configClassMapping)   // override layer
{
    FdoString* directory;
    FdoPtr<FdoClassDefinition> logicalClass;
    FdoString* logicalClassName;
    FdoPtr<FdoGeometricPropertyDefinition> geometry;
    FdoPtr<FdoPropertyDefinition> configLogicalProperty;
    int physicalColumnCount;
    eShapeTypes shape_type;
    size_t length;
    wchar_t* dbfFilename;
    wchar_t* shpFilename;
	wchar_t* cpgFilename;

    VALIDATE_ARGUMENT(configLogicalClass);

    // duplicate the class definition
    logicalClass = FdoCommonSchemaUtil::DeepCopyFdoClassDefinition (configLogicalClass);

    logicalClassName = logicalClass->GetName ();
    if ((NULL == logicalClassName) || (0 == wcslen (logicalClassName)))
        throw FdoException::Create (NlsMsgGet(SHP_CLASS_NAME_INVALID, "The class name '%1$s' is invalid.", logicalClassName));

    // Validate the logical class
    /////////////////////////////

    // Get the geometry property if it has one:
    FdoPtr<FdoPropertyDefinitionCollection> configLogicalProperties = logicalClass->GetProperties();
    FdoPtr<FdoGeometricPropertyDefinition> logicalGeometryProperty;
    FdoInt32 logicalPropertyCount = configLogicalProperties->GetCount();
    for (int i = 0; i < logicalPropertyCount; i++)
    {
        FdoPtr<FdoPropertyDefinition> logicalProperty = configLogicalProperties->GetItem (i);
        if (FdoPropertyType_GeometricProperty == logicalProperty->GetPropertyType ())
            if (geometry != NULL)
                throw FdoException::Create (NlsMsgGet(SHP_SCHEMA_EXCESSIVE_GEOMETRY, "The class '%1$ls' contains more than one geometry property.", logicalClassName));
            else
                geometry = static_cast<FdoGeometricPropertyDefinition*>(FDO_SAFE_ADDREF (logicalProperty.p));
    }

    // Validate that there is exactly one identity property of type int64:
    FdoPtr<FdoDataPropertyDefinitionCollection> logicalIdProperties = logicalClass->GetIdentityProperties();
    FdoPtr<FdoDataPropertyDefinition> logicalIdProperty;
    if (logicalIdProperties->GetCount() == 0)
    {
        logicalIdProperty = FdoDataPropertyDefinition::Create (FEATID_PROPERTY, L"");
        logicalIdProperty->SetDataType (FdoDataType_Int32);
        logicalIdProperty->SetIsAutoGenerated (true);
        logicalIdProperty->SetNullable (false);
        configLogicalProperties->Add (logicalIdProperty);
        logicalIdProperties->Add (logicalIdProperty);
        logicalPropertyCount = configLogicalProperties->GetCount();
    }
    else if (logicalIdProperties->GetCount() > 1)
        throw FdoException::Create(NlsMsgGet(SHP_TOO_MANY_IDENTITY_PROPERTIES, "FDO class '%1$ls' has more than one identity property; only one identity property is supported.", logicalClassName));
    else
    {
        logicalIdProperty = logicalIdProperties->GetItem(0);
        if (logicalIdProperty->GetDataType() != FdoDataType_Int32)
            throw FdoException::Create(NlsMsgGet(SHP_IDENTITY_PROPERTY_INCORRECT_TYPE, "FDO class '%1$ls' has an identity property of type '%2$ls'; only type 'Int32' is supported.",
                logicalClassName, FdoCommonMiscUtil::FdoDataTypeToString(logicalIdProperty->GetDataType())));
    }

    // Convert the logical classname to physical filenames:
    if (configClassMapping == NULL)
    {
        // No mapping provided; default to classname as filename, and store in directory provided in connection string:
        bool IsValidName = true;
        wchar_t buffer[20];
        wchar_t* name;
        FdoStringP physicalFilename;

        directory = connection->GetDirectory ();
        if (directory == NULL)
            throw FdoException::Create(NlsMsgGet(SHP_NO_DIRECTORY_SPECIFIED, "The '%1$ls' connection property was not specified and no shape file location override was specified.", CONNECTIONPROPERTY_DEFAULT_FILE_LOCATION));
        length = wcslen (directory);

        if (NULL != wcspbrk (logicalClassName, L"<>?*\\/:\"'|"))
            IsValidName = false;
        else
            physicalFilename = logicalClassName;

        if (!IsValidName)
        {
            // use CLASSnnn, create our own rather than use FdoCommonFile::GetTempFile
            srand ((unsigned)time (NULL));
            name = (wchar_t*)alloca (sizeof(wchar_t) * (length + 30));
            do
            {
#ifdef _WIN32
                swprintf (buffer, sizeof(buffer)/sizeof(wchar_t), L"CLASS%d", rand ());
                swprintf (name, length+30, L"%s%s%s", directory, buffer, SHP_EXTENSION);
#else
                swprintf (buffer, sizeof(buffer)/sizeof(wchar_t), L"CLASS%d", rand() % 1000);
                swprintf (name, length+30, L"%s%s%s", directory, buffer, SHP_EXTENSION);
#endif
            }
            while (FdoCommonFile::FileExists (name));
            physicalFilename = buffer;
        }

        length += wcslen (physicalFilename);

        // Build shpFilename:
        shpFilename = (wchar_t*)alloca (sizeof(wchar_t) * (length + wcslen (SHP_EXTENSION) + 1));
        wcscpy (shpFilename, directory);
        wcscat (shpFilename, physicalFilename);
        wcscat (shpFilename, SHP_EXTENSION);

        // Build dbfFilename:
        dbfFilename = (wchar_t*)alloca (sizeof(wchar_t) * (length + wcslen (DBF_EXTENSION) + 1));
        wcscpy (dbfFilename, directory);
        wcscat (dbfFilename, physicalFilename);
        wcscat (dbfFilename, DBF_EXTENSION);

		// Build cpgFilename:
        cpgFilename = (wchar_t*)alloca (sizeof(wchar_t) * (length + wcslen (CPG_EXTENSION) + 1));
        wcscpy (cpgFilename, directory);
        wcscat (cpgFilename, physicalFilename);
        wcscat (cpgFilename, CPG_EXTENSION);
    }
    else
    {
        // Build shpFilename:
        if (configClassMapping->GetShapeFile () == L"")
            throw FdoException::Create(NlsMsgGet(SHP_OVERRIDE_FILENAME_NOT_QUALIFIED, "The SHP filename override '%1$ls' is not a fully-qualified path.", configClassMapping->GetShapeFile()));
        shpFilename = (wchar_t*)alloca(sizeof(wchar_t) * (1 + wcslen(configClassMapping->GetShapeFile())));
        wcscpy(shpFilename, configClassMapping->GetShapeFile());

        // Build dbfFilename:
        if (0 != FdoCommonOSUtil::wcsnicmp (shpFilename + wcslen(shpFilename) - wcslen(SHP_EXTENSION), SHP_EXTENSION, wcslen(SHP_EXTENSION)))
            throw FdoException::Create(NlsMsgGet(SHP_OVERRIDE_FILENAME_NOT_QUALIFIED, "The SHP filename override '%1$ls' is not a fully-qualified path.", shpFilename));
        dbfFilename = (wchar_t*)alloca(sizeof(wchar_t) * (1 + wcslen(shpFilename)));
        wcscpy(dbfFilename, shpFilename);
        wcscpy(dbfFilename + wcslen(shpFilename) - wcslen(SHP_EXTENSION), DBF_EXTENSION);
    }


    // Create the physical column info (if any):
    physicalColumnCount = logicalPropertyCount;
    physicalColumnCount--;  // subtract one since we don't store the identity property
    if (geometry != NULL)
        physicalColumnCount--; // subtract one if there's a geometry property, since we don't store it in the DBF
    if (0 != physicalColumnCount)
        m_physicalColumns = ShapeDBF::NewColumnInfo (physicalColumnCount);
    else
        m_physicalColumns = NULL;

    try
    {
        // Convert each logical data property to physical column info:
        FdoPtr<FdoShpOvPropertyDefinitionCollection> configPropertyMappings = (configClassMapping == NULL)
            ? NULL : configClassMapping->GetProperties();
        for (int i = 0, n = 0; i < logicalPropertyCount; i++)
        {
            configLogicalProperty = configLogicalProperties->GetItem (i);

            if (FdoPropertyType_GeometricProperty == configLogicalProperty->GetPropertyType ())
            {
                geometry = static_cast<FdoGeometricPropertyDefinition*>(FDO_SAFE_ADDREF (configLogicalProperty.p));
                // get the shape type and check for only one geometry type
                shape_type = ShpSchemaUtilities::FdoGeometryToShapeType (
                    geometry->GetGeometryTypes (),
                    geometry->GetHasElevation (),
                    geometry->GetHasMeasure ());
            }
            else if (FdoPropertyType_DataProperty == configLogicalProperty->GetPropertyType ())
            {
                if (configLogicalProperty != logicalIdProperty)
                {
                    // Only non-identity data properties have mappings:
                    FdoPtr<FdoShpOvPropertyDefinition> configPropertyMapping = (configPropertyMappings==NULL)
                        ? NULL : configPropertyMappings->GetItem(configLogicalProperty->GetName());

                    FdoPtr<ShpLpPropertyDefinition> lpProperty = new ShpLpPropertyDefinition(this, n++, configLogicalProperty, configPropertyMapping, false);
                }
            }
            else
            {
                throw FdoException::Create (NlsMsgGet(SHP_SCHEMA_CREATION_FAILED, "Schema create failed."));
            }
        }
        if (NULL != m_physicalColumns)
        {
            // update the column offsets
            int coloff = 1;
            for (int i = 0; i < logicalPropertyCount; i++)
            {
                m_physicalColumns->SetColumnOffset (i, coloff);
                coloff += m_physicalColumns->GetColumnWidthAt (i);
            }
        }

        // create the files:
        if (m_physicalColumns != NULL) 
		{
            ShapeDBF _dbf (dbfFilename, m_physicalColumns);

			if (configClassMapping == NULL )
				ShapeCPG _cpg (cpgFilename, setlocale(LC_ALL, NULL ));
		}

        if (geometry != NULL)
            ShapeFile _shp (shpFilename, shape_type, geometry->GetHasMeasure ());
	

        // Create the physical FileSet:
        FdoString* base = ShpFileSet::CreateBaseName (shpFilename);
        m_physicalFileSet = new ShpFileSet(base, m_connection->GetTemporary());
        delete[] base;

        // Add the physical FileSet to its parent FileSet collection:
        FdoPtr<ShpPhysicalSchema> physicalSchema = m_parentLpSchema->GetPhysicalSchema();
        physicalSchema->AddFileSet(m_physicalFileSet);

        // Hang on to the copy of the logical class:
        m_logicalClassDefinition = FDO_SAFE_ADDREF(logicalClass.p);

        // Add the logical class to its parent collection (may already be there if the whole schema was added) :
        FdoPtr<FdoFeatureSchema> logicalSchema = m_parentLpSchema->GetLogicalSchema();
        FdoPtr<FdoClassCollection> logicalClasses = logicalSchema->GetClasses();
        bool bAlreadyInCollection = false;
        for (FdoInt32 i=0; i<logicalClasses->GetCount() && !bAlreadyInCollection; i++)
        {
            FdoPtr<FdoClassDefinition> logicalClass = logicalClasses->GetItem(i);
            if (0 == wcscmp (logicalClass->GetName (), m_logicalClassDefinition->GetName ()))
                bAlreadyInCollection = true;
        }
        if (!bAlreadyInCollection)
            logicalClasses->Add (m_logicalClassDefinition);
    }
    catch (...)
    {
        delete m_physicalColumns;
        throw;
    }
}


// Permanently delete the Physical, LogicalPhysical and Logical counterparts of this class:
void ShpLpClassDefinition::Delete(ShpConnection* connection)
{
    ShpFileSet* fileset;
    wchar_t* shp_name;
    wchar_t* dbf_name;
    wchar_t* shx_name;
    wchar_t* idx_name;
    wchar_t* prj_name;
	wchar_t* cpg_name;

	bool rc;
    fileset = GetPhysicalFileSet ();

    // Close and delete the SHP file:
    ShapeFile* shp = fileset->GetShapeFile ();
    shp_name = (wchar_t*)alloca (sizeof (wchar_t) * (1 + wcslen (shp->FileName ())));
    wcscpy (shp_name, shp->FileName ());
    shp->CloseFile ();
    rc = FdoCommonFile::Delete (shp_name);


    // Close and delete the DBF file:
    ShapeDBF* dbf = fileset->GetDbfFile ();
    dbf_name = (wchar_t*)alloca (sizeof (wchar_t) * (1 + wcslen (dbf->FileName ())));
    wcscpy (dbf_name, dbf->FileName ());
    dbf->CloseFile ();
    rc = FdoCommonFile::Delete (dbf_name);


    // Close and delete the SHX file:
    ShapeIndex* shx = fileset->GetShapeIndexFile ();
    shx_name = (wchar_t*)alloca (sizeof (wchar_t) * (1 + wcslen (shx->FileName ())));
    wcscpy (shx_name, shx->FileName ());
    shx->CloseFile ();
    rc = FdoCommonFile::Delete (shx_name);

    // Close and delete the IDX file:
    ShpSpatialIndex* idx = fileset->GetSpatialIndex ();
    idx_name = (wchar_t*)alloca (sizeof (wchar_t) * (1 + wcslen (idx->FileName ())));
    wcscpy (idx_name, idx->FileName ());
    idx->CloseFile ();
    rc = FdoCommonFile::Delete (idx_name);

    // Close and delete the PRJ file:
    ShapePRJ* prj = fileset->GetPrjFile();
    if ( prj != NULL )
    {
        prj_name = (wchar_t*)alloca (sizeof (wchar_t) * (1 + wcslen (prj->FileName ())));
        wcscpy (prj_name, prj->FileName ());
        prj->CloseFile ();
        FdoCommonFile::Delete (prj_name);
    }

	// Close and delete the CPG file:
    ShapeCPG* cpg = fileset->GetCpgFile ();
	if ( cpg != NULL )
	{
		cpg_name = (wchar_t*)alloca (sizeof (wchar_t) * (1 + wcslen (cpg->FileName ())));
		wcscpy (cpg_name, cpg->FileName ());
		cpg->CloseFile ();
		rc = FdoCommonFile::Delete (cpg_name);
	}

	// Mark de files as deleted in aid of its destructor
	fileset->SetFilesDeleted();

    // Remove this Lp Class from its parent:
    FdoPtr<ShpLpClassDefinitionCollection> lpClasses = m_parentLpSchema->GetLpClasses();
    lpClasses->Remove(this);

    // Remove the logical class from its parent:
    FdoPtr<FdoFeatureSchema> logicalSchema = m_parentLpSchema->GetLogicalSchema();
    FdoPtr<FdoClassCollection> logicalClasses = logicalSchema->GetClasses();
    FdoPtr<FdoClassDefinition> logicalDefinition = logicalClasses->FindItem (m_logicalClassDefinition->GetName ());
    logicalClasses->Remove (logicalDefinition);

    // Remove the physical fileset from its parent:
    FdoPtr<ShpPhysicalSchema> physicalSchema = m_parentLpSchema->GetPhysicalSchema();
	physicalSchema->RemoveFileSet(m_physicalFileSet);
	
	if ( fileset == m_connection->GetLastEditedFileSet() )
		m_connection->SetLastEditedFileSet(NULL);
}




// Convert physical identity 'column' to/from logical identity property;
// NOTE: by design there is no override for identity properties
FdoDataPropertyDefinition* ShpLpClassDefinition::ConvertPhysicalToLogicalIdentityProperty(FdoClassDefinition* configLogicalClass)
{
    // Get logical identity property name:
    FdoStringP logicalIdentityPropertyName;
    FdoStringP logicalIdentityPropertyDescription;
    if (NULL != configLogicalClass)
    {
        FdoDataPropertiesP logicalIdProps = configLogicalClass->GetIdentityProperties();
        VALIDATE_POINTER(logicalIdProps);
        FdoInt32 count = logicalIdProps->GetCount();
        //TODO: throw an exception if 0 or more than 1 identity property
        FdoDataPropertyP logicalIdProp = logicalIdProps->GetItem(0);
        VALIDATE_POINTER(logicalIdProp);
        logicalIdentityPropertyName = logicalIdProp->GetName();
        logicalIdentityPropertyDescription = logicalIdProp->GetDescription ();
    }
    else
    {
        // There is no key column in the physical layer, so use a default name here:
        logicalIdentityPropertyName = FEATID_PROPERTY;
        logicalIdentityPropertyDescription = L"";
    }

    // Create logical identity property:
    FdoPtr<FdoDataPropertyDefinition> pIdentityPropertyDefinition =
        FdoDataPropertyDefinition::Create(logicalIdentityPropertyName, logicalIdentityPropertyDescription);
    pIdentityPropertyDefinition->SetDataType (FdoDataType_Int32);
    pIdentityPropertyDefinition->SetIsAutoGenerated (true);
    pIdentityPropertyDefinition->SetNullable (false);
    pIdentityPropertyDefinition->SetReadOnly (true);

    // Return the logical identity property;
    return FDO_SAFE_ADDREF(pIdentityPropertyDefinition.p);
}



// Convert physical geometry 'column' to/from logical geometry property;
// NOTE: by design there is no override for geometry properties:
FdoGeometricPropertyDefinition* ShpLpClassDefinition::ConvertPhysicalToLogicalGeometryProperty(ShpConnection *connection, FdoClassDefinition* configLogicalClass, ShpFileSet *physicalFileSet)
{
    FdoGeometricType geometry_type;
    bool has_z;
    bool has_m;

    ShapeFile* shp = physicalFileSet->GetShapeFile ();
    int shape_type = shp->GetFileShapeType ();
    if (eNullShape == shape_type)
        return (NULL);

    // Get the logical geometry property name:
    FdoStringP logicalGeomPropertyName;
    FdoStringP logicalGeomPropertyDescription;
    if (NULL != configLogicalClass)
    {
        if (configLogicalClass->GetClassType() == FdoClassType_FeatureClass)
        {
            FdoFeatureClass* configLogicalFeatureClass = static_cast<FdoFeatureClass*>(configLogicalClass);
            FdoPtr<FdoGeometricPropertyDefinition> geomProperty = configLogicalFeatureClass->GetGeometryProperty();
            logicalGeomPropertyName = geomProperty->GetName();
            logicalGeomPropertyDescription = geomProperty->GetDescription ();
            geometry_type = (FdoGeometricType)geomProperty->GetGeometryTypes ();
            has_z = geomProperty->GetHasElevation ();
            has_m = geomProperty->GetHasMeasure ();
        }
        else if (configLogicalClass->GetClassType() == FdoClassType_Class)
        {
            // find the geometry property:
            FdoPtr<FdoPropertyDefinitionCollection> configLogicalProperties = configLogicalClass->GetProperties();
            for (FdoInt32 i=0; i<configLogicalProperties->GetCount(); i++)
            {
                FdoPtr<FdoPropertyDefinition> configLogicalProperty = configLogicalProperties->GetItem(i);
                if (configLogicalProperty->GetPropertyType() == FdoPropertyType_GeometricProperty)
                {
                    if (logicalGeomPropertyName == L"")
                    {
                        logicalGeomPropertyName = configLogicalProperty->GetName();
                        logicalGeomPropertyDescription = configLogicalProperty->GetDescription ();
                        FdoGeometricPropertyDefinition* geometry = (FdoGeometricPropertyDefinition*)configLogicalProperty.p;
                        geometry_type = (FdoGeometricType)geometry->GetGeometryTypes ();
                        has_z = geometry->GetHasElevation ();
                        has_m = geometry->GetHasMeasure ();
                    }
                    else
                    {
                        FdoString* class_name = configLogicalClass->GetQualifiedName();
                        throw FdoException::Create(NlsMsgGet(SHP_SCHEMA_EXCESSIVE_GEOMETRY, "The class '%1$ls' contains more than one geometry property.", class_name));
                    }
                }
            }
        }
        else
            throw FdoException::Create(NlsMsgGet(SHP_UNSUPPORTED_CLASSTYPE, "The '%1$ls' class type is not supported by Shp.", FdoCommonMiscUtil::FdoClassTypeToString(configLogicalClass->GetClassType())));
    }
    else
    {
        logicalGeomPropertyName = GEOMETRY_PROPERTY;
        logicalGeomPropertyDescription = L"";
        // Convert physical fileset's shapetype to logical geometry type/measure/elevation:
        switch (shape_type)
        {
            case ePointShape:
                geometry_type = FdoGeometricType_Point;
                has_z = false;
                has_m = false;
                break;
            case eMultiPointShape:
                geometry_type = FdoGeometricType_Point;
                has_z = false;
                has_m = false;
                break;
            case ePointZShape:
                geometry_type = FdoGeometricType_Point;
                has_z = true;
                has_m = shp->HasMData ();
                break;
            case eMultiPointZShape:
                geometry_type = FdoGeometricType_Point;
                has_z = true;
                has_m = shp->HasMData ();
                break;
            case ePointMShape:
                geometry_type = FdoGeometricType_Point;
                has_z = false;
                has_m = true;
                break;
            case eMultiPointMShape:
                geometry_type = FdoGeometricType_Point;
                has_z = false;
                has_m = true;
                break;
            case ePolylineShape:
                geometry_type = FdoGeometricType_Curve;
                has_z = false;
                has_m = false;
                break;
            case ePolylineZShape:
                geometry_type = FdoGeometricType_Curve;
                has_z = true;
                has_m = shp->HasMData ();
                break;
            case ePolylineMShape:
                geometry_type = FdoGeometricType_Curve;
                has_z = false;
                has_m = true;
                break;
            case ePolygonShape:
                geometry_type = FdoGeometricType_Surface;
                has_z = false;
                has_m = false;
                break;
            case ePolygonZShape:
                geometry_type = FdoGeometricType_Surface;
                has_z = true;
                has_m = shp->HasMData ();
                break;
            case ePolygonMShape:
                geometry_type = FdoGeometricType_Surface;
                has_z = false;
                has_m = true;
                break;
            case eMultiPatchShape:
                geometry_type = FdoGeometricType_Surface;
                has_z = true;
    #pragma message ("ToDo: check for valid Z values in multi patch shape file")
                has_m = shp->HasMData ();
                break;
            case eNullShape:
            default:
                throw FdoSchemaException::Create(NlsMsgGet(SHP_UNKNOWN_SHAPE_TYPE, "The shape type number '%1$d' is unknown.", shape_type));
        }
    }


    // Create the logical geometry property:
    FdoPtr<FdoGeometricPropertyDefinition> pLogicalGeomProperty = FdoGeometricPropertyDefinition::Create(logicalGeomPropertyName, logicalGeomPropertyDescription);
    pLogicalGeomProperty->SetGeometryTypes (geometry_type);
    pLogicalGeomProperty->SetReadOnly (false);
    pLogicalGeomProperty->SetHasElevation (has_z);
    pLogicalGeomProperty->SetHasMeasure (has_m);

    // Set spatial context: the name of the SC is the same with the name
	ShapePRJ*  prj = physicalFileSet->GetPrjFile();
	FdoStringP csysName;
	
	if ( prj )
		csysName = prj->GetCoordSysName();
	else
	{
		// Get the default one (may be from config file)
		ShpSpatialContextCollectionP scColl = connection->GetSpatialContexts();
		ShpSpatialContextP		sc = scColl->GetItem(0);
		csysName = sc->GetName();
	}

    pLogicalGeomProperty->SetSpatialContextAssociation(csysName);

    // Return the logical geometry property:
    return FDO_SAFE_ADDREF(pLogicalGeomProperty.p);
}



// Generate a new copy of the schema mappings for this class, optionally including default mappings
FdoShpOvClassDefinition* ShpLpClassDefinition::GetSchemaMappings(bool bIncludeDefaults)
{
    FdoPtr<FdoShpOvClassDefinition> classMapping = FdoShpOvClassDefinition::Create();
    bool bHasMappings = false;

    ShapeFile* shp = GetPhysicalFileSet ()->GetShapeFile ();
    classMapping->SetName( GetName() );
    if (!shp->IsTemporaryFile ())
    {
        // Set shapefile name if filename != connection folder + class name:
        FdoStringP defaultShapefileNameNoExt = m_connection->GetDirectory();
        defaultShapefileNameNoExt += GetName();
        FdoString* actualShapefileName = shp->FileName();
        if (bIncludeDefaults || (0!=wcsncmp(actualShapefileName, defaultShapefileNameNoExt, wcslen(actualShapefileName)-4)))
        {
            bHasMappings = true;
			FdoStringP pFullFileName = FdoCommonFile::GetAbsolutePath(GetPhysicalFileSet()->GetShapeFile()->FileName());
			FdoStringP pFullFolderName = FdoCommonFile::GetAbsolutePath(m_connection->GetDirectory());
			classMapping->SetShapeFile(FdoCommonFile::GetRelativePath(pFullFolderName, pFullFileName));
        }
    }
    // Set property mappings:
    FdoPtr<FdoShpOvPropertyDefinitionCollection> propertyMappings = classMapping->GetProperties();
    for (FdoInt32 i=0; i<m_LpProperties->GetCount(); i++)
    {
        FdoPtr<ShpLpPropertyDefinition> lpProperty = m_LpProperties->GetItem(i);
        FdoPtr<FdoShpOvPropertyDefinition> propertyMapping = lpProperty->GetSchemaMappings(bIncludeDefaults);

        if (propertyMapping != NULL)
        {
            bHasMappings = true;
            propertyMappings->Add(propertyMapping);
        }
    }

    return bHasMappings ? FDO_SAFE_ADDREF(classMapping.p) : NULL;
}
