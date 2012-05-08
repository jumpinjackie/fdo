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
#include "ArcSDEProvider.h"

ArcSDEDescribeSchemaCommand::ArcSDEDescribeSchemaCommand (FdoIConnection *connection, FdoIdentifier* fdoClassIdToLoad) :
    ArcSDECommand<FdoIDescribeSchema> (connection),
    mSchemaName (NULL)
{
	mClassNames = FdoStringCollection::Create();

	// Keep compatible with previous construct. 
	if (fdoClassIdToLoad != NULL)
	{
		SetSchemaName(fdoClassIdToLoad->GetSchemaName());
		mClassNames->Add(fdoClassIdToLoad->GetName());
	}
}

/** Do not implement the copy constructor. **/
//ArcSDEDescribeSchemaCommand::ArcSDEDescribeSchemaCommand (const ArcSDEDescribeSchemaCommand &right) { }

ArcSDEDescribeSchemaCommand::~ArcSDEDescribeSchemaCommand (void)
{
    if (NULL != mSchemaName)
        delete [] mSchemaName;
}

/** Do not implement the assignment operator. **/
//ArcSDEDescribeSchemaCommand & operator= (const ArcSDEDescribeSchemaCommand &right);

/// <summary>Gets the name of the schema to describe. This function is optional;
/// if not specified, execution of the command will describe all schemas.</summary>
/// <returns>Returns the schema name</returns> 
const wchar_t* ArcSDEDescribeSchemaCommand::GetSchemaName ()
{
    return (mSchemaName);
}


/// <summary>Sets the name of the schema to describe. This function is optional; if not
/// specified, execution of the command will describe all schemas.</summary>
/// <param name="value">Input the schema name</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEDescribeSchemaCommand::SetSchemaName (const wchar_t* value)
{
    if (NULL != mSchemaName)
    {
        delete [] mSchemaName;
        mSchemaName = NULL;
    }
 
    if (value != NULL)
    {
        mSchemaName = new wchar_t[wcslen(value) + 1];
        if (NULL != mSchemaName)
            wcscpy (mSchemaName, value);
    }
}

/// <summary>Gets the names of the classes to retrieve. This is optional,
/// if not specified execution of the command will describe all classes.
/// If the class name is not qualified, and the schema name is not specified,
/// the requested class from all schemas will be described.
/// The class names specified serve only as a hint.  Use of the hint
/// during command execution is provider dependent.  Providers that 
/// will not use the hint will describe the schema for all classes.</summary>
/// <returns>Returns the collection of class names</returns>
FdoStringCollection* ArcSDEDescribeSchemaCommand::GetClassNames()
{
    return FDO_SAFE_ADDREF(mClassNames.p);
}

/// <summary>Sets the name of the classes to retrieve. This is optional, if not
/// specified execution of the command will describe all classes.
/// If the class name is not qualified, and the schema name is not specified,
/// the requested class from all schemas will be described.
/// The class names specified serve only as a hint.  Use of the hint
/// during command execution is provider dependent.  Providers that 
/// will not use the hint will describe the schema for all classes.</summary>
/// <param name="value">Input the collection of class names</parm>
/// <returns>Returns nothing</returns>
void ArcSDEDescribeSchemaCommand::SetClassNames(FdoStringCollection* value)
{
	mClassNames = FDO_SAFE_ADDREF(value);
}

FdoDataType ArcSDEDescribeSchemaCommand::MetadataValueToFDOType(CHAR* value)
{
    FdoDataType retVal = (FdoDataType)-1L;
    if (0==sde_stricmp(sde_pcus2wc(value), sde_pcus2wc(METADATA_V_DATATYPE_BOOLEAN)))
        retVal = FdoDataType_Boolean;
    else if (0==sde_stricmp(sde_pcus2wc(value), sde_pcus2wc(METADATA_V_DATATYPE_BYTE)))
        retVal = FdoDataType_Byte;
    else if (0==sde_stricmp(sde_pcus2wc(value), sde_pcus2wc(METADATA_V_DATATYPE_INT16)))
        retVal = FdoDataType_Int16;
    else if (0==sde_stricmp(sde_pcus2wc(value), sde_pcus2wc(METADATA_V_DATATYPE_INT64)))
        retVal = FdoDataType_Int64;
    else if (0==sde_stricmp(sde_pcus2wc(value), sde_pcus2wc(METADATA_V_DATATYPE_SINGLE)))
        retVal = FdoDataType_Single;
    else if (0==sde_stricmp(sde_pcus2wc(value), sde_pcus2wc(METADATA_V_DATATYPE_DOUBLE)))
        retVal = FdoDataType_Double;
    else
        throw FdoSchemaException::Create(NlsMsgGet(ARCSDE_UNSUPPORTED_METADATA_VALUE, "Unsupported metadata value encountered."));

    return retVal;
}

bool ArcSDEDescribeSchemaCommand::MetadataValueToBoolean(CHAR* value)
{
    bool retVal = false;

    if (0==sde_stricmp(sde_pcus2wc(value), sde_pcus2wc(METADATA_V_BOOL_YES)))
        retVal = true;
    else if (0==sde_stricmp(sde_pcus2wc(value), sde_pcus2wc(METADATA_V_BOOL_NO)))
        retVal = false;
    else
        throw FdoSchemaException::Create(NlsMsgGet(ARCSDE_UNSUPPORTED_METADATA_VALUE, "Unsupported metadata value encountered."));

    return retVal;
}

