// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include "stdafx.h"
#include "SchemaDb.h"
#include "SdfConnection.h"
#include <FdoCommonSchemaUtil.h>

#define   SDF_CLASSHEADER_CLASSTYPE			0x000000ff
#define   SDF_CLASSHEADER_ISABSTRACT		0x00000100

char* DB_SCHEMA_NAME = "SCHEMA"; //NOXLATE
const REC_NO DB_METADATA_RECNO = 1;
const REC_NO DB_COORDSYS_RECNO = 2;
const REC_NO DB_SCHEMA_ROOT_RECNO = 3;

static const FdoInt32 APPLY_SCHEMA_ERROR_LIMIT = 100;

//----------------------------------------------------------------------------------
// SchemaDb database organization
// ==============================
//
// We store metadata other than the schema in the SchemaDb database, including
// the coordinate system for the SDF file.
// Here is how the records in this database are currently organized:
//
// Record 1 Reserved for stuff like Creator, Timestamp, Password, whatever...)
// Record 2 
//      Coordinate System
// Record 3
//      Root of SDF FeatureSchema
// Record 4+
//      Feature class records pointed to by Feature schema root node
//----------------------------------------------------------------------------------




SchemaDb::SchemaDb(SQLiteDataBase* env, const char* filename, bool bReadOnly) :
    m_bHasAssociations(false),
    m_majorVersion(0),
    m_minorVersion(0),
	m_scName( NULL ),
    m_env( env )
{
    m_db = new SQLiteTable(env);

    int res;

    int readOnlyFlag = bReadOnly ? SQLiteDB_RDONLY : 0;

    //try to open a database that already exists
    if (res = m_db->open(0, filename, DB_SCHEMA_NAME, readOnlyFlag, 0) == 0)
    {
        // Verify that this SDF File is one of the supported versions:
        ReadMetadata(m_majorVersion, m_minorVersion);
        if (
               (m_majorVersion != SDFPROVIDER_VERSION_MAJOR_3 || m_minorVersion != SDFPROVIDER_VERSION_MINOR_3_0)
            && (m_majorVersion != SDFPROVIDER_VERSION_MAJOR_3 || m_minorVersion != SDFPROVIDER_VERSION_MINOR_3_1)
            && (m_majorVersion != SDFPROVIDER_VERSION_MAJOR_3 || m_minorVersion != SDFPROVIDER_VERSION_MINOR_3_2)
            )
            throw FdoConnectionException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_5_INCORRECT_SDF_VERSION),
                m_majorVersion, m_minorVersion, SDFPROVIDER_VERSION_MAJOR_CURRENT, SDFPROVIDER_VERSION_MINOR_CURRENT_NLS));
    }
    else
    {
        //must close even if open failed
        m_db->close(0);
        delete m_db;

        if (bReadOnly)
            throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_4_CONNECTION_IS_READONLY)));

        m_db = new SQLiteTable(env);

        // Since we failed to open an existing SDF file, create a new one instead:
        if (res = m_db->open(0, filename, DB_SCHEMA_NAME, SQLiteDB_CREATE, 0) != 0)
            throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_10_ERROR_ACCESSING_SDFDB)));

        //we are creating the SDF file, so we have to write an 
        //SDF version ID to metadata record in the schema database
        WriteMetadata(SDFPROVIDER_VERSION_MAJOR_CURRENT, SDFPROVIDER_VERSION_MINOR_CURRENT);
    }

    //retrieve root schema record -- it is stored in position 3 of the db
    REC_NO rootRecno = DB_SCHEMA_ROOT_RECNO;
    SQLiteData keySchema(&rootRecno, sizeof(REC_NO));
    SQLiteData data;

    //deserialize the schema from the database
    //If there isn't any, it will be set to NULL
    m_schema = ReadSchema();
}


SchemaDb::~SchemaDb()
{
    FDO_SAFE_RELEASE(m_schema);

	if( m_scName )
		delete[] m_scName;

    m_db->close(0);
    delete m_db;
}


FdoFeatureSchema* SchemaDb::ReadSchema(FdoString *schemaName)
{
    //retrieve root schema record -- it is stored in position 3 of the db
    REC_NO rootRecno = DB_SCHEMA_ROOT_RECNO;
    SQLiteData keySchema(&rootRecno, sizeof(REC_NO));
    SQLiteData data;

	// Initialize the spatial context name needed by all the geometry properties
	BinaryWriter wrt(256);
    ReadCoordinateSystemRecord(wrt);

    if (wrt.GetDataLen() != 0)
    {
		//read all the coord sys properties from the 
		//binary record
		BinaryReader rdr(wrt.GetData(), wrt.GetDataLen());

		FdoString* csName = rdr.ReadString();
		if (csName != NULL)
		{
			size_t len = wcslen(csName);
            if (m_scName)
                delete [] m_scName;
			m_scName = new wchar_t[len+1];
			wcscpy(m_scName, csName);
		}
	}

    //this can fail if there is no schema in the database...
    //so return NULL in that case
    if (m_db->get(0, &keySchema, &data, 0) != 0)
    {
        CloseCursor();
        return NULL;
    }

    FdoPtr<FdoFeatureSchema>schema = FdoFeatureSchema::Create(NULL,NULL);

    //check that we have something in there
    _ASSERT(data.get_size() > 0);

    BinaryReader rdr((unsigned char*)data.get_data(), data.get_size());

    //first, read name of schema
    FdoString *dbSchemaName = rdr.ReadString();
    if (schemaName && wcscmp(schemaName, dbSchemaName) != 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_58_INVALID_SCHEMANAME)));

    schema->SetName(dbSchemaName);
    schema->SetDescription(rdr.ReadString());

	

    //now, read number of feature class definitions
    int numClasses = rdr.ReadInt32();

    //read all the record numbers -- we need to do this
    //in advance because our SQLiteData record will get reset
    //once we start reading feature classes -- we don't
    //own the memory of the record
    REC_NO* recs = new REC_NO[numClasses];
    for (int i=0; i<numClasses; i++)
        recs[i] = rdr.ReadInt32();

    //now read records #s for feature classes and process them
    for (int i=0; i<numClasses; i++)
    {
        REC_NO recno = recs[i];

        //class collection is stored in the records following the schema record
        //make sure that this is correct by assuring record numbers start at 4
        //and go up for each class
        _ASSERT(recno == i+DB_SCHEMA_ROOT_RECNO+1);
        
        ReadFeatureClass(recno, schema);
    }

	// Fix any missing cross-references between schema elements
	PostReadSchema( schema );

	// This method(SchemaDb::ReadSchema) gets call just opening the FdoIConnection.
	// The following closes any open cursors so inserting features would work. 
	CloseCursor();

    delete [] recs;
    
    return schema.Detach();
}


