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
#include "ArcSDEUtils.h"

ArcSDEApplySchemaCommand::ArcSDEApplySchemaCommand (FdoIConnection *connection) :
    ArcSDECommand<FdoIApplySchema> (connection),
    mSchema (NULL)
{
}

/** Do not implement the copy constructor. **/
//ArcSDEApplySchemaCommand::ArcSDEApplySchemaCommand (const ArcSDEApplySchemaCommand &right) { }

ArcSDEApplySchemaCommand::~ArcSDEApplySchemaCommand (void)
{
    FDO_SAFE_RELEASE (mSchema);
}

/** Do not implement the assignment operator. **/
//ArcSDEApplySchemaCommand & operator= (const ArcSDEApplySchemaCommand &right);

///<summary>Gets the schema to create.</summary>
/// <returns>Returns the schema that would be created.</returns> 
FdoFeatureSchema* ArcSDEApplySchemaCommand::GetFeatureSchema ()
{
    return (FDO_SAFE_ADDREF (mSchema));
}

///<summary>Sets the schema to create.</summary>
/// <param name="value">Input the definition of the schema to create.</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEApplySchemaCommand::SetFeatureSchema (FdoFeatureSchema* value)
{
    FDO_SAFE_RELEASE (mSchema);
    mSchema = FDO_SAFE_ADDREF (value);
}

/// <summary> Gets the FdoPhysicalSchemaMapping used to specify how the schema definition
/// gets mapped into physical storage.</summary>
/// <returns>Returns FdoPhysicalSchemaMapping</returns> 
FdoPhysicalSchemaMapping* ArcSDEApplySchemaCommand::GetPhysicalMapping ()
{
    //TODO
    return (NULL);
}

///<summary>Sets the FdoPhysicalSchemaMapping used to specify how the schema definition
///gets mapped into physical storage.</summary>
/// <param name="value">Input the FdoPhysicalSchemaMapping</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEApplySchemaCommand::SetPhysicalMapping (FdoPhysicalSchemaMapping* value)
{
    //TODO
}



/// <summary> Indicates whether Execute() will ignore element states 
/// when applying the feature schema.</summary>
/// <returns>Returns true if elements states will be ignored, false otherwise. 
/// </returns> 
FdoBoolean ArcSDEApplySchemaCommand::GetIgnoreStates()
{
    //TODO
    return false;
}

///<summary>Changes the handling of element states by Execute().</summary>
/// <param name="ignoreStates">Input True: Execute() will ignore the 
/// element states on the Feature Schema elements. Instead, it 
/// will additively merge the Feature Schema into the current DataStore.
/// False: Execute() will use the element states to determine whether
/// each element is inserted, modified or deleted.</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEApplySchemaCommand::SetIgnoreStates( FdoBoolean ignoreStates )
{
    //TODO
}


//
// internals
//

// Generates a unique table name for the given class;
// Caller is responsible for allocation table_name with size >= SE_MAX_SCHEMA_TABLE_LEN
void ArcSDEApplySchemaCommand::generateUniqueTableName (FdoClassDefinition* cls, bool bStrict, CHAR* generatedTableName)
{
    CHAR* class_name = NULL;
    LONG result = 0L;

    FdoPtr<ArcSDEConnection> connection = static_cast<ArcSDEConnection*>(GetConnection ());

    // Put all (qualified) table names currently in use into tableNames;
    // NOTE: we dont cache this info as it will change every time we call ApplySchema:
    std::vector<sde_std_string> tableNames;
    FdoPtr<FdoFeatureSchemaCollection> cachedSchemas = mConnection->GetSchemaCollection();
    for (FdoInt32 i=0; i<cachedSchemas->GetCount(); i++)
    {
        FdoPtr<FdoFeatureSchema> cachedSchema = cachedSchemas->GetItem(i);
        FdoPtr<FdoClassCollection> classes = cachedSchema->GetClasses();
        for (FdoInt32 iClassIndex=0; iClassIndex<classes->GetCount(); iClassIndex++)
        {
            CHAR table[SE_QUALIFIED_TABLE_NAME];
            FdoPtr<FdoClassDefinition> aClass = classes->GetItem(iClassIndex);
            connection->ClassToTable(table, aClass);
            tableNames.push_back(sde_cstwc(table));
        }
    }

    // Get the given class's underlying ArcSDE owner name:
    FdoPtr<FdoFeatureSchema> schema = static_cast<FdoFeatureSchema*>(cls->GetParent());

    CHAR ownerName[SE_MAX_OWNER_LEN+1];
    CHAR dbName[SE_MAX_DATABASE_LEN+1];
    connection->ClassToUser(ownerName, cls);
    connection->ClassToDatabase(dbName, cls);

    // Generate the unique table name:
    CHAR *mbClassName = NULL;
    sde_wide_to_multibyte(mbClassName, cls->GetName());
    generateUniqueName(connection, tableNames, SE_MAX_SCHEMA_TABLE_LEN, mbClassName, bStrict, dbName, ownerName, generatedTableName);
}