void ArcSDEDescribeSchemaCommand::addClass (ArcSDEConnection* connection, FdoFeatureSchema* schema, wchar_t* fdoClassName, wchar_t* fdoClassDesc,
                                            CHAR* qualified_table_name, SE_REGINFO registration, SE_METADATAINFO *metadata_list, LONG num_records)
{
    LONG result = 0L;
    SHORT column_count = 0;
    SE_COLUMN_DEF *columns = NULL;
    bool has_geometry = false;
    FdoStringP fdoPropertyName;
    FdoStringP property_description;
#ifdef _DEBUG
    CHAR classname[SE_MAX_METADATA_CLASS_LEN];
    CHAR description[SE_MAX_DESCRIPTION_LEN];
    CHAR property[SE_MAX_METADATA_PROPERTY_LEN];
    CHAR value[SE_MAX_METADATA_VALUE_LEN];
    CHAR metadata_qual_table[SE_MAX_OBJECT_NAME_LEN];
#endif
    FdoPtr<FdoPropertyDefinition> property_definiton;
    FdoPtr<FdoDataPropertyDefinition> data_definition;
    FdoPtr<FdoGeometricPropertyDefinition> geometry_definition;
    FdoDataType dataType = (FdoDataType)-1L;
    FdoPtr<FdoDataPropertyDefinitionCollection> identity_properties;
    FdoPtr<FdoPropertyDefinitionCollection> properties;
    SE_LAYERINFO info;
    LONG shapes = 0L;
    FdoPtr<FdoClassDefinition> newFdoClass;
    FdoPtr<FdoClassCollection> classes;

    // Get ArcSDE table's columns:
    result = SE_table_describe (connection->GetConnection (), qualified_table_name, &column_count, &columns);
    // SPR 613177.01 ArcSDE provider does not exit gracefully if one of the tables is deleted linked
    // simply skip classes where no table exists (or any other error occurs).
    if ((result != SE_SUCCESS) && (result != SE_FINISHED) )
        return;
    handle_sde_err<FdoSchemaException>(connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_UNEXPECTED_ERROR, "Unexpected error encountered in ArcSDE Provider.");


    // Find if there is a shape column:
    has_geometry = false;
    for (int i = 0; i < column_count; i++)
    {
        if (SE_SHAPE_TYPE == columns[i].sde_type)
        {
            has_geometry = true;
            break;
        }
    }

    // Create appropriate FDO class type (feature VS plain class):
    //TODO: handle metadata METADATA_CN_CLASSTYPE here
    if (has_geometry)
        newFdoClass = FdoFeatureClass::Create (fdoClassName, fdoClassDesc);
    else
        newFdoClass = FdoClass::Create (fdoClassName, fdoClassDesc);

    // Update class capabilities for this class:
    FdoPtr<FdoClassCapabilities> classCapabilities = FdoClassCapabilities::Create(*newFdoClass.p);
    bool bClassSupportsLocking = 0 != SE_reginfo_allow_rowlocks (registration);
    classCapabilities->SetSupportsLocking(bClassSupportsLocking);
    if (bClassSupportsLocking)
    {
        FdoLockType lockTypes[1];
        lockTypes[0] = FdoLockType_Exclusive;
        classCapabilities->SetLockTypes(lockTypes, ELEMENTS(lockTypes));
    }
    else
        classCapabilities->SetLockTypes(NULL, 0);
    bool bSupportsVersioning = 0 != SE_reginfo_is_multiversion (registration);
    classCapabilities->SetSupportsLongTransactions(bSupportsVersioning);

    LONG permissions = 0;
    result = SE_reginfo_get_access(registration, &permissions);
    handle_sde_err<FdoSchemaException>(connection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_UNEXPECTED_ERROR, "Unexpected error encountered in ArcSDE Provider.");
    classCapabilities->SetSupportsWrite((permissions & SE_UPDATE_PRIVILEGE) || (permissions & SE_INSERT_PRIVILEGE) || (permissions & SE_DELETE_PRIVILEGE));
    newFdoClass->SetCapabilities(classCapabilities);


    // add the FDO class to it's schema
    classes = schema->GetClasses ();
    classes->Add (newFdoClass);

    // Add each column as an FDO property:
    for (int i = 0; i < column_count; i++)
    {
        try
        {
            FdoStringP defaultValue;
            fdoPropertyName = L"";

            // Determine read-only setting if rowid type != SE_REGISTRATION_ROW_ID_COLUMN_TYPE_NONE:
            bool bAutogenerated = false;
            bool bReadOnly = false;
            if (SE_REGISTRATION_ROW_ID_COLUMN_TYPE_SDE == columns[i].row_id_type ||  columns[i].sde_type == SE_UUID_TYPE)
            {
                bAutogenerated = true;
                bReadOnly = true;
            }
            else if (SE_REGISTRATION_ROW_ID_COLUMN_TYPE_USER == columns[i].row_id_type)
            {
                bAutogenerated = false;
                bReadOnly = false;
            }

            // default to property name same as column name
            wchar_t* fdoPropertyNameTmp = NULL;
            sde_multibyte_to_wide (fdoPropertyNameTmp, columns[i].column_name);
            fdoPropertyName = fdoPropertyNameTmp;
            property_description = L"";

            // default data type to -1:
            dataType = (FdoDataType)-1L;

            // search for metadata overrides for this column (only in debug mode, since this is only
			// required for ApplySchema support which is only supported in debug mode):
#ifdef _DEBUG
            for (int j = 0; j < num_records; j++)
            {
                GetArcSDEMetadata(metadata_list[j], classname, property, value, description, metadata_qual_table);

                if ((0 == connection->RdbmsNamesMatch (sde_pcus2wc(property), sde_pcus2wc(columns[i].column_name)))
                    && (0==sde_strcmp(sde_pcus2wc(qualified_table_name), sde_pcus2wc(metadata_qual_table)))
                    )
                {
                    if (0 == sde_strcmp (sde_pcus2wc(METADATA_CN_PROPNAME), sde_pcus2wc(classname)))
                    {
                        sde_multibyte_to_wide (fdoPropertyNameTmp, value);
						fdoPropertyName = fdoPropertyNameTmp;

                        // store as schema mapping override:
                        FdoPtr<ArcSDEPropertyMapping> propertyMapping = mConnection->GetPropertyMapping(newFdoClass, fdoPropertyName, false);
                        wchar_t* wColumnName = NULL;
                        sde_multibyte_to_wide (wColumnName, columns[i].column_name);
                        propertyMapping->SetColumnName(wColumnName);
                    }

                    if (0 == sde_strcmp (sde_pcus2wc(METADATA_CN_PROPDESC), sde_pcus2wc(classname)))
                    {
                        wchar_t* property_descriptionTmp = NULL;
                        sde_multibyte_to_wide (property_descriptionTmp, value);
                        property_description = property_descriptionTmp;
                    }

                    if (0 == sde_strcmp (sde_pcus2wc(METADATA_CN_PROPTYPE), sde_pcus2wc(classname)))
                    {
                        dataType = MetadataValueToFDOType(value);
                        //TODO: validate that underlying ArcSDE type is appropriate for the overriden fdo data type

                        // store as schema mapping override:
                        FdoPtr<ArcSDEPropertyMapping> propertyMapping = mConnection->GetPropertyMapping(newFdoClass, fdoPropertyName, false);
                        propertyMapping->SetColumnType(columns[i].sde_type);
                    }

                    if (0 == sde_strcmp (sde_pcus2wc(METADATA_CN_PROPREADONLY), sde_pcus2wc(classname)))
                    {
                        // Ignore this for identity properties, since they determine whether or not they're read-only:
                        if (SE_REGISTRATION_ROW_ID_COLUMN_TYPE_NONE == columns[i].row_id_type)
                            bReadOnly = MetadataValueToBoolean(value);
                    }

                    if (0 == sde_strcmp (sde_pcus2wc(METADATA_CN_PROPDEFAULTVALUE), sde_pcus2wc(classname)))
                    {
                        wchar_t* defaultValueTmp = NULL;
                        sde_multibyte_to_wide(defaultValueTmp, value);
                        defaultValue = defaultValueTmp;
                    }
                }
            }
#endif


            if (SE_SHAPE_TYPE == columns[i].sde_type)
            {
                geometry_definition = FdoGeometricPropertyDefinition::Create (fdoPropertyName, property_description);
                result = connection->GetArcSDELayerInfo(info, qualified_table_name, columns[i].column_name);
                if (SE_SUCCESS == result)
                {
                    result = SE_layerinfo_get_shape_types (info, &shapes);
                    if (SE_SUCCESS == result)
                    {
                        FdoGeometryType geomTypes[20];
                        FdoInt32 geomTypeCount = 0;

                        if (0 != (shapes & SE_POINT_TYPE_MASK))
                            geomTypes[geomTypeCount++] = FdoGeometryType_Point;
                        if (0 != (shapes & SE_POINT_TYPE_MASK) && 0 != (shapes & SE_MULTIPART_TYPE_MASK))
                            geomTypes[geomTypeCount++] = FdoGeometryType_MultiPoint;
                        if (0 != (shapes & SE_LINE_TYPE_MASK) || 0 != (shapes & SE_SIMPLE_LINE_TYPE_MASK))
                            geomTypes[geomTypeCount++] = FdoGeometryType_LineString;
                        if ((0 != (shapes & SE_LINE_TYPE_MASK) || 0 != (shapes & SE_SIMPLE_LINE_TYPE_MASK)) && 0 != (shapes & SE_MULTIPART_TYPE_MASK))
                            geomTypes[geomTypeCount++] = FdoGeometryType_MultiLineString;
                        if (0 != (shapes & SE_AREA_TYPE_MASK))
                            geomTypes[geomTypeCount++] = FdoGeometryType_Polygon;
                        if (0 != (shapes & SE_AREA_TYPE_MASK) && 0 != (shapes & SE_MULTIPART_TYPE_MASK))
                            geomTypes[geomTypeCount++] = FdoGeometryType_MultiPolygon;
                        geometry_definition->SetSpecificGeometryTypes (geomTypes, geomTypeCount);
                    }

                    // Set measure and elevation:
                    geometry_definition->SetHasElevation(SE_layerinfo_is_3D(info)==TRUE);
                    geometry_definition->SetHasMeasure(SE_layerinfo_is_measured(info)==TRUE);

                    // Set associated spatial context:
                    SE_COORDREF coordRef;
                    LONG srid = 0L;
                    result = SE_coordref_create(&coordRef);
                    handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_COORDREF_ERROR, "Unexpected error encountered while manipulating an ArcSDE coordinate reference.");
                    result = SE_layerinfo_get_coordref(info, coordRef);
                    handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_COORDREF_ERROR, "Unexpected error encountered while manipulating an ArcSDE coordinate reference.");
                    result = SE_coordref_get_srid (coordRef, &srid);
                    handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_COORDREF_ERROR, "Unexpected error encountered while manipulating an ArcSDE coordinate reference.");
                    if (srid!=0)  //NOTE: in some rare cases, it seems I may get srid==0 even though the database shows something different; possible corruption of data?
                    {
                        geometry_definition->SetSpatialContextAssociation(ArcSDESpatialContextUtility::SRIDToSpatialContextName(connection, srid));
                    }
                    SE_coordref_free (coordRef);
                }
                property_definiton = FDO_SAFE_ADDREF(geometry_definition.p);
                ((FdoFeatureClass*)newFdoClass.p)->SetGeometryProperty (geometry_definition);
                properties = newFdoClass->GetProperties ();
                properties->Add (geometry_definition);

				// Set vertex order and strictness rule for geometry property     
				classCapabilities->SetPolygonVertexOrderRule(fdoPropertyName, FdoPolygonVertexOrderRule_CCW);     
				classCapabilities->SetPolygonVertexOrderStrictness(fdoPropertyName, false); 
            }
            else  // not a shape column
            {
                // Create a new FDO data property:
                data_definition = FdoDataPropertyDefinition::Create (fdoPropertyName, property_description);
                if (dataType == -1L) // only get data type if no override has been set
                    dataType = SDEType2FdoType (columns[i].sde_type);

                if (dataType != FDODATATYPE_UNHANDLEDSDETYPE)
                {
                    data_definition->SetDataType (dataType);

                    // Set Length (Only applies to String/Blob/Clob data properties):
                    if ((dataType == FdoDataType_String) || (dataType == FdoDataType_BLOB) || (dataType == FdoDataType_CLOB))
                    {
                        data_definition->SetLength (columns[i].size);
                    }

                    // Set Precision and Scale (only applies to Decimal data properties):
                    if (dataType == FdoDataType_Decimal)
                    {
                        data_definition->SetPrecision (columns[i].size);        // total # of digits
                        data_definition->SetScale (columns[i].decimal_digits);  // # digits after the decimal
                    }

                    data_definition->SetNullable (0 == columns[i].nulls_allowed ? false : true);
                    data_definition->SetIsAutoGenerated (bAutogenerated);
                    data_definition->SetReadOnly (bReadOnly);
                    data_definition->SetDefaultValue(defaultValue);

                    // Add this property to the FDO class's property collection:
                    properties = newFdoClass->GetProperties ();
                    properties->Add(data_definition);

                    // Add this property to the FDO class's identity property collection, if an identity property:
                    if (SE_REGISTRATION_ROW_ID_COLUMN_TYPE_NONE != columns[i].row_id_type)
                    {
                        identity_properties = newFdoClass->GetIdentityProperties ();
                        identity_properties->Add (data_definition);
                    }
                }
            }
        }
        catch (FdoException *e)
        {
            // NOTE: if we get an unexpected exception while adding a property to a class,
            // quietly discard the property and keep processing the remainder of the properties.
            e->Release();
            if (fdoPropertyName.GetLength() != 0)
            {
                // remove from main geometry property, if necessary:
                if (newFdoClass->GetClassType()==FdoClassType_FeatureClass)
                {
                    FdoFeatureClass* featClass = dynamic_cast<FdoFeatureClass*>(newFdoClass.p);
                    FdoPtr<FdoGeometricPropertyDefinition> geomProp = featClass->GetGeometryProperty();
                    if ((geomProp != NULL) && (0==wcscmp(geomProp->GetName(), fdoPropertyName)))
                        featClass->SetGeometryProperty(NULL);
                }

                // remove from identity property collection, if necessary:
                FdoPtr<FdoDataPropertyDefinitionCollection> idProps = newFdoClass->GetIdentityProperties();
                FdoPtr<FdoDataPropertyDefinition> idProp = idProps->FindItem(fdoPropertyName);
                if (idProp != NULL)
                    idProps->Remove(idProp);

                // remove from property collection, if necessary:
                FdoPtr<FdoPropertyDefinitionCollection> props = newFdoClass->GetProperties();
                FdoPtr<FdoPropertyDefinition> prop = props->FindItem(fdoPropertyName);
                if (prop != NULL)
                    props->Remove(prop);
            }
        }
    }
    SE_table_free_descriptions (columns);


    // Get uniqueness constraints:
    SHORT numIndexes = 0;
    SE_INDEX_DEF *indexes = NULL;
    result = SE_table_describe_indexes(connection->GetConnection(), qualified_table_name, &numIndexes, &indexes);
    handle_sde_err<FdoSchemaException>(connection->GetConnection(), result, __FILE__, __LINE__, ARCSDE_SCHEMA_GET_INDEX_FAILED, "Failed to read index(es) on class '%1$ls'.", fdoClassName);
    FdoPtr<FdoPropertyDefinitionCollection> classProperties = newFdoClass->GetProperties();
    FdoPtr<FdoPropertyDefinition> classProperty;
    FdoPtr<FdoUniqueConstraintCollection> uniqueConstraints = newFdoClass->GetUniqueConstraints();
    for (int i=0; i<numIndexes; i++)
    {
        if (indexes[i].unique)
        {
            // Add a new unique constraint to this class:
            bool bContainsNonDataProperty = false;
            FdoPtr<FdoUniqueConstraint> uniqueConstraint = FdoUniqueConstraint::Create();
            FdoPtr<FdoDataPropertyDefinitionCollection> uniqueConstraintProperties = uniqueConstraint->GetProperties();

            for (int j=0; j<indexes[i].num_columns; j++)
            {
                CHAR* mbColumnName = indexes[i].column_name[j];
                wchar_t* wColumnName = NULL;
                sde_multibyte_to_wide(wColumnName, mbColumnName);

                FdoString* wPropertyName = connection->ColumnToProperty(newFdoClass, wColumnName, false);
                classProperty = classProperties->FindItem(wPropertyName);

                if (classProperty && classProperty->GetPropertyType() == FdoPropertyType_DataProperty)
                {
                    FdoDataPropertyDefinition* dataProperty = static_cast<FdoDataPropertyDefinition*>(classProperty.p);
                    uniqueConstraintProperties->Add(dataProperty);
                }
                else
                    bContainsNonDataProperty = true;
            }

            // Determine if this unique constraint corresponds to the identity property of this class:
            bool bUniqueConstraintIsPrimaryKey = false;
            if ( (uniqueConstraintProperties->GetCount() == 1) && identity_properties && (identity_properties->GetCount()==1)
                && (0==wcscmp(FdoPtr<FdoDataPropertyDefinition>(identity_properties->GetItem(0))->GetName(),FdoPtr<FdoDataPropertyDefinition>(uniqueConstraintProperties->GetItem(0))->GetName())) )
                 bUniqueConstraintIsPrimaryKey = true;

            // Only add this constraint if all its underlying columns mapped to data properties, since
            // a constraint can only point to data properties.  Adding a constraint that contains some data and some other property
            // types will mislead the user since we would only be able to expose the data properties and not the rest.
            if (!bContainsNonDataProperty && !bUniqueConstraintIsPrimaryKey)
                uniqueConstraints->Add(uniqueConstraint);
        }
    }
    SE_table_free_index_descriptions(numIndexes, indexes);
}