void SchemaDb::GetSchemaVersion(unsigned char& major, unsigned char& minor)
{
    major = m_majorVersion;
    minor = m_minorVersion;
}

FdoFeatureSchema* SchemaDb::GetSchema()
{
    return m_schema;
}



void SchemaDb::SetSchema(SdfISchemaMergeContextFactory* mergeFactory, FdoFeatureSchema* schema, bool ignoreStates)
{
    _ASSERT(schema != NULL);

    bool transactionStarted = false;

    // Work on copy of current schema in case we hit an error and mess them up.
    FdoFeatureSchemaP oldSchema = m_schema ? FdoCommonSchemaUtil::DeepCopyFdoFeatureSchema( m_schema ) : (FdoFeatureSchema*) NULL;

    bool hasOldSchema = (oldSchema == NULL);

    // Merge applied schema into current schemas
    SdfSchemaMergeContextP context = MergeSchema( mergeFactory, oldSchema, FDO_SAFE_ADDREF(schema), ignoreStates );
    
    FdoFeatureSchemaP mergedSchema;
    if ( context ) {
        // Get the merged schema
        FdoFeatureSchemasP mergedSchemas = context->GetSchemas();
        mergedSchema = mergedSchemas->GetItem( oldSchema->GetName() );

        // Handle database updates that must be done before AcceptChanges() is called on merged
        // schemas, and before merged schemas are written to database.
        // No updates necessary if no merge context (no existing schema).
        PreUpdatePhysical( context );
    }
    else {
        // Merge Context is NULL, meaning that there is no existing schema.
        // Use given schemas as is.
        mergedSchema = FDO_SAFE_ADDREF(schema);
    }

    // Remove any deleted elements so they are not written to the database.
    // AcceptChanges removes any elements with "Deleted" state.
    mergedSchema->AcceptChanges();

    // Update the schema table with resulting schemas.

    // Start transaction so we can roll back on error.
    if ( !m_env->transaction_started() ) {
        if ( m_env->begin_transaction() != 0 ) 
            throw FdoSchemaException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_78_START_TRANSACTION)));

        transactionStarted = true;
    }

    try {
        // Write the merged schemas to the database
        WriteSchema( mergedSchema );

        if ( context ) {
            // Handle database updates that must be done after new schemas are written to database.
            // No updates necessary if no merge context (no existing schema).
            PostUpdatePhysical( context );
        }
    }
    catch ( ... ) {
        // Rollback on error
        if ( transactionStarted ) 
            m_env->rollback();

        throw;
    }

    if ( transactionStarted ) {
        // Successful, so commit schema changes.
        if ( m_env->commit() != 0 ) 
            throw FdoSchemaException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_79_COMMIT_TRANSACTION)));

    }

    if ( context ) {
        // Clear element states for applied schema as per FDO IApplySchema spec.
        // If no Merge Context was created then this was already since schema and 
        // mergedSchema are the same.
        schema->AcceptChanges();
    }
}

SdfSchemaMergeContextP SchemaDb::MergeSchema(
    SdfISchemaMergeContextFactory* mergeFactory,    
    FdoFeatureSchemaP oldSchema, 
    FdoFeatureSchemaP newSchema, 
    bool ignoreStates
)
{
    if ( oldSchema == NULL ) 
        // Datastore has no schema so no need to merge.
        return SdfSchemaMergeContextP();


    // Create a context to merge applied schema into existing schema. 

    FdoFeatureSchemasP oldSchemas = FdoFeatureSchemaCollection::Create((FdoSchemaElement*) NULL);
    oldSchemas->Add(oldSchema);

    FdoFeatureSchemasP dummySchemas = FdoFeatureSchemaCollection::Create((FdoSchemaElement*) NULL);

    SdfSchemaMergeContextP context = mergeFactory->CreateMergeContext( oldSchemas, newSchema, ignoreStates );

    try {
        // Perform the merge.
        context->CommitSchemas();
    }
    catch ( FdoException* ex ) {
        // The SDF provider does not currently throw chained exceptions so combine all exception
        // messages into a single exception.

        FdoStringsP msgs = FdoStringCollection::Create();

        FdoInt32 idx;
        FdoPtr<FdoException> currEx = FDO_SAFE_ADDREF(ex);
        for ( idx = 0; idx < APPLY_SCHEMA_ERROR_LIMIT, currEx; idx++ ) {
            msgs->Add( currEx->GetExceptionMessage() );
            currEx = currEx->GetCause();
        }

        if ( currEx ) {
            // Add message indicating that not all errors shown
            msgs->Add( 
                FdoPtr<FdoSchemaException>(
                    FdoSchemaException::Create(
                        NlsMsgGetMain(
                            FDO_NLSID(SDFPROVIDER_80_TOO_MANY_ERRORS),  
                            APPLY_SCHEMA_ERROR_LIMIT
                        )
                    )
                )->GetExceptionMessage()
            );
        }

        FDO_SAFE_RELEASE(ex);
        FdoSchemaException* schEx = FdoSchemaException::Create( msgs->ToString(L"\n") );
        throw schEx;
    }

    return context;
}