// Generates unique column names for each of the properties in the given property collection;
void ArcSDEApplySchemaCommand::generateUniqueColumnNames (FdoPropertyDefinitionCollection* properties, bool bStrict, std::vector<sde_std_string> &columnNames)
{
    CHAR* property_name = NULL;

    // start with original fdo property names as column names:
    columnNames.clear();
    for (int i=0; i<properties->GetCount(); i++)
    {
        FdoPtr<FdoPropertyDefinition> property = properties->GetItem(i);
        sde_wide_to_multibyte(property_name, property->GetName());
        columnNames.push_back(sde_pcus2wc(property_name));
    }

    // Generate a unique name for each column:
    for (unsigned int i=0; i<columnNames.size(); i++)
    {
        // Store all column names EXCEPT this one in columnNamesInUse:
        std::vector<sde_std_string> columnNamesInUse;
        for (unsigned int j=0; j<columnNames.size(); j++)
            if (j!=i)
                columnNamesInUse.push_back(columnNames.at(j));

        // Generate a unique name for this column:
        // NOTE: ArcSDE's SE_MAX_COLUMN_LEN is too long for some underlying RDBMS's, so we use our own ARCSDE_MAX_CREATED_COLUMN_LEN isntead.
        CHAR generatedColumnName[ARCSDE_MAX_CREATED_COLUMN_LEN];
        generateUniqueName(mConnection, columnNamesInUse, ARCSDE_MAX_CREATED_COLUMN_LEN, sde_pcwc2us(columnNames.at(i).c_str()), bStrict, NULL, NULL, generatedColumnName);

        // Overwrite the original given name:
        columnNames[i] = sde_pcus2wc(generatedColumnName);
    }
}

// sets the sde_type, size, decimal_digits and nulls_allowed field of the column definition based on the property
void ArcSDEApplySchemaCommand::getDataType (FdoPropertyDefinition* property, SE_COLUMN_DEF& column_definition)
{
    FdoDataType type;

    switch (property->GetPropertyType ())
    {
        // Represents a Data Property type.
        case FdoPropertyType_DataProperty:
            {
                FdoDataPropertyDefinition* data_property = (FdoDataPropertyDefinition*)property;
                column_definition.sde_type = FdoType2SDEType (data_property->GetDataType ());
                type = data_property->GetDataType ();
                switch (type)
                {   // see ArcSDE Configuration and Tuning Guide for yadda yadda
                    // for Oracle:
                    // SE_SMALLINT_TYPE      NUMBER(4,0)
                    // SE_INTEGER_TYPE       NUMBER(10,0)
                    // SE_FLOAT_TYPE         NUMBER(6,2)
                    // SE_DOUBLE_TYPE        NUMBER(15,4)
                    // SE_STRING_TYPE        VARCHAR2(255)
                    // SE_NSTRING_TYPE       NVARCHAR2(255)
                    // SE_BLOB_TYPE          LONG RAW

                    case FdoDataType_Int16:
                        column_definition.size = 6;
                        column_definition.decimal_digits = 1;
                        // NOTE: ArcSDE doesn't allow an SE_FLOAT_TYPE of size 5 and decimal digits 0;
                        //       the closest thing it allows is an SE_FLOAT_TYPE of size 6 and decimal digits 1.
                        break;
                    case FdoDataType_DateTime:
                    case FdoDataType_Int32:
                    case FdoDataType_Decimal:
                        column_definition.size = data_property->GetPrecision ();
                        column_definition.decimal_digits = data_property->GetScale ();
                        break;
                    case FdoDataType_Single:
                        column_definition.size = 38;
                        column_definition.decimal_digits = 8;
                        break;
                    case FdoDataType_Double:
                        column_definition.size = 38;
                        column_definition.decimal_digits = 8;
                        break;
                    case FdoDataType_String:
                    case FdoDataType_BLOB:
                        column_definition.size = data_property->GetLength ();
                        column_definition.decimal_digits = 0;
                        break;
                    case FdoDataType_CLOB:
                    case FdoDataType_Boolean:
                    case FdoDataType_Byte:
                    case FdoDataType_Int64:
                    default:
                        throw FdoException::Create (NlsMsgGet1(ARCSDE_DATATYPE_UNHANDLED, "The FDO DataType %1$d is unsupported.", (int)type));
                }
                column_definition.nulls_allowed = data_property->GetNullable ();
                column_definition.row_id_type = SE_REGISTRATION_ROW_ID_COLUMN_TYPE_NONE;
            }
            break;
        // Represents an Object Property type.
        case FdoPropertyType_ObjectProperty:
            {
                throw FdoException::Create (NlsMsgGet(ARCSDE_OBJECT_PROPERTIES_NOT_SUPPORTED, "Object properties are not supported."));
            }
            break;
        // Represents a Geometric Property type.
        case FdoPropertyType_GeometricProperty:
            {
                // You cannot create a column of type SE_SHAPE_TYPE with this SE_table_create.
                // If you wish to create a shape column, create it as an SE_INTEGER_TYPE
                // and use SE_layer_create to spatially enable it.
                column_definition.sde_type = SE_INTEGER_TYPE;
                column_definition.size = 0;
                column_definition.decimal_digits = 0;
                column_definition.nulls_allowed = true;
            }
            break;
    }
}