// find the schema, or create it if it doesn't exist
FdoFeatureSchema* ArcSDEDescribeSchemaCommand::findOrCreateSchema (FdoFeatureSchemaCollection* schemas, const wchar_t* name, const wchar_t* description)
{
    FdoFeatureSchema* schema;
    int size;
    int index;
    const wchar_t* schema_name;
    FdoPtr <FdoFeatureSchema> ret;

    size = schemas->GetCount ();
    index = -1;
    for (int i = 0; i < size; i++)
    {
        schema = schemas->GetItem (i);
        if (NULL != schema)
        {
            schema_name = schema->GetName();
            schema->Release ();
            if ((NULL != schema_name) && (0 == wcscmp (name, schema_name)))
            {
                index = i;
                break;
            }
        }
    }
    if (-1 == index)
    {   // create the schema if it doesn't exist
        ret = FdoFeatureSchema::Create (name, description);
        schemas->Add (ret);
    }
    else
        ret = schemas->GetItem (index);

    return (FDO_SAFE_ADDREF (ret.p));
}

void ArcSDEDescribeSchemaCommand::addTable (
	ArcSDEConnection* connection,
	FdoFeatureSchemaCollection* schemas,
	FdoStringP qFeatureClassName)
{
	CHAR* qualified_table_name;
#ifdef _DEBUG
	CHAR metadata_qual_table[SE_MAX_OBJECT_NAME_LEN];
#endif
	SE_METADATAINFO *metadata_list = NULL;
	LONG count = 0L;
#ifdef _DEBUG
	CHAR classname[SE_MAX_METADATA_CLASS_LEN];
	CHAR value[SE_MAX_METADATA_VALUE_LEN];
#endif
	CHAR description[SE_MAX_DESCRIPTION_LEN];
	FdoStringP fdoSchemaName;
	wchar_t* fdoSchemaDesc = NULL;
	FdoStringP fdoClassName;
	FdoStringP fdoClassDesc;
	LONG result = 0L;
	FdoPtr<FdoFeatureSchema> schema;

	// At the moment, the relevant SDE table registry and qualified table name shoule be cached.
	const SE_REGINFO* registration = mConnection->GetCachedTableRegistryInfo(qFeatureClassName);
	assert(registration != NULL);

	qualified_table_name = mConnection->GetCachedSDEQualifiedTableName(qFeatureClassName);
	assert(qualified_table_name != NULL);

	fdoSchemaName = qFeatureClassName.Left(L":");
	fdoClassName = qFeatureClassName.Right(L":");

	try
	{
		// Get this table's metadata (only in debug mode, since this is only required for
		// ApplySchema support which is only supported in debug mode):
#ifdef _DEBUG
		connection->GetArcSDEMetadataList(&metadata_list, &count);
		for (int i = 0; i < count; i++)
		{
			result = SE_metadatainfo_get_object_name (metadata_list[i], metadata_qual_table);
			handle_sde_err<FdoSchemaException>(result, __FILE__, __LINE__, ARCSDE_METADATA_MANIPULATE_FAILED, "Failed to get or set ArcSDE metadata.");
			if (0==sde_strcmp(sde_pcus2wc(metadata_qual_table), sde_pcus2wc(qualified_table_name)))
			{
				result = SE_metadatainfo_get_classname (metadata_list[i], classname);
				handle_sde_err<FdoSchemaException>(result, __FILE__, __LINE__, ARCSDE_METADATA_MANIPULATE_FAILED, "Failed to get or set ArcSDE metadata.");
								
				if (0==sde_strcmp(sde_pcus2wc(classname), sde_pcus2wc(METADATA_CN_CLASSDESC)))
				{
					result = SE_metadatainfo_get_value (metadata_list[i], value);
					handle_sde_err<FdoSchemaException>(result, __FILE__, __LINE__, ARCSDE_METADATA_MANIPULATE_FAILED, "Failed to get or set ArcSDE metadata.");

					wchar_t* fdoClassDescTemp = NULL;
					sde_multibyte_to_wide (fdoClassDescTemp, value);
					fdoClassDesc = fdoClassDescTemp;
				}
			}
			// TODO: METADATA_CN_CLASSATTRIBUTE, METADATA_CN_CLASSBASE, METADATA_CN_CLASSABSTRACT
		}
#endif

		// Get or create the schema this class belongs in:
		schema = findOrCreateSchema (schemas, fdoSchemaName, fdoSchemaDesc);
		FdoPtr<FdoClassCollection> classes = schema->GetClasses();
		FdoPtr<FdoClassDefinition> preloadedClass = classes->FindItem(fdoClassName);

		// If the class is already loaded, skip it.
		if (preloadedClass != NULL)
			return;

		// if we don't have a class description yet, get a default description from class registration info:
		if (fdoClassDesc.GetLength() == 0)
		{
			result = SE_reginfo_get_description (*registration, description);
			if (SE_SUCCESS == result)
			{
				wchar_t* fdoClassDescTmp = NULL;
				sde_multibyte_to_wide (fdoClassDescTmp, description);
				fdoClassDesc = fdoClassDescTmp;
			}
			if (fdoClassDesc.GetLength() == 0)
				fdoClassDesc = L"Default class description";
		}

		// Create the FDO class and its FDO properties:
		addClass (connection, schema, (wchar_t*)(FdoString*)fdoClassName, (wchar_t*)(FdoString*)fdoClassDesc, qualified_table_name, *registration, metadata_list, count);

		// Populate the schema attributes dictionary:
		//TODO: do we want to try to expose native ArcSDE metadata as schema attributes?
		/*
		dictionary = schema->GetAttributes ();
		for (int i = 0; i < count; i++)
		{
		GetArcSDEMetadata(metadata_list[i], classname, property, value, NULL);
		if (   (0 != strcmp (SCHEMA_KEYWORD, classname))
		&& (0 != strcmp (CLASS_KEYWORD, classname))
		&& (0 != strcmp (COLUMN_KEYWORD, classname)))
		{
		multibyte_to_wide (wproperty, property);
		multibyte_to_wide (wvalue, value);
		if ((NULL != wproperty) && (NULL != wvalue))
		dictionary->Add (wproperty, wvalue);
		}
		}
		*/
	}
	catch (FdoException *e)
	{
		// NOTE: if an exception occurs during this method, we will ignore it and silently
		// remove the class from its associated schema:
		e->Release();
		if ((schema != NULL) && (fdoClassName != NULL))
		{
			FdoPtr<FdoClassCollection> classes = schema->GetClasses();
			FdoPtr<FdoClassDefinition> addedClass = classes->FindItem(fdoClassName);
			if (addedClass != NULL)
				classes->Remove(addedClass);
		}
	}
}