void SchemaDb::PreUpdatePhysical( SdfSchemaMergeContextP mergeContext )
{
    mergeContext->PreUpdatePhysical();
}

void SchemaDb::PostUpdatePhysical( SdfSchemaMergeContextP mergeContext )
{
    mergeContext->PostUpdatePhysical();
}

void SchemaDb::ReadFeatureClass(REC_NO classRecno, FdoFeatureSchema* schema)
{
    //retrieve feature class record from given record in db
    REC_NO rootRecno = classRecno;
    SQLiteData keySchema(&rootRecno, sizeof(REC_NO));
    SQLiteData data;

    if (m_db->get(0, &keySchema, & data, 0) != 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_20_SCHEMA_STORAGE_ERROR)));

    //check that we have something in there
    _ASSERT(data.get_size() > 0);

    BinaryReader rdr((unsigned char*)data.get_data(), data.get_size());

    //read type of class -- feature or ordinary
	unsigned int header = rdr.ReadUInt32();
    FdoClassType type = (FdoClassType) ( header & SDF_CLASSHEADER_CLASSTYPE );
	
    FdoPtr<FdoClassDefinition> clas;
    if (type == FdoClassType_FeatureClass)
        clas = FdoFeatureClass::Create();
    else
        clas = FdoClass::Create();

	clas->SetIsAbstract( ( header & SDF_CLASSHEADER_ISABSTRACT ) !=0 );
    //set the class capabilities.
    //In the case of SDF, the class is not too capable...
    FdoPtr<FdoClassCapabilities> classcaps = FdoClassCapabilities::Create(*clas.p);
    classcaps->SetSupportsLocking(false);
    classcaps->SetSupportsLongTransactions(false);

    clas->SetCapabilities(classcaps);
    

    clas->SetName(rdr.ReadString());
    clas->SetDescription(rdr.ReadString());

    //read base class name
    FdoString* base = rdr.ReadString();

    //get base class here.......
    //it is guaranteed to be read already because of the way
    //we save classes recursively
    if (base != NULL && wcslen(base) > 0)
    {
        FdoPtr<FdoClassCollection> classes = schema->GetClasses();

        //it has to be there.
        FdoPtr<FdoClassDefinition> baseClass = classes->GetItem(base);
        
        //we will build up this collection to contain all the base properties
        //of the current class
        FdoPtr<FdoPropertyDefinitionCollection> allBaseProps = FdoPropertyDefinitionCollection ::Create(NULL);

        //first get the base properties of the base class
        FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseBaseProps = baseClass->GetBaseProperties();

        if (baseBaseProps)
        {
            for (int i=0; i<baseBaseProps->GetCount(); i++)
            {
                FdoPtr<FdoPropertyDefinition> pd = baseBaseProps->GetItem(i);
                allBaseProps->Add(pd);
            }
        }

        //now get the properties of the base class itself
        FdoPtr<FdoPropertyDefinitionCollection> baseClassProps = baseClass->GetProperties();

        if (baseClassProps)
        {
            for (int i=0; i<baseClassProps->GetCount(); i++)
            {
                FdoPtr<FdoPropertyDefinition> pd = baseClassProps->GetItem(i);
                allBaseProps->Add(pd);
            }
        }

        clas->SetBaseProperties(allBaseProps);
        clas->SetBaseClass(baseClass);
    }

    //now read properties...
    int numProps = rdr.ReadInt32();

    FdoPtr<FdoPropertyDefinitionCollection> pdc = clas->GetProperties();

    for (int i=0; i<numProps; i++)
    {
        int type = rdr.ReadInt32();

        switch(type)
        {
            case FdoPropertyType_DataProperty: ReadDataPropertyDefinition(rdr, pdc);
                break;
            case FdoPropertyType_GeometricProperty: ReadGeometricPropertyDefinition(rdr, pdc);
                break;
            case FdoPropertyType_ObjectProperty: ReadObjectPropertyDefinition(rdr, pdc);
                break;
			case FdoPropertyType_AssociationProperty: ReadAssociationPropertyDefinition(rdr, pdc);
                break;
            default:
                throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_21_UNKNOWN_PROPERTY_TYPE)));
        }
    }


    //read number of identity properties
    FdoPtr<FdoDataPropertyDefinitionCollection> idprops = clas->GetIdentityProperties();
    int numIdProps = rdr.ReadInt32();

    for (int i=0; i<numIdProps; i++)
    {
        FdoString* name = rdr.ReadString();

        FdoPtr<FdoDataPropertyDefinition> dpd;

        //get a property definition from the collection and cast it to a data 
        //property. FDO makes this a little ugly...
        dpd = dynamic_cast<FdoDataPropertyDefinition*>(pdc->FindItem (name));

        //if we could not find it in the class properties, look in the inherited properties
        if (dpd == NULL)
        {
            //get a property definition from the collection and cast it to a geometric 
            //property. FDO makes this a little ugly...
            FdoPtr<FdoReadOnlyPropertyDefinitionCollection> ropdc = clas->GetBaseProperties();

            dpd = dynamic_cast<FdoDataPropertyDefinition*> (
                ((FdoReadOnlyCollection<FdoPropertyDefinition, FdoIDisposableCollection, FdoSchemaException>*)ropdc)->GetItem(name));
        }

        if (dpd != NULL)
            idprops->Add(dpd);
    }

    if (type == FdoClassType_FeatureClass)
    {
        //read name of geometry property in case of a feature class
        FdoString* geomname = rdr.ReadString();

        if (geomname && wcslen(geomname) > 0)
        {
            //look for geometry property in class properties we already read
            FdoPtr<FdoGeometricPropertyDefinition> pd;
            
            //get a property definition from the collection and cast it to a geometric 
            //property. FDO makes this a little ugly...
            pd = dynamic_cast<FdoGeometricPropertyDefinition*>(pdc->FindItem(geomname));

            //if we could not find it in the class properties, look in the inherited properties
            //if it's not there, the class does not have a geometry property
            if (pd == NULL)
            {
                //get a property definition from the collection and cast it to a geometric 
                //property. FDO makes this a little ugly...

                FdoPtr<FdoReadOnlyPropertyDefinitionCollection> ropdc = clas->GetBaseProperties();

                try
                {
                    pd = dynamic_cast<FdoGeometricPropertyDefinition*> (
                        ((FdoReadOnlyCollection<FdoPropertyDefinition, FdoIDisposableCollection, FdoSchemaException>*)ropdc)->GetItem(geomname));
                }
                catch (FdoException* e)
                {
                    //pd remains NULL
                    e->Release();
                }
            }

            (dynamic_cast<FdoFeatureClass*>(clas.p))->SetGeometryProperty(pd);
        }
    }

    //add new class to the schema
    FdoPtr<FdoClassCollection>(schema->GetClasses())->Add(clas);

}