LONG ArcSDEApplySchemaCommand::getGeometryMask (FdoGeometricPropertyDefinition* geometry)
{
    FdoInt32 geomTypeCount;
    FdoGeometryType* geomTypes = NULL;
    LONG ret = 0L;

    geomTypes = geometry->GetSpecificGeometryTypes (geomTypeCount);

    ret |= SE_NIL_TYPE_MASK;  // always allow NULL values

    if (FdoCommonMiscUtil::ContainsGeomType(geomTypes, geomTypeCount, FdoGeometryType_Point))
        ret |= (SE_POINT_TYPE_MASK);
    if (FdoCommonMiscUtil::ContainsGeomType(geomTypes, geomTypeCount, FdoGeometryType_MultiPoint))
        ret |= (SE_POINT_TYPE_MASK | SE_MULTIPART_TYPE_MASK);
    if (FdoCommonMiscUtil::ContainsGeomType(geomTypes, geomTypeCount, FdoGeometryType_LineString)
     || FdoCommonMiscUtil::ContainsGeomType(geomTypes, geomTypeCount, FdoGeometryType_CurveString))
        ret |= (SE_LINE_TYPE_MASK | SE_SIMPLE_LINE_TYPE_MASK);
    if (FdoCommonMiscUtil::ContainsGeomType(geomTypes, geomTypeCount, FdoGeometryType_MultiLineString)
     || FdoCommonMiscUtil::ContainsGeomType(geomTypes, geomTypeCount, FdoGeometryType_MultiCurveString))
        ret |= (SE_LINE_TYPE_MASK | SE_SIMPLE_LINE_TYPE_MASK | SE_MULTIPART_TYPE_MASK);
    if (FdoCommonMiscUtil::ContainsGeomType(geomTypes, geomTypeCount, FdoGeometryType_Polygon)
     || FdoCommonMiscUtil::ContainsGeomType(geomTypes, geomTypeCount, FdoGeometryType_CurvePolygon))
        ret |= (SE_AREA_TYPE_MASK);
    if (FdoCommonMiscUtil::ContainsGeomType(geomTypes, geomTypeCount, FdoGeometryType_MultiPolygon)
     || FdoCommonMiscUtil::ContainsGeomType(geomTypes, geomTypeCount, FdoGeometryType_MultiCurvePolygon))
        ret |= (SE_AREA_TYPE_MASK | SE_MULTIPART_TYPE_MASK);
    if (FdoCommonMiscUtil::ContainsGeomType(geomTypes, geomTypeCount, FdoGeometryType_MultiGeometry))
        ret |= (SE_POINT_TYPE_MASK | SE_LINE_TYPE_MASK | SE_SIMPLE_LINE_TYPE_MASK | SE_AREA_TYPE_MASK | SE_MULTIPART_TYPE_MASK);

    return (ret);
}


void ArcSDEApplySchemaCommand::CleanUpIndexes(SE_INDEX_DEF* newIndexes, LONG numNewIndexes)
{
    for (int i = 0; i < numNewIndexes; i++)
    {
        delete[] newIndexes[i].ascending;
        for (int j=0; j<newIndexes[i].num_columns; j++)
            delete[] newIndexes[i].column_name[j];
        delete[] newIndexes[i].column_name;
    }
    delete[] newIndexes;
}