/// <summary>Executes the DescribeSchema command and returns a 
/// FdoFeatureSchemaCollection. If a schema name is given that has 
/// references to another schema, the dependent schemas will 
/// be returned as well. If the specified schema name does not exist,
/// the Execute method throws an exception.</summary>
/// <returns>Returns the schema collection representing the schema created.</returns> 
FdoFeatureSchemaCollection* ArcSDEDescribeSchemaCommand::Execute ()
{
    FdoPtr<ArcSDEConnection> connection;
    FdoPtr<FdoFeatureSchemaCollection> ret;

    // verify the connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established."));

    // Load schemas, unless already cached:
    //TODO: need to check database to see if it has changed since it was cached!
	bool bIsFullyLoaded = false;
    FdoPtr<FdoFeatureSchemaCollection> cachedSchemas = connection->GetSchemaCollection (NULL, false, &bIsFullyLoaded);
    FdoPtr<FdoPhysicalSchemaMappingCollection> schemaMappings = connection->GetSchemaMappingCollection(NULL, NULL, false);
	
	// Get all the schema/class names.
	// TODO: make further optimization to only get the requested schema/class names.
	if (!mConnection->IsSchemaClassNamesCached())
		mConnection->GetRegisteredTableNames();
	
	// Construct the requested qualified feature class name collection,
	// If both schema name and class names are not set in this command, 
	// the collection will contain all the qualified feature class names.
	FdoStringsP qFeatureClassNames = FdoStringCollection::Create();
	// The flag indicate if a full describing is requested.
	bool bIsAllRequested = false;
	constructRequestedClassNames(qFeatureClassNames, &bIsAllRequested);

	// If the schema collection is already loaded fully, we don't need try to construct it again.
	if (!bIsFullyLoaded)
	{
		// Construct the requested schema/feature class.
		FdoInt32 count = qFeatureClassNames->GetCount();
		for (FdoInt32 i = 0; i < count; ++i)
		{
			addTable(connection, cachedSchemas, qFeatureClassNames->GetString(i));
		}

		// Mark all schemas as "unchanged":
		int size = cachedSchemas->GetCount ();
		for (int i = 0; i < size; i++)
		{
			FdoPtr<FdoFeatureSchema> schema = cachedSchemas->GetItem (i);
			schema->AcceptChanges ();
		}

		// Cache these schemas on this connection:
		connection->SetSchemaCollection (cachedSchemas, bIsAllRequested);
	}

	// clone the relevant schemas from the cached schema collection.
	if (bIsAllRequested)
		// If this is fully describing schema, clone the entire schema collection.
		ret = FdoCommonSchemaUtil::DeepCopyFdoFeatureSchemas(cachedSchemas, NULL);
	else
	{
		// Just clone the requested schema/class.
		ret = FdoFeatureSchemaCollection::Create(NULL);
		cloneClassDefinitions(qFeatureClassNames, ret, cachedSchemas);
	}
	
    return (FDO_SAFE_ADDREF(ret.p));
}