//fills out the identity and base properties collections
void CompleteFeatureClass(FdoClassDefinition* clas)
{
}

void SchemaDb::ReadDataPropertyDefinition(BinaryReader& rdr, FdoPropertyDefinitionCollection* pdc)
{
    //create property
    FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create();

    //read property members from data array and set them
    dpd->SetName(rdr.ReadString());
    dpd->SetDescription(rdr.ReadString());
    dpd->SetDataType((FdoDataType)rdr.ReadInt32());
    dpd->SetDefaultValue(rdr.ReadString());
    dpd->SetLength(rdr.ReadInt32());
    dpd->SetNullable(rdr.ReadByte() != 0);
    dpd->SetPrecision(rdr.ReadInt32());
    dpd->SetReadOnly(rdr.ReadByte() != 0);
    dpd->SetScale(rdr.ReadInt32());
    dpd->SetIsAutoGenerated(rdr.ReadByte() != 0);
    dpd->SetReadOnly(rdr.ReadByte() != 0);

    //read property constraints, if this version of SDF supports it:
    if (SdfConnection::VersionIsAtLeast(m_majorVersion, m_minorVersion, SDFPROVIDER_VERSION_MAJOR_3, SDFPROVIDER_VERSION_MINOR_3_1))
    {
        bool bHasValueConstraint = (rdr.ReadByte() != 0);
        if (bHasValueConstraint)
        {
            FdoPropertyValueConstraintType constraintType = (FdoPropertyValueConstraintType)rdr.ReadByte();
            FdoPtr<FdoPropertyValueConstraint> constraint;
            switch (constraintType)
            {
                case FdoPropertyValueConstraintType_Range:
                {
                    FdoPtr<FdoPropertyValueConstraintRange> constraintRange = FdoPropertyValueConstraintRange::Create();
                    constraintRange->SetMaxInclusive(rdr.ReadByte() != 0);
                    FdoPtr<FdoDataValue> maxValue = rdr.ReadDataValue();
                    constraintRange->SetMaxValue(maxValue);
                    constraintRange->SetMinInclusive(rdr.ReadByte() != 0);
                    FdoPtr<FdoDataValue> minValue = rdr.ReadDataValue();
                    constraintRange->SetMinValue(minValue);
                    constraint = FDO_SAFE_ADDREF(constraintRange.p);
                }
                break;

                case FdoPropertyValueConstraintType_List:
                {
                    FdoPtr<FdoPropertyValueConstraintList> constraintList = FdoPropertyValueConstraintList::Create();
                    FdoPtr<FdoDataValueCollection> dataValues = constraintList->GetConstraintList();
                    FdoInt32 dataValueCount = rdr.ReadInt32();
                    for (int i=0; i<dataValueCount; i++)
                    {
                        FdoPtr<FdoDataValue> listItemValue = rdr.ReadDataValue();
                        dataValues->Add(listItemValue);
                    }
                    constraint = FDO_SAFE_ADDREF(constraintList.p);
                }
                break;

                default:
                    throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_20_SCHEMA_STORAGE_ERROR)));
            }
            dpd->SetValueConstraint(constraint);
        }
    }

    if (dpd->GetIsAutoGenerated())
        dpd->SetReadOnly(true); //autogen properties are readonly

    //add property to collection
    pdc->Add(dpd);
}