void ArcSDEApplySchemaCommand::add_class (FdoFeatureSchema* schema, FdoClassDefinition* cls)
{
    CHAR tableName[SE_QUALIFIED_TABLE_NAME];
    int column_count; // number of columns created before adding a layer
    SE_COLUMN_DEF* column_definitions = NULL;
    SE_METADATAINFO* metadata_records = NULL;
    CHAR* schema_name = NULL;
    CHAR* schema_description = NULL;
    SE_LAYERINFO layer;
    SE_REGINFO registration;
    LONG result = 0L;
    std::vector<sde_std_string> columnNames;

    FdoPtr<ArcSDEConnection> connection = static_cast<ArcSDEConnection*>(GetConnection ());
    FdoPtr<FdoPropertyDefinitionCollection> properties = cls->GetProperties ();
    if (FdoClassType_FeatureClass == cls->GetClassType ())
    {
        FdoPtr<FdoDataPropertyDefinitionCollection> identities = cls->GetIdentityProperties ();
        if (1 < identities->GetCount ())
            throw FdoException::Create (NlsMsgGet(ARCSDE_SCHEMA_MULTIPLE_ID_PROPERTIES, "ArcSDE Provider does not support classes with more than one identity property."));
        else if (0 == identities->GetCount ())
            throw FdoException::Create (NlsMsgGet(ARCSDE_SCHEMA_FEATURECLASS_NO_ID_PROPERTIES, "A feature class cannot have zero identity properties."));
        else
        {
            // check for read-only not-null id property
            FdoPtr<FdoPropertyDefinition> property = identities->GetItem (0);
            if (FdoPropertyType_DataProperty != property->GetPropertyType ())
                throw FdoAutogenerationException::Create (NlsMsgGet(ARCSDE_SCHEMA_UNSUPPORTED_ID_TYPE, "Only data properties are supported as identity properties."));
            FdoDataPropertyDefinition* id = (FdoDataPropertyDefinition*)property.p;
            if (id->GetNullable ())
                throw FdoException::Create (NlsMsgGet(ARCSDE_SCHEMA_UNSUPPORTED_ID_NULLABLE, "Identity properties cannot be nullable."));
            if (id->GetIsAutoGenerated ())
                id->SetReadOnly (true);
        }
    }

    // Initialize empty SE_COLUMN_DEF's:
    column_definitions = new SE_COLUMN_DEF[properties->GetCount ()];
    if (NULL == column_definitions)
        handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_MEMORY_FAILURE, "Failed to allocate memory.");

    // Initialize empty SE_METADATAINFO records:
    long lMaximumMetadataCount = (properties->GetCount () * 5) /*maximum metadata entries per FDO property*/
        + 3 /* metadata entries for one FDO class*/;
    long lActualMetadataCount = 0;
    metadata_records = new SE_METADATAINFO[lMaximumMetadataCount];
    for (int i = 0; i < lMaximumMetadataCount; i++)
    {
        result = SE_metadatainfo_create (&metadata_records[i]);
        handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_METADATA_CREATE_FAILED, "Failed to create ArcSDE metadata.");
    }

    // Two attempts to create table; first attempt is with 'non-strict' name conversion, second attempt is with 'strict' name conversion:
    for (int iCreationAttempt=1; iCreationAttempt<=2; iCreationAttempt++)
    {
        lActualMetadataCount = 0;

        // Convert FDO classname to ArcSDE tablename:
        bool bStrict = (iCreationAttempt==2);
        generateUniqueTableName (cls, bStrict,tableName);
        generateUniqueColumnNames(properties, bStrict, columnNames);

        // Add table metadata:
        SetArcSDEMetadata(metadata_records[lActualMetadataCount++], METADATA_CN_CLASSSCHEMA, sde_pcwc2us(_TXT("")), schema->GetName());
        SetArcSDEMetadata(metadata_records[lActualMetadataCount++], METADATA_CN_CLASSNAME, sde_pcwc2us(_TXT("")), cls->GetName());
        SetArcSDEMetadata(metadata_records[lActualMetadataCount++], METADATA_CN_CLASSDESC, sde_pcwc2us(_TXT("")), cls->GetDescription());

        // Add column metadata & populate column definitions:
        column_count = 0;
        for (int i = 0; i < properties->GetCount (); i++)
        {
            FdoPtr<FdoPropertyDefinition> property = properties->GetItem (i);
            if (FdoPropertyType_GeometricProperty != property->GetPropertyType ())
            {
                sde_strcpy(sde_pus2wc(column_definitions[column_count].column_name), columnNames.at(i).c_str());
                getDataType (property, column_definitions[column_count]);

                // add column metadata:
                SetArcSDEMetadata(metadata_records[lActualMetadataCount++], METADATA_CN_PROPNAME, column_definitions[column_count].column_name, property->GetName());
                SetArcSDEMetadata(metadata_records[lActualMetadataCount++], METADATA_CN_PROPDESC, column_definitions[column_count].column_name, property->GetDescription());
                if (property->GetPropertyType() == FdoPropertyType_DataProperty)
                {
                    FdoDataPropertyDefinition* dataProp = dynamic_cast<FdoDataPropertyDefinition*>(property.p);

                    // Set data type override, if necessary:
                    const CHAR* dataTypeOverride = FDOTypeToMetadataValue(dataProp->GetDataType());
                    if (NULL != dataTypeOverride)
                        SetArcSDEMetadata(metadata_records[lActualMetadataCount++], METADATA_CN_PROPTYPE, column_definitions[column_count].column_name, dataTypeOverride);

                    // Set read-only override, if necessary:
                    if (dataProp->GetReadOnly())
                        SetArcSDEMetadata(metadata_records[lActualMetadataCount++], METADATA_CN_PROPREADONLY, column_definitions[column_count].column_name, METADATA_V_BOOL_YES);

                    // Set default-value override, if necessary:
                    FdoString *defaultValue = dataProp->GetDefaultValue();
                    if ((defaultValue!=NULL) && (wcslen(defaultValue)>0))
                        SetArcSDEMetadata(metadata_records[lActualMetadataCount++], METADATA_CN_PROPDEFAULTVALUE, column_definitions[column_count].column_name, defaultValue);
                }
                column_count++;
            }
        }

        // Create table:
        result = SE_table_create (connection->GetConnection (), tableName, column_count, column_definitions, sde_pcwc2us(_TXT("")));
        if (result == SE_SUCCESS)
            break;  // exit for loop
        else if (bStrict)
            handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_SCHEMA_ADD_CLASS_FAILED, "Failed to add class '%1$ls'.", cls->GetName());
    }

    // Get table's registration info:
    result = SE_reginfo_create (&registration);
    handle_sde_err<FdoSchemaException> (connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_CREATE, "Table registration info could not be created.");
    result = SE_registration_get_info (connection->GetConnection (), tableName, registration);
    handle_sde_err<FdoSchemaException> (connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_GET, "Table registration info could not be retrieved.");

    // Set the rowid column, if necessary:
    FdoPtr<FdoDataPropertyDefinitionCollection> identities = cls->GetIdentityProperties ();
    FdoPtr<FdoDataPropertyDefinition> identityProperty;
    if (identities->GetCount() > 0)
    {
        if (identities->GetCount() > 1)
            throw FdoException::Create (NlsMsgGet(ARCSDE_SCHEMA_MULTIPLE_ID_PROPERTIES, "ArcSDE Provider does not support classes with more than one identity property."));

        // Get the identity property and its column name:
        identityProperty = identities->GetItem(0);
        CHAR *identityColumnName = NULL;
        for (int i = 0; i < properties->GetCount (); i++)
        {
            FdoPtr<FdoPropertyDefinition> property = properties->GetItem (i);
            if (0==wcscmp(property->GetName(), identityProperty->GetName()))
            {
                identityColumnName = column_definitions[i].column_name;
                break;
            }
        }

        // Set registration info's rowid:
        if (identityProperty->GetIsAutoGenerated())
        {
            // Validate data type:
            if (identityProperty->GetDataType() != FdoDataType_Int32)
                throw FdoException::Create (NlsMsgGet(ARCSDE_SCHEMA_WRONG_AUTOGENERATED_ID_TYPE, "ArcSDE Provider only supports auto-generated identity properties of type Int32."));

            result = SE_reginfo_set_rowid_column (registration, identityColumnName, SE_REGISTRATION_ROW_ID_COLUMN_TYPE_SDE);
            handle_sde_err<FdoSchemaException> (connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_SET_ITEM, "Table registration info item '%1$ls' could not be set.", L"rowid_column");
        }
        else
        {
            result = SE_reginfo_set_rowid_column (registration, identityColumnName, SE_REGISTRATION_ROW_ID_COLUMN_TYPE_USER);
            handle_sde_err<FdoSchemaException> (connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_SET_ITEM, "Table registration info item '%1$ls' could not be set.", L"rowid_column");

            // Defect 634241 "no error when inserting duplicate identity value in table with user-managed id column";
            // ArcSDE doesn't enforce uniqueness of user-managed id columns, so we add a 'unique index' here:
            FdoPtr<FdoUniqueConstraintCollection> uniqueConstraints = cls->GetUniqueConstraints();
            FdoPtr<FdoUniqueConstraint> uniqueConstraint = FdoUniqueConstraint::Create();
            FdoPtr<FdoDataPropertyDefinitionCollection> dataPropDefs = uniqueConstraint->GetProperties();
            dataPropDefs->Add(identityProperty);
            uniqueConstraints->Add(uniqueConstraint);
        }
    }

    // Set rowlocking:
    FdoPtr<FdoClassCapabilities> capabilities = cls->GetCapabilities();
    if ((capabilities!=NULL) && capabilities->SupportsLocking())
    {
        if (capabilities->SupportsLongTransactions())
            throw FdoException::Create(NlsMsgGet1(ARCSDE_SCHEMA_LOCKING_AND_VERSIONNING, "The class '%1$ls' has both versionning and locking enabled; this is not supported by ArcSDE.", cls->GetName()));

        if ((identityProperty != NULL) && (identityProperty->GetIsAutoGenerated()))
        {
            result = SE_reginfo_set_rowlocks(registration, TRUE);
            handle_sde_err<FdoSchemaException> (connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_SET_ITEM, "Table registration info item '%1$ls' could not be set.", L"rowlocks");
        }
        else
            throw FdoException::Create(NlsMsgGet1(ARCSDE_SCHEMA_LOCKING_INCORRECT_IDENTITY, "The class '%1$ls' has locking enabled but no autogenerated identity property; this is not supported by ArcSDE.", cls->GetName()));
    }
    else
    {
        result = SE_reginfo_set_rowlocks(registration, FALSE);
        handle_sde_err<FdoSchemaException> (connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_SET_ITEM, "Table registration info item '%1$ls' could not be set.", L"rowlocks");
    }

    // Set versionning:
    if ((capabilities!=NULL) && capabilities->SupportsLongTransactions())
    {
        if ((identityProperty != NULL) && (identityProperty->GetIsAutoGenerated()))
        {
            result = SE_reginfo_set_multiversion(registration, TRUE);
            handle_sde_err<FdoSchemaException> (connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_SET_ITEM, "Table registration info item '%1$ls' could not be set.", L"multiversion");
        }
        else
            throw FdoException::Create(NlsMsgGet1(ARCSDE_SCHEMA_VERSIONNING_INCORRECT_IDENTITY, "The class '%1$ls' has versionning enabled but no autogenerated identity property; this is not supported by ArcSDE.", cls->GetName()));
    }
    else
    {
        result = SE_reginfo_set_multiversion(registration, FALSE);
        handle_sde_err<FdoSchemaException> (connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_SET_ITEM, "Table registration info item '%1$ls' could not be set.", L"multiversion");
    }

    // Update registration info:
    result = SE_registration_alter (connection->GetConnection (), registration);
    handle_sde_err<FdoSchemaException> (connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_REGISTRATION_INFO_ALTER, "Registration info could not be altered.");
    SE_reginfo_free (registration);

    // add layer if necessary
    for (int i = 0; i < properties->GetCount (); i++)
    {
        FdoPtr<FdoPropertyDefinition> property = properties->GetItem (i);
        if (FdoPropertyType_GeometricProperty == property->GetPropertyType ())
        {
            FdoPtr<FdoGeometricPropertyDefinition> geometry = static_cast<FdoGeometricPropertyDefinition*>(properties->GetItem (i));

            // add column metadata:
            sde_strcpy(sde_pus2wc(column_definitions[column_count].column_name), columnNames.at(i).c_str());
            SetArcSDEMetadata(metadata_records[lActualMetadataCount++], METADATA_CN_PROPNAME, column_definitions[column_count].column_name, property->GetName());
            SetArcSDEMetadata(metadata_records[lActualMetadataCount++], METADATA_CN_PROPDESC, column_definitions[column_count].column_name, property->GetDescription());

            // Determine which spatial context to use for this geometry propety:
            FdoString *spatialContext = geometry->GetSpatialContextAssociation();
            if ((spatialContext == NULL) || (wcslen(spatialContext) == 0))
                spatialContext = mConnection->GetActiveSpatialContext();

            // Get the active coordinate reference system:
            // NOTE: SpatialContextNameToSRID() will throw an exception if the given spatial context is null/invalid:
            LONG lSRID = ArcSDESpatialContextUtility::SpatialContextNameToSRID(mConnection, spatialContext);
            SE_SPATIALREFINFO spatialrefinfo;
            result = SE_spatialrefinfo_create(&spatialrefinfo);
            handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_RETRIEVE_COORDSYS_FAILED, "Error while retrieving active coordinate reference system.");
            result = SE_spatialref_get_info(connection->GetConnection(), lSRID, spatialrefinfo);
            handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_RETRIEVE_COORDSYS_FAILED, "Error while retrieving active coordinate reference system.");
            SE_COORDREF coordref;
            result = SE_coordref_create (&coordref);
            handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_RETRIEVE_COORDSYS_FAILED, "Error while retrieving active coordinate reference system.");
            result = SE_spatialrefinfo_get_coordref(spatialrefinfo, coordref);
            handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_RETRIEVE_COORDSYS_FAILED, "Error while retrieving active coordinate reference system.");

            // get the "number of system units per x,y unit"
            LFLOAT falsex;
            LFLOAT falsey;
            LFLOAT xyunits;
            result = SE_coordref_get_xy (coordref, &falsex, &falsey, &xyunits);
            handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_RETRIEVE_COORDSYS_FAILED, "Error while retrieving active coordinate reference system.");

            // Set the coordinate reference system of the layer:
            result = SE_layerinfo_create (coordref, &layer);
            SE_coordref_free (coordref);
            handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_CREATE_OBJECT_FAILED, "Failed to create ArcSDE object '%1$ls'.", L"SE_LAYERINFO");


            // NOTE: we must set the grid size in order to get a spatial index,
            //       otherwise SE_stream_set_spatial_constraints fails with SE_NO_SPATIAL_INDEX (Spatial index does not exist)
            //       but proper grid size depends on underlying coordinate reference system,
            //       and user preferences, so...
            // TODO: calculate a valid set of grid values (based on the coordref envelope?)
            // TODO: allow user to specify spatial grid size?  What about other settings via SE_layerinfo_set_*() ?
            LFLOAT grid = 250000.0 / xyunits; // grid sizes must be >250 integer system units (converted with SE_COORDREF)
            if ( grid < 250 )
                grid = 250;

            result = SE_layerinfo_set_grid_sizes (layer, grid, 0.0, 0.0);
            handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_SCHEMA_ADD_LAYER_FAILED, "Failed to create layer (spatial column) for class '%1$ls'.", cls->GetName());

            // Set measure and elevation:
            result = SE_layerinfo_set_3D (layer, geometry->GetHasElevation() ? TRUE : FALSE);
            handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_SCHEMA_ADD_LAYER_FAILED, "Failed to create layer (spatial column) for class '%1$ls'.", cls->GetName());
            result = SE_layerinfo_set_measured (layer, geometry->GetHasMeasure() ? TRUE : FALSE);
            handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_SCHEMA_ADD_LAYER_FAILED, "Failed to create layer (spatial column) for class '%1$ls'.", cls->GetName());

            // Set other important stuff:
            result = SE_layerinfo_set_spatial_column (layer, tableName, column_definitions[column_count].column_name);
            handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_SCHEMA_ADD_LAYER_FAILED, "Failed to create layer (spatial column) for class '%1$ls'.", cls->GetName());
            result = SE_layerinfo_set_creation_keyword (layer, sde_pcwc2us(_TXT("")));
            handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_SCHEMA_ADD_LAYER_FAILED, "Failed to create layer (spatial column) for class '%1$ls'.", cls->GetName());
            result = SE_layerinfo_set_shape_types (layer, getGeometryMask (geometry));
            handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_SCHEMA_ADD_LAYER_FAILED, "Failed to create layer (spatial column) for class '%1$ls'.", cls->GetName());

            result = SE_layer_create (connection->GetConnection (), layer, 0, 0);
            handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_SCHEMA_ADD_LAYER_FAILED, "Failed to create layer (spatial column) for class '%1$ls'.", cls->GetName());

            // check for spatial index
            result = SE_layer_get_info (connection->GetConnection (), tableName, column_definitions[column_count].column_name, layer);
            handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_SCHEMA_ADD_LAYER_FAILED, "Failed to create layer (spatial column) for class '%1$ls'.", cls->GetName());
            if (!SE_layerinfo_has_spatial_index (layer))
                throw FdoSchemaException::Create (NlsMsgGet1(ARCSDE_SCHEMA_MISSING_SPATIAL_INDEX, "Missing spatial index on layer (spatial column) for class '%1$ls'.", cls->GetName()));

            SE_layerinfo_free (layer);

            break; // There is at most one spatial column, so we can exit early from the loop
            //TODO: validate this code
        }
    }

    // add the meta data to the table:
    result = SE_table_metadata_add (connection->GetConnection (), tableName, lActualMetadataCount, metadata_records);
    handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_METADATA_ADD_FAILED, "Failed to add ArcSDE metadata.");

    // Add unique indexes, if any are specified via the constraints API:
    SHORT numNewIndexes = 0;
    SE_INDEX_DEF *newIndexes = NULL;
    FdoPtr<FdoUniqueConstraintCollection> uniqueConstraints = cls->GetUniqueConstraints();
    if (uniqueConstraints->GetCount() > 0)
    {
        int iIndexNameCounter = 1;
        do
        {
            // Grab existing index info, to avoid index name conflicts:
            SHORT numExistingIndexes = 0;
            SE_INDEX_DEF *existingIndexes = NULL;
            result = SE_table_describe_indexes(connection->GetConnection(), tableName, &numExistingIndexes, &existingIndexes);
            handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_UNEXPECTED_ERROR, "Unexpected error encountered in ArcSDE Provider.");

            // Create new indexes:
            numNewIndexes = uniqueConstraints->GetCount();
            newIndexes = new SE_INDEX_DEF[numNewIndexes];
            for (int i=0; i<uniqueConstraints->GetCount(); i++)
            {
                FdoPtr<FdoUniqueConstraint> uniqueConstraint = uniqueConstraints->GetItem(i);
                FdoPtr<FdoDataPropertyDefinitionCollection> dataPropDefs = uniqueConstraint->GetProperties();
                newIndexes[i].unique = TRUE;
                newIndexes[i].num_columns = dataPropDefs->GetCount();
                newIndexes[i].ascending = new BOOL[newIndexes[i].num_columns];
                newIndexes[i].column_name = new CHAR*[newIndexes[i].num_columns];
                for (int j=0; j<dataPropDefs->GetCount(); j++)
                {
                    FdoPtr<FdoDataPropertyDefinition> dataPropDef = dataPropDefs->GetItem(j);
                    FdoPtr<FdoIdentifier> dataPropDefId = FdoIdentifier::Create(dataPropDef->GetName());
                    newIndexes[i].column_name[j] = new CHAR[SE_QUALIFIED_COLUMN_LEN+1];
                    connection->PropertyToColumn(newIndexes[i].column_name[j], cls, dataPropDefId);
                    //newIndexes[i].ascending[j] = FALSE;
                    newIndexes[i].ascending[j] = TRUE;
                }

                // Figure out the new unique index name:
                bool bUniqueIndexNameFound = false;
                while (!bUniqueIndexNameFound)
                {
                    sde_sprintf(sde_pus2wc(newIndexes[i].index_name), SE_MAX_INDEX_LEN, _TXT("ARCSDE_UNIQUE_%d"), iIndexNameCounter);

                    iIndexNameCounter++;
                    bUniqueIndexNameFound = true;
                    for (int k=0; k<numExistingIndexes && bUniqueIndexNameFound; k++)
                        if (0==sde_stricmp(sde_pus2wc(existingIndexes[k].index_name), sde_pus2wc(newIndexes[i].index_name)))
                            bUniqueIndexNameFound = false;
                }
            }
            SE_table_free_index_descriptions(numExistingIndexes, existingIndexes);
            result = SE_table_create_indexes(connection->GetConnection(), tableName, numNewIndexes, newIndexes, sde_pcwc2us(_TXT("DEFAULTS")));

            if (result == SE_INDEX_EXISTS)
                CleanUpIndexes(newIndexes, numNewIndexes);
        }
        while (result == SE_INDEX_EXISTS);

        handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_SCHEMA_UNIQUE_INDEX_CREATION_FAILED, "Failed to create unique index on class '%1$ls'.", cls->GetName());
    }

    // Clean up:
    for (int i = 0; i < lMaximumMetadataCount; i++)
        SE_metadatainfo_free (metadata_records[i]);
    delete[] metadata_records;
    delete[] column_definitions;
    if (numNewIndexes > 0)
        CleanUpIndexes(newIndexes, numNewIndexes);
}