void ArcSDEDescribeSchemaCommand::cloneClassDefinitions(FdoStringCollection* qFCNames, FdoFeatureSchemaCollection* retSchemaCol, FdoFeatureSchemaCollection* schemaCol)
{
	FdoInt32 count = qFCNames->GetCount();
	for (FdoInt32 i = 0; i < count; ++i)
	{
		FdoStringP qualifiedClassName = qFCNames->GetString(i);
		FdoStringP schemaName = qualifiedClassName.Left(L":");
		FdoStringP className = qualifiedClassName.Right(L":");

		
		// It's possible that the requested schema doesn't exist due to some reasons. See defect 613177.01.
		if (!schemaCol->Contains(schemaName))
			continue;
		FdoPtr<FdoFeatureSchema> schema = schemaCol->GetItem(schemaName);

		FdoPtr<FdoClassCollection> classes = schema->GetClasses();
		if (classes == NULL) // it's needed to do the check?
			continue;

		// It's possible that the requested schema doesn't exist due to some reasons. See defect 613177.01.
		if (!classes->Contains(className))
			continue;
		FdoPtr<FdoClassDefinition> classDef = classes->GetItem(className);

		FdoPtr<FdoFeatureSchema> retSchema;
		if (!retSchemaCol->Contains(schema->GetName()))
		{
			retSchema = FdoFeatureSchema::Create(schema->GetName(), schema->GetDescription());
			retSchemaCol->Add(retSchema);
		}
		else
			retSchema = retSchemaCol->GetItem(schema->GetName());

		classes = retSchema->GetClasses();
        // At this point, classes should be check to see if it already contains the current class. 
		// The deep copy and adding of the class can be skipped if it already is. This can happen 
		// if the client puts a duplicate entry in mClassNames, or if a class in qFCNames is the 
		// base class of a class appearing earlier on in qFCNames. 
		if (classes->Contains(classDef->GetName()))
			continue;

		classDef = FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(classDef, NULL);
		FDO_SAFE_ADDREF(classDef.p);
		classes->Add(classDef);

		retSchema->AcceptChanges();
	}
}