void SchemaDb::ReadGeometricPropertyDefinition(BinaryReader& rdr, FdoPropertyDefinitionCollection* pdc)
{
    //create property
    FdoPtr<FdoGeometricPropertyDefinition> gpd = FdoGeometricPropertyDefinition::Create();
    
    //read property members from data array and set them
    gpd->SetName(rdr.ReadString());
    gpd->SetDescription(rdr.ReadString());
    gpd->SetGeometryTypes(rdr.ReadInt32());
    gpd->SetHasElevation(rdr.ReadByte() != 0);
    gpd->SetHasMeasure(rdr.ReadByte() != 0);
    gpd->SetReadOnly(rdr.ReadByte() != 0);        
	if( m_scName != NULL )
		gpd->SetSpatialContextAssociation( m_scName );
    //add property to collection
    pdc->Add(gpd);
}

void SchemaDb::ReadObjectPropertyDefinition(BinaryReader& rdr, FdoPropertyDefinitionCollection* pdc)
{
    throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_22_OBJECT_PROPERTY)));
}

void SchemaDb::WriteSchema(FdoFeatureSchema* schema)
{
    //now write schema to database
    BinaryWriter wrt(256);
    wrt.WriteString(schema->GetName());
    wrt.WriteString(schema->GetDescription());

    FdoPtr<FdoClassCollection> classes = schema->GetClasses();

    //how many classes
    int numClasses = classes->GetCount();
    wrt.WriteInt32(numClasses);

    //write the record # of each class -- we store class records
    //right after the schema record so we know their record # in advance
    for (int i=0; i<numClasses; i++)
        wrt.WriteInt32(i + DB_SCHEMA_ROOT_RECNO + 1);
    
    //save schema record in record #3
    REC_NO rootRecno = DB_SCHEMA_ROOT_RECNO;
    SQLiteData keySchema(&rootRecno, sizeof(REC_NO));
    
    SQLiteData data(wrt.GetData(), wrt.GetDataLen());
    
    if (m_db->put(0, &keySchema, &data, 0) != 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_20_SCHEMA_STORAGE_ERROR)));

    
    //build a temporary list of classes we are going to write
    //we need this so that we keep track of which base classes
    //have already been written when recursively saving 
    //child classes
    FdoPtr<FdoClassCollection> tempClasses = FdoClassCollection::Create(NULL);

    for (int i=0; i<numClasses; i++)
    {
        FdoPtr<FdoClassDefinition> def = classes->GetItem(i);
        tempClasses->Add(def);
    }


    //this will be set and then reused by the recursive WriteClassDefinition function
    //it is used to keep track of which record number to use next when recursively
    //writing out feature class definitions
    REC_NO currentRecno = rootRecno; /* = 3 */

    while (tempClasses->GetCount() > 0)
    {
        FdoPtr<FdoClassDefinition> clas0 = tempClasses->GetItem(0);
        WriteClassDefinition(currentRecno, clas0, tempClasses);
    }


    //set the schema member variable indirectly
    //by deserializing it from the database. This makes
    //sure the things like read only properties are handled correctly
    m_schema = ReadSchema();
}


void SchemaDb::WriteClassDefinition(REC_NO& recno, FdoClassDefinition* clas, FdoClassCollection* classes)
{
    FdoPtr<FdoClassDefinition> base = clas->GetBaseClass();

    if (base != NULL)
    {
        //see if we need to write the base class before writing the current class
        //recursively write the base class if it is still not written
        FdoPtr<FdoClassDefinition> baseToWrite = classes->FindItem (base->GetName());
        if (baseToWrite != NULL)
            WriteClassDefinition(recno, baseToWrite, classes);
    }

    // The following checks if any character in the class name is non-ASCII7.
    // The 3.0 and 3.1 file formats cannot properly deal with these characters so
    // the file version needs to be bumped up to 3.2 in this case.

    if ( m_majorVersion ==  SDFPROVIDER_VERSION_MAJOR_3 && 
         (m_minorVersion == SDFPROVIDER_VERSION_MINOR_3_0 || m_minorVersion == SDFPROVIDER_VERSION_MINOR_3_1)
    ) {
        FdoString* className = clas->GetName();
        size_t idx;
        size_t nameLen = wcslen(className);

        for ( idx = 0; idx < nameLen; idx++ ) 
        {
            if ( (className[idx] & 0x7f) != className[idx] ) 
            {
                // A non-ASCII7 character was found so bump up the file version to 3.2.
                // This causes the class table name to be the UTF8 version of the class name.
                // In 3.1 files, the table name is the MBCS version of the class name, 
                // which leads to problems since MBCS is locale-dependent.
                // For maximum forward-compatibility, the version is not bumped up if 
                // all class names only have ASCII-7 characters. This allows such a file
                // to be read by the SDF 3.1 Provider, even if created by the SDF 3.2 
                // provider. 
                m_minorVersion = SDFPROVIDER_VERSION_MINOR_3_2;
                WriteMetadata(m_majorVersion, m_minorVersion);
                break;
            }
        }
    }

    //increment record number -- the argument holds the record number of the last class that was
    //written to the database
    recno++;
    //remove class from temporary collection
    classes->Remove(clas);


    //TODO: for optimization, we can reuse the BinaryWriter from WriteSchema
    //if we pass it in as argument... it is not vital when writing a schema though
    BinaryWriter wrt(256);
	
	//write common stuff first
	unsigned int header = clas->GetClassType();
	if( clas->GetIsAbstract() )
		header = header | SDF_CLASSHEADER_ISABSTRACT;
    wrt.WriteUInt32( header );

    wrt.WriteString(clas->GetName());
    wrt.WriteString(clas->GetDescription());
    //write base class name
    if (base != NULL)
        wrt.WriteString(base->GetName());
    else
        wrt.WriteString(NULL);

    //now write class properties...
    FdoPtr<FdoPropertyDefinitionCollection> pdc = clas->GetProperties();

    //number of properties
    int numProps = pdc->GetCount();
    wrt.WriteInt32(numProps);

    for (int i=0; i<numProps; i++)
    {
        FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(i);
        
        FdoPropertyType type = pd->GetPropertyType();

        wrt.WriteInt32(type);

        switch (type)
        {
        case FdoPropertyType_DataProperty: WriteDataPropertyDefinition(wrt, (FdoDataPropertyDefinition*)(pd.p));
            break;
        case FdoPropertyType_GeometricProperty: WriteGeometricPropertyDefinition(wrt, (FdoGeometricPropertyDefinition*)(pd.p));
            break;
        case FdoPropertyType_ObjectProperty: WriteObjectPropertyDefinition(wrt, (FdoObjectPropertyDefinition*)(pd.p));
            break;
		case FdoPropertyType_AssociationProperty: WriteAssociationPropertyDefinition(wrt, (FdoAssociationPropertyDefinition*)(pd.p));
            break;
        default:
            throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_21_UNKNOWN_PROPERTY_TYPE)));

        }
    }

    //write count and names of identity properties, if any (if this is a derived class
    //it will have no id props, the collection will be empty)
    FdoPtr<FdoDataPropertyDefinitionCollection> idprops = clas->GetIdentityProperties();

    int numIdProps = idprops->GetCount();
    wrt.WriteInt32(numIdProps);

    for (int i=0; i<idprops->GetCount(); i++)
    {
        FdoPtr<FdoDataPropertyDefinition> dpd = idprops->GetItem(i);
        wrt.WriteString(dpd->GetName());
    }

    
    if (clas->GetClassType() == FdoClassType_FeatureClass)
    {
        FdoPtr<FdoGeometricPropertyDefinition> gp = ((FdoFeatureClass*)clas)->GetGeometryProperty();

        if (gp)
            wrt.WriteString(gp->GetName());
        else
            wrt.WriteString(NULL);
    }

    //insert class definition into database
    SQLiteData keyClass(&recno, sizeof(REC_NO));
    SQLiteData dataClass(wrt.GetData(), wrt.GetDataLen());

    if (m_db->put(0, &keyClass, &dataClass, SQLiteDB_APPEND) != 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_20_SCHEMA_STORAGE_ERROR)));

}