void ArcSDEApplySchemaCommand::delete_class (FdoFeatureSchema* schema, FdoClassDefinition* cls)
{
    CHAR table[SE_QUALIFIED_TABLE_NAME];
    LONG result = 0L;

    FdoPtr<ArcSDEConnection> connection = static_cast<ArcSDEConnection*>(GetConnection ());
    connection->ClassToTable(table, cls);
    result = SE_table_delete (connection->GetConnection (), table);
    handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_SCHEMA_DELETE_CLASS_FAILED, "Failed to delete class '%1$ls'.", cls->GetName());
}

void ArcSDEApplySchemaCommand::modify_class (FdoFeatureSchema* schema, FdoClassDefinition* cls)
{
    //TODO: can we support this, perhaps only if the class is empty?
    throw FdoException::Create (NlsMsgGet(ARCSDE_SCHEMA_MODIFY_CLASS_UNSUPPORTED, "ArcSDE Provider does not support modifying a class definition."));
}

void ArcSDEApplySchemaCommand::add_schema (FdoFeatureSchema* schema)
{
    FdoPtr<FdoClassCollection> classes = mSchema->GetClasses ();
    for (int i = 0, size = classes->GetCount (); i < size; i++)
    {
        FdoClassDefinition* cls = classes->GetItem (i);
        switch (cls->GetElementState ())
        {
            // Indicates the schema element has been added.
            case FdoSchemaElementState_Added:
                add_class (mSchema, cls);
                break;

            // Indicates the schema element is marked for deletion.
            case FdoSchemaElementState_Deleted:
                // if we're adding a schema, complain about deleted classes
                throw FdoException::Create (L"Deleted class in added schema");
                break;

            // Indicates the schema element is not associated with an FdoFeatureSchema.
            case FdoSchemaElementState_Detached:
                // duh, waddo I do
                break;

            // Indicates the schema element has been modified in some way.
            case FdoSchemaElementState_Modified:
                // if we're adding a schema, complain about modified classes
                throw FdoException::Create (NlsMsgGet(ARCSDE_SCHEMA_DELETED_CLASS_IN_ADDED_SCHEMA, "Conflicting schema element state: a class marked for deletion was found in a schema to be added."));
                break;

            // Indicates the schema element has not been changed.
            case FdoSchemaElementState_Unchanged:
                // if we're adding a schema, complain about unchcanged classes
                throw FdoException::Create (NlsMsgGet(ARCSDE_SCHEMA_UNCHANGED_CLASS_IN_ADDED_SCHEMA, "Conflicting schema element state: a class marked unchanged was found in a schema to be added."));
                break;

            default:
                throw FdoException::Create (NlsMsgGet1(ARCSDE_SCHEMA_UNSUPPORTED_ELEMENT_STATE, "Schema element state '%1$d' is not supported.", (int)cls->GetElementState()));
        }
        cls->Release ();
    }
}