void ArcSDEDescribeSchemaCommand::constructRequestedClassNames(FdoStringCollection* qFeatureClassNames, bool* isFullyLoaded)
{
	if (mSchemaName != NULL && mSchemaName[0] != '\0') // The schema name was set.
	{
		FdoStringsP cachedSchemaNames = mConnection->GetSchemaNames();
		// Not find the specified schema name, an exception will be thrown.
		if (cachedSchemaNames->IndexOf(mSchemaName) == -1)
			throw FdoException::Create(NlsMsgGet1(ARCSDE_SCHEMA_DOES_NOT_EXIST, "Schema '%1$ls' does not exist.", (FdoString*)mSchemaName)); 

		FdoInt32 size = mClassNames->GetCount();
		for (FdoInt32 i = 0; i < size; ++i)
		{
			FdoStringP className = mClassNames->GetString(i);
			FdoStringsP cachedClassNames = mConnection->GetFeatureClassNames(mSchemaName);
			//
			// className might have been passed in as a qualified name. 
			// Also, to be consistent with the GenericRdbms providers, 
			// an exception should be thrown if className is qualified 
			// but its schema name part differs from mSchemaName
			FdoStringP qFCName;
			if (className.Contains(L":"))
			{
				FdoStringP schemaName = className.Left(L":");
				if (schemaName != mSchemaName)
					throw FdoException::Create(NlsMsgGet2(ARCSDE_SCHEMA_NAME_INCONSISTENT, 
					"The schema in the qualified class name '%1$ls' is inconsistent with the specified one '%2$ls'.", (FdoString*)className, (FdoString*)mSchemaName));

				qFCName = className;
			}
			else
			{
				qFCName = FdoStringP(mSchemaName) + L":" + className;
			}

			if (cachedClassNames->IndexOf(qFCName) == -1)
				throw FdoException::Create(NlsMsgGet1(ARCSDE_FEATURE_CLASS_NOT_FOUND, "Feature class '%1$ls' not found in schema.", (FdoString*)qFCName)); 

			qFeatureClassNames->Add(qFCName);
		}

		// No feature class name is specified, get all the feature class names in the schema.
		if (qFeatureClassNames->GetCount() == 0)
		{
			FdoStringsP cachedClassNames = mConnection->GetFeatureClassNames(mSchemaName);
			for (FdoInt32 i = 0; i < cachedClassNames->GetCount(); ++i)
			{
				FdoStringP className = cachedClassNames->GetString(i);
				qFeatureClassNames->Add(className);
			}
		}
	}
	else // The schema name is not set.
	{
		FdoStringsP cachedSchemaNames = mConnection->GetSchemaNames();
		for (FdoInt32 i = 0; i < mClassNames->GetCount(); ++i)
		{
			FdoStringP className = mClassNames->GetString(i);
			// If the class name is qualified, we should verify if it exists.
			// If not, an exception will be thrown.
			if (className.Contains(L":"))
			{
				FdoStringP schemaName = className.Left(L":");
				if (cachedSchemaNames->IndexOf(schemaName) == -1)
					throw FdoException::Create(NlsMsgGet1(ARCSDE_FEATURE_CLASS_NOT_FOUND, "Feature class '%1$ls' not found in schema.", (FdoString*)className)); 
				FdoStringsP cachedClassNames = mConnection->GetFeatureClassNames(schemaName);
				if (cachedClassNames->IndexOf(className) == -1)
					throw FdoException::Create(NlsMsgGet1(ARCSDE_FEATURE_CLASS_NOT_FOUND, "Feature class '%1$ls' not found in schema.", (FdoString*)className)); 

				qFeatureClassNames->Add(className);
			}
			// If the class name is not qualified,
			// to find the class names in all feature schema.
			else
			{
				bool isFound = false;
				for (FdoInt32 j = 0; j < cachedSchemaNames->GetCount(); ++j)
				{
					FdoStringP schemaName = cachedSchemaNames->GetString(j);
					FdoStringsP cachedClassNames = mConnection->GetFeatureClassNames(schemaName);
					FdoStringP qFCName = schemaName + L":" + className;
					// The schema has a class with the same name.
					if ( cachedClassNames->IndexOf(qFCName) != -1)
					{
						qFeatureClassNames->Add(qFCName);
						isFound = true;
					}
				}

				if (!isFound)
					throw FdoException::Create(NlsMsgGet1(ARCSDE_FEATURE_CLASS_NOT_FOUND, "Feature class '%1$ls' not found in schema.", (FdoString*)className)); 
			}
		}

		// At the point, we can conclude that both the schema name and class names are not specified.
		// We should do a full schema describing.
		if (qFeatureClassNames->GetCount() == 0)
		{
			// Get all of the qualified feature class name.
			FdoStringsP cachedClassNames = mConnection->GetFeatureClassNames(NULL);
			for (FdoInt32 j = 0; j < cachedClassNames->GetCount(); ++j)
			{
				FdoStringP className = cachedClassNames->GetString(j);
				qFeatureClassNames->Add(className);
			}

			*isFullyLoaded = true;
		}

		//
		// Verify that the requested qualified feature class names all exist.
		FdoStringsP cachedClassNames = mConnection->GetFeatureClassNames(NULL); // Get all the cached feature class names.
		for (FdoInt32 i = 0; i < qFeatureClassNames->GetCount(); ++i)
		{
			FdoStringP qFCName = qFeatureClassNames->GetString(i);
			// If no existing class matches with the requested one, an exception will be thrown.
			if (cachedClassNames->IndexOf(qFCName) == -1)
				throw FdoException::Create(NlsMsgGet1(ARCSDE_FEATURE_CLASS_NOT_FOUND, "Feature class '%1$ls' not found in schema.", (FdoString*)qFCName));
		}
	}
}