void SchemaDb::WriteDataPropertyDefinition(BinaryWriter& wrt, FdoDataPropertyDefinition* dpd)
{
    wrt.WriteString(dpd->GetName());
    wrt.WriteString(dpd->GetDescription());
    wrt.WriteInt32(dpd->GetDataType());
    wrt.WriteString(dpd->GetDefaultValue());
    wrt.WriteInt32(dpd->GetLength());
    wrt.WriteByte(dpd->GetNullable() ? 1 : 0);
    wrt.WriteInt32(dpd->GetPrecision());
    wrt.WriteByte(dpd->GetReadOnly() ? 1 : 0);
    wrt.WriteInt32(dpd->GetScale());
    wrt.WriteByte(dpd->GetIsAutoGenerated() ? 1 : 0);
    wrt.WriteByte(dpd->GetReadOnly() ? 1 : 0);

    //write property constraints:
    FdoPtr<FdoPropertyValueConstraint> valueConstraint = dpd->GetValueConstraint();
    wrt.WriteByte(valueConstraint != NULL ? 1 : 0);
    if (valueConstraint != NULL)
    {
        wrt.WriteByte(valueConstraint->GetConstraintType());
        switch (valueConstraint->GetConstraintType())
        {
            case FdoPropertyValueConstraintType_Range:
            {
                FdoPropertyValueConstraintRange* constraintRange = static_cast<FdoPropertyValueConstraintRange*>(valueConstraint.p);
                wrt.WriteByte(constraintRange->GetMaxInclusive() ? 1 : 0);
                wrt.WriteDataValue(FdoPtr<FdoDataValue>(constraintRange->GetMaxValue()));
                wrt.WriteByte(constraintRange->GetMinInclusive() ? 1 : 0);
                wrt.WriteDataValue(FdoPtr<FdoDataValue>(constraintRange->GetMinValue()));
            }
            break;

            case FdoPropertyValueConstraintType_List:
            {
                FdoPropertyValueConstraintList* constraintList = static_cast<FdoPropertyValueConstraintList*>(valueConstraint.p);
                FdoPtr<FdoDataValueCollection> dataValues = constraintList->GetConstraintList();
                wrt.WriteInt32(dataValues->GetCount());
                for (int i=0; i<dataValues->GetCount(); i++)
                {
                    FdoPtr<FdoDataValue> listItemValue = dataValues->GetItem(i);
                    wrt.WriteDataValue(listItemValue);
                }
            }
            break;
        }
    }

    //check if the type is right.
    //TODO: what is the state after we throw this?
    //There is part of a schem already written, and this can't be good...
    //We should allow resetting of the schema if such an error occurs.
    if (dpd->GetIsAutoGenerated() && (dpd->GetDataType() != FdoDataType_Int32))
    {
        throw FdoAutogenerationException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_23_AUTOGEN_INT_ONLY)));
    }
}

void SchemaDb::WriteGeometricPropertyDefinition(BinaryWriter& wrt, FdoGeometricPropertyDefinition* gpd)
{
    wrt.WriteString(gpd->GetName());
    wrt.WriteString(gpd->GetDescription());
    wrt.WriteInt32(gpd->GetGeometryTypes());
    wrt.WriteByte(gpd->GetHasElevation() ? 1 : 0);
    wrt.WriteByte(gpd->GetHasMeasure() ? 1 : 0);
    wrt.WriteByte(gpd->GetReadOnly() ? 1 : 0);
}