void ArcSDEApplySchemaCommand::modify_schema (FdoFeatureSchema* schema)
{
    /*todo: check for modified schema description or attributes and update all tables.*/
    /*todo: how to determine what's changed, i.e. this is protected: schema->m_descriptionCHANGED*/
    FdoPtr<FdoClassCollection> classes = mSchema->GetClasses ();
    for (int i = 0, size = classes->GetCount (); i < size; i++)
    {
        FdoClassDefinition* cls = classes->GetItem (i);
        switch (cls->GetElementState ())
        {
            // Indicates the schema element has been added.
            case FdoSchemaElementState_Added:
                add_class (mSchema, cls);
                break;

            // Indicates the schema element is marked for deletion.
            case FdoSchemaElementState_Deleted:
                delete_class (mSchema, cls); /*todo: at the moment, deleting the last class deletes the schema */
                break;

            // Indicates the schema element is not associated with an FdoFeatureSchema.
            case FdoSchemaElementState_Detached:
                // duh, waddo I do
                break;

            // Indicates the schema element has been modified in some way.
            case FdoSchemaElementState_Modified:
                modify_class (mSchema, cls);
                break;

            // Indicates the schema element has not been changed.
            case FdoSchemaElementState_Unchanged:
                break;

            default:
                throw FdoException::Create (NlsMsgGet1(ARCSDE_SCHEMA_UNSUPPORTED_ELEMENT_STATE, "Schema element state '%1$d' is not supported.", (int)cls->GetElementState()));
        }
        cls->Release ();
    }
}