// the caller must allocate the arguments to the following sizes, or pass in NULL:
//    CHAR classname[SE_MAX_METADATA_CLASS_LEN];
//    CHAR description[SE_MAX_DESCRIPTION_LEN];
//    CHAR property[SE_MAX_METADATA_PROPERTY_LEN];
//    CHAR value[SE_MAX_METADATA_VALUE_LEN];
void ArcSDEDescribeSchemaCommand::GetArcSDEMetadata(const SE_METADATAINFO &metadata, CHAR* classname, CHAR* property, CHAR* value, CHAR* description, CHAR* metadata_qual_table)
{
    LONG result = 0L;

    if (NULL != classname)
    {
        result = SE_metadatainfo_get_classname (metadata, classname);
        handle_sde_err<FdoSchemaException>(result, __FILE__, __LINE__, ARCSDE_METADATA_MANIPULATE_FAILED, "Failed to get or set ArcSDE metadata.");
    }

    if (NULL != property)
    {
        result = SE_metadatainfo_get_property (metadata, property);
        handle_sde_err<FdoSchemaException>(result, __FILE__, __LINE__, ARCSDE_METADATA_MANIPULATE_FAILED, "Failed to get or set ArcSDE metadata.");
    }

    if (NULL != value)
    {
        result = SE_metadatainfo_get_value (metadata, value);
        handle_sde_err<FdoSchemaException>(result, __FILE__, __LINE__, ARCSDE_METADATA_MANIPULATE_FAILED, "Failed to get or set ArcSDE metadata.");
    }

    if (NULL != description)
    {
        result = SE_metadatainfo_get_description (metadata, description);
        handle_sde_err<FdoSchemaException>(result, __FILE__, __LINE__, ARCSDE_METADATA_MANIPULATE_FAILED, "Failed to get or set ArcSDE metadata.");
    }

    if (NULL != metadata_qual_table)
    {
        result = SE_metadatainfo_get_object_name (metadata, metadata_qual_table);
        handle_sde_err<FdoSchemaException>(result, __FILE__, __LINE__, ARCSDE_METADATA_MANIPULATE_FAILED, "Failed to get or set ArcSDE metadata.");
    }
}