void SchemaDb::WriteObjectPropertyDefinition(BinaryWriter& wrt, FdoObjectPropertyDefinition* opd)
{
    throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_22_OBJECT_PROPERTY)));
}


void SchemaDb::ReadCoordinateSystemRecord(BinaryWriter& wrt)
{
    //retrieve root coord sys record -- it is stored in position 1 of the db
    REC_NO csRecno = DB_COORDSYS_RECNO;
    SQLiteData keyCs(&csRecno, sizeof(REC_NO));
    SQLiteData data;

    //this will fail if there is no coord sys stored in the database...
    if (m_db->get(0, &keyCs, &data, 0) != 0)
    {
        //Usually means that CS record is empty.
        //Return without storing anything in the given writer.
        //Caller should look for zero-length record
        return;
    }

    wrt.WriteBytes((unsigned char*)data.get_data(), data.get_size());
	m_db->close_cursor();
}

void SchemaDb::WriteCoordinateSystemRecord(BinaryWriter& wrt)
{
    //we will just overwrite the old coordinate system record, if any

    REC_NO csRecno = DB_COORDSYS_RECNO;
    SQLiteData keyCs(&csRecno, sizeof(REC_NO));
    SQLiteData data(wrt.GetData(), wrt.GetDataLen());

    if (m_db->put(0, &keyCs, &data, 0) != 0)
    {
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_24_COORDSYS_ERROR)));
    }
}


void SchemaDb::ReadMetadata(unsigned char& major, unsigned char& minor)
{
    //retrieve root coord sys record -- it is stored in position 1 of the db
    REC_NO vRecno = DB_METADATA_RECNO;
    SQLiteData keyCs(&vRecno, sizeof(REC_NO));
    SQLiteData data;

    //this will fail if there is no coord sys stored in the database...
    if (m_db->get(0, &keyCs, &data, 0) != 0)
    {
        //Record is empty.
        //Return without storing anything in the given writer.
        //Caller should look for zero version
        major = 0;
        minor = 0;
    }
    else
    {
        if (data.get_size() == 0)
        {
            major = 0;
            minor = 0;
        }
        else
        {
            BinaryReader rdr((unsigned char*)data.get_data(), data.get_size());
            major = rdr.ReadByte();
            minor = rdr.ReadByte();
        }
	    // This method(SchemaDb::ReadMetadata) gets call just opening the FdoIConnection.
	    // The following closes any open cursors so inserting features would work. 
	    CloseCursor();
    }
}

void SchemaDb::WriteMetadata(unsigned char major, unsigned char minor)
{
    BinaryWriter wrt(8);
    wrt.WriteByte(major);
    wrt.WriteByte(minor);

    //we will just overwrite the old record, if any 
    //but this should not happen anyway

    REC_NO vRecno = DB_METADATA_RECNO;
    SQLiteData keyCs(&vRecno, sizeof(REC_NO));
    SQLiteData data(wrt.GetData(), wrt.GetDataLen());

    if (m_db->put(0, &keyCs, &data, 0) != 0)
    {
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_25_METADATA_ERROR)));
    }

    m_majorVersion = major;
    m_minorVersion = minor;
}

void SchemaDb::CloseCursor()
{
	m_db->close_cursor();
}

void SchemaDb::WriteAssociationPropertyDefinition(BinaryWriter& wrt, FdoAssociationPropertyDefinition* apd)
{
	FdoPtr<FdoClassDefinition> cls = apd->GetAssociatedClass();
	if( cls.p == NULL )
		throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_61_ASSOCIATED_CLASS), apd->GetName() ));

    wrt.WriteByte( apd->GetIsReadOnly() ? 1 : 0);
    if( apd->GetIsReadOnly() ) // We do not save read only association as they are created from the owning association
        return;

    wrt.WriteString(apd->GetName());
    wrt.WriteString(apd->GetReverseName() );
	

    wrt.WriteString(cls->GetName() );
    wrt.WriteByte( (unsigned char )apd->GetDeleteRule() );
    wrt.WriteString( apd->GetMultiplicity() );
    wrt.WriteString( apd->GetReverseMultiplicity() );
    wrt.WriteByte( apd->GetLockCascade() ? 1 : 0);
	FdoPtr<FdoDataPropertyDefinitionCollection> properties = apd->GetIdentityProperties();
	int propCount = properties->GetCount();
	if( propCount != 0 )
	{
		wrt.WriteInt32( propCount );
		for(int i=0; i<propCount; i++ )
		{
			FdoPtr<FdoDataPropertyDefinition> prop = properties->GetItem(i);
			wrt.WriteString( prop->GetName() );
		}
		properties = apd->GetReverseIdentityProperties();
		if( properties.p != NULL )
		{
			propCount = properties->GetCount();
			wrt.WriteInt32( propCount );
			for(int i=0; i<propCount; i++ )
			{
				FdoPtr<FdoDataPropertyDefinition> prop = properties->GetItem(i);
				wrt.WriteString( prop->GetName() );
			}
		}
		else
			wrt.WriteInt32( 0 ); // No reverse identities
	}
	else
		wrt.WriteInt32( 0 ); // No identity properties
}