void ArcSDEApplySchemaCommand::delete_schema (FdoFeatureSchema* schema)
{
    FdoPtr<FdoClassCollection> classes = mSchema->GetClasses ();
    for (int i = 0, size = classes->GetCount (); i < size; i++)
    {
        FdoClassDefinition* cls = classes->GetItem (i);
        /*todo: check for class used elsewhere? */
        delete_class (mSchema, cls);
        cls->Release ();
    }
}

///<summary>Executes the ApplySchema command that creates metadata and physical
///storage for the schema. An exception is thrown if the schema already
///exists or if a schema already exists and the feature provider only
///supports a single schema. If schema changes include deletion of classes
///or the schema itself, then those classes (or the schema, if it is marked
///as deleted) must not contain any instance data. If they do, an exception
///will be thrown.</summary>
/// <returns>Returns nothing</returns> 
void ArcSDEApplySchemaCommand::Execute ()
{
    if (NULL == mSchema)
        throw FdoException::Create (NlsMsgGet(ARCSDE_SCHEMA_NOT_SPECIFIED, "No schema specified for the apply schema command."));

    switch (mSchema->GetElementState ())
    {
        // Indicates the schema element has been added.
        case FdoSchemaElementState_Added:
            add_schema (mSchema);
            break;

        // Indicates the schema element is marked for deletion.
        case FdoSchemaElementState_Deleted:
            delete_schema (mSchema);
            break;

        // Indicates the schema element is not associated with an FdoFeatureSchema.
        case FdoSchemaElementState_Detached:
            // duh, waddo I do
            break;

        // Indicates the schema element has been modified in some way.
        case FdoSchemaElementState_Modified:
            modify_schema (mSchema);
            break;

        // Indicates the schema element has not been changed.
        case FdoSchemaElementState_Unchanged:
            break;

        default:
            throw FdoException::Create (NlsMsgGet1(ARCSDE_SCHEMA_UNSUPPORTED_ELEMENT_STATE, "Schema element state '%1$d' is not supported.", (int)mSchema->GetElementState()));
    }
    mSchema->AcceptChanges ();

    // Clear the schema cache, to force reload during next DescribeSchema:
    FdoPtr<ArcSDEConnection> connection = static_cast<ArcSDEConnection*>(GetConnection ());
    connection->DecacheSchema();
}