void SchemaDb::ReadAssociationPropertyDefinition(BinaryReader& rdr, FdoPropertyDefinitionCollection* pdc)
{
    if( rdr.ReadByte() ==1 ) // Read only association will be created from the owning association
        return;
	m_bHasAssociations = true;

    //create property
    FdoPtr<FdoAssociationPropertyDefinition> apd = FdoAssociationPropertyDefinition::Create();

    apd->SetName(rdr.ReadString());
    apd->SetReverseName( rdr.ReadString() );
	FdoPtr<FdoClass> cls = FdoClass::Create(rdr.ReadString(), L"dummy" ); // place holder for the Associated Class Name
    apd->SetAssociatedClass( cls );
	apd->SetDeleteRule((FdoDeleteRule)rdr.ReadByte() );
    apd->SetMultiplicity( rdr.ReadString() );
    apd->SetReverseMultiplicity( rdr.ReadString() );
    apd->SetLockCascade( (rdr.ReadByte()==1) );
	int propCount = rdr.ReadInt32();
	if( propCount != 0 )
	{
		// Read the identity properties. Since the data proeprties may not be loaded/create yet, we are going to
		// use place holders(holds the name) and fix it in the PostReadSchema.
		FdoPtr<FdoDataPropertyDefinitionCollection>properties = apd->GetIdentityProperties();
		for(int i=0; i<propCount; i++ )
		{
			FdoPtr<FdoDataPropertyDefinition>pd = FdoDataPropertyDefinition::Create( rdr.ReadString() ,L"");
			properties->Add(pd);
		}
		// Read the reverse identity propertis if they exist
		int propCount = rdr.ReadInt32(); 
		if( propCount != 0 )
		{
			properties = apd->GetReverseIdentityProperties();
			for(int i=0; i<propCount; i++ )
			{
				FdoPtr<FdoDataPropertyDefinition>pd = FdoDataPropertyDefinition::Create( rdr.ReadString() ,L"");
				properties->Add(pd);
			}
		}
	}

    //add property to collection
    pdc->Add(apd);
}

void SchemaDb::PostReadSchema( FdoFeatureSchema* schema )
{
	// If the schema contains any association properties, then we need to reset the references of the various objects:
	// 1- Reset the associated class of each association; Replace the place holder class that was	used when the association property was read.
	// 2- Reset the identity and reverse identity properties. Replace the place holder properties with the real properties.
	if(	! m_bHasAssociations )
		return;
	
	FdoPtr<FdoClassCollection> classes = schema->GetClasses();
	for	(int i=0; i<classes->GetCount(); i++)
	{
		FdoPtr<FdoClassDefinition> cls = classes->GetItem(i);
		FdoPtr<FdoPropertyDefinitionCollection>	pdc	= cls->GetProperties();
		for	(int j=0; j<pdc->GetCount(); j++)
		{
			FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(j);
			if(	pd->GetPropertyType() == FdoPropertyType_AssociationProperty )
			{
				FdoAssociationPropertyDefinition* apd =	(FdoAssociationPropertyDefinition*)(pd.p);
				FdoPtr<FdoClassDefinition> acls	= classes->FindItem( FdoPtr<FdoClassDefinition>(apd->GetAssociatedClass())->GetName() );
				if(	acls !=	NULL )
					apd->SetAssociatedClass( acls );
				else //	Should not happen; the schema is probably corrupt
					throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_59_ASSOCIATED_CLASS), pd->GetName() ));

				//
				// Set the identity and reverse identities references
				FdoPtr<FdoDataPropertyDefinitionCollection>	properties = apd->GetReverseIdentityProperties();
				if(	properties.p !=	NULL )
				{
					int	propCount =	properties->GetCount();
					for(int	i=0; i<propCount; i++ )
					{
						FdoPtr<FdoDataPropertyDefinition> prop = properties->GetItem(i);
						FdoPtr<FdoPropertyDefinition> new_prop = pdc->FindItem( prop->GetName() );
						if( new_prop.p == NULL ) // Try the base properties
						{
							try // FdoReadOnlyPropertyDefinitionCollection does not have a FindItem
							{
								new_prop = FdoPtr<FdoReadOnlyPropertyDefinitionCollection>(cls->GetBaseProperties())->GetItem( prop->GetName() );
							}
							catch(FdoException *e)
							{
								// Ignore as a new exception will be thrown 
								e->Release();
							}
						}
						if( new_prop.p == NULL || new_prop->GetPropertyType() != FdoPropertyType_DataProperty )
							throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_60_ASSOCIATION_IDENTITY), pd->GetName() ));
						
						properties->Remove( prop );
						properties->Add( (FdoDataPropertyDefinition*)new_prop.p );
					}
					properties = apd->GetIdentityProperties(); 
					if(	properties.p !=	NULL )
					{
						pdc	= acls->GetProperties();  // Get the collection from the associated class
						int	propCount =	properties->GetCount();
						for(int	i=0; i<propCount; i++ )
						{
							FdoPtr<FdoDataPropertyDefinition> prop = properties->GetItem(i);
							FdoPtr<FdoPropertyDefinition> new_prop = pdc->FindItem( prop->GetName() );
							if( new_prop.p == NULL ) // Try the base properties
							{
								try // FdoReadOnlyPropertyDefinitionCollection does not have a FindItem
								{   
									new_prop = FdoPtr<FdoReadOnlyPropertyDefinitionCollection>(acls->GetBaseProperties())->GetItem( prop->GetName() );
								}
								catch(FdoException *e)
								{
									// Ignore as a new exception will be thrown 
									e->Release();
								}
							}
							if( new_prop.p == NULL || new_prop->GetPropertyType() != FdoPropertyType_DataProperty )
								throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_60_ASSOCIATION_IDENTITY), pd->GetName() ));
							
							properties->Remove( prop );
							properties->Add( (FdoDataPropertyDefinition*)new_prop.p );
						}
					}
				}
			}
		}
	}
}