void ArcSDEApplySchemaCommand::SetArcSDEMetadata(SE_METADATAINFO &metadata, const CHAR* classname, const CHAR* property, const CHAR* value, const CHAR* description)
{
    LONG result = 0L;

    if (NULL != classname)
    {
        result = SE_metadatainfo_set_classname(metadata, classname);
        handle_sde_err<FdoSchemaException>(result, __FILE__, __LINE__, ARCSDE_METADATA_MANIPULATE_FAILED, "Failed to manipulate ArcSDE metadata.");
    }

    if (NULL != property)
    {
        result = SE_metadatainfo_set_property (metadata, property);
        handle_sde_err<FdoSchemaException>(result, __FILE__, __LINE__, ARCSDE_METADATA_MANIPULATE_FAILED, "Failed to manipulate ArcSDE metadata.");
    }

    if (NULL != value)
    {
        result = SE_metadatainfo_set_value (metadata, value);
        handle_sde_err<FdoSchemaException>(result, __FILE__, __LINE__, ARCSDE_METADATA_MANIPULATE_FAILED, "Failed to manipulate ArcSDE metadata.");
    }

    if (NULL != description)
    {
        result = SE_metadatainfo_set_description (metadata, description);
        handle_sde_err<FdoSchemaException>(result, __FILE__, __LINE__, ARCSDE_METADATA_MANIPULATE_FAILED, "Failed to manipulate ArcSDE metadata.");
    }
}


void ArcSDEApplySchemaCommand::SetArcSDEMetadata(SE_METADATAINFO &metadata, const CHAR* classname, const CHAR* property, const wchar_t* value, const CHAR* description)
{
    CHAR* valueMultibyte = NULL;
    sde_wide_to_multibyte (valueMultibyte, value);
    SetArcSDEMetadata(metadata, classname, property, valueMultibyte, description);
}



const CHAR* ArcSDEApplySchemaCommand::FDOTypeToMetadataValue(FdoDataType dataType)
{
    const CHAR* retVal = NULL;

    if (FdoDataType_Boolean == dataType)
        retVal = METADATA_V_DATATYPE_BOOLEAN;
    else if (FdoDataType_Byte == dataType)
        retVal = METADATA_V_DATATYPE_BYTE;
    else if (FdoDataType_Int16 == dataType)
        retVal = METADATA_V_DATATYPE_INT16;
    else if (FdoDataType_Int64 == dataType)
        retVal = METADATA_V_DATATYPE_INT64;
    else if (FdoDataType_Single == dataType)
        retVal = METADATA_V_DATATYPE_SINGLE;
    else if (FdoDataType_Double == dataType)
        retVal = METADATA_V_DATATYPE_DOUBLE;

    return retVal;
}



