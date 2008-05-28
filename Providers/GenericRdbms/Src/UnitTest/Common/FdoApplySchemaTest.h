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

#ifndef CPP_UNIT_FDOAPPLYSCHEMATEST_H
#define CPP_UNIT_FDOAPPLYSCHEMATEST_H

#include <Sm/SchemaManager.h>
#include <Rdbms/Override/RdbmsOvPhysicalSchemaMapping.h>

/* 
 * A test case for ApplySchemaCommand.
 *
 * Loads some schemas into a database.
 */

class StaticConnection;

class FdoApplySchemaTest : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( FdoApplySchemaTest );
  CPPUNIT_TEST( TestSchema );
  CPPUNIT_TEST( TestOverrides );
  CPPUNIT_TEST( TestOverrideDefaults );
  CPPUNIT_TEST( TestOverrideErrors );
  CPPUNIT_TEST( TestLT );
  CPPUNIT_TEST( TestConfigDoc );
  CPPUNIT_TEST( TestNoMeta );
  CPPUNIT_TEST_SUITE_END();

public:
    FdoApplySchemaTest(void);
    virtual ~FdoApplySchemaTest(void);
    void setUp ();

protected:
    virtual void  set_provider() {};   
	static FdoPropertyValue* AddNewProperty( FdoPropertyValueCollection* propertyValues, const wchar_t *name );

    virtual void TestSchema ();
    virtual void TestOverrides ();
    virtual void TestOverrideDefaults ();
    virtual void TestOverrideErrors ();
    virtual void TestLT();
    virtual void TestConfigDoc();
    virtual void TestNoMeta();

    void ApplyNoMetaSuccess( FdoIConnection* connection, StaticConnection* staticConn );
    void ApplyNoMetaFailure( FdoIConnection* connection, StaticConnection* staticConn );

    void DeleteAcadSchema( FdoIConnection* connection );
	void DeleteLandSchema( FdoIConnection* connection );
	void DeleteDefaultSchema( FdoIConnection* connection, bool update );
	void CreateSystemSchema( FdoIConnection* connection );
	void CreateAcadSchema( FdoIConnection* connection, bool hasMetaSchema = true, bool addSAD = false );
	void CreateElectricSchema( FdoIConnection* connection, bool hasMetaSchema = true );
	void CreateLandSchema( FdoIConnection* connection, bool hasMetaSchema = true );
    void CreateLandSchema( FdoFeatureSchemaCollection* pSchemas, bool hasMetaSchema = true );
    void CreateLTSchema( FdoIConnection* connection );
	void CreateErrorSchema( FdoIConnection* connection );
	virtual void CreateNLSSchema( FdoIConnection* connection, StaticConnection* staticConn, bool hasMetaSchema = true );

	void CreateLongStringSchema( FdoIConnection* connection, bool hasMetaSchema = true  );
	void CreateOverrideSchema( FdoIConnection* connection, FdoRdbmsOvPhysicalSchemaMapping* pOverrides = NULL, bool nnull = false, bool addConstraints = true, bool hasMetaSchema = true );
    void CreateForeignBasedSchema( FdoIConnection* connection, FdoFeatureSchema* pBaseSchema, FdoRdbmsOvPhysicalSchemaMapping* pOverrides = NULL );
/*
    void CreateForeignErrorSchema( FdoIConnection* connection );
*/
	void ModOverrideSchema1( FdoIConnection* connection, FdoRdbmsOvPhysicalSchemaMapping* pOverrides = NULL );
	void ModOverrideSchema2( FdoIConnection* connection, FdoRdbmsOvPhysicalSchemaMapping* pOverrides = NULL );
	void ModOverrideSchemaForeign( FdoFeatureSchema* pSchema, FdoRdbmsOvPhysicalSchemaMapping* pOverrides = NULL );
	void ModOverrideSchemaForeign2( FdoIConnection* connection, FdoRdbmsOvPhysicalSchemaMapping* pOverrides = NULL );
	void ModElectricSchema( FdoIConnection* connection, bool hasMetaSchema = true  );
    void ModElectricSchema( FdoFeatureSchemaCollection* pSchemas, bool hasMetaSchema = true  );
	void ModLandSchema( FdoIConnection* connection, bool hasMetaSchema = true );
	void ModLTSchema( FdoIConnection* connection );
	void RedefineGeometry( FdoIConnection* connection );
	void DelPropertyError( FdoIConnection* connection );
	void ModDelSchemas( FdoIConnection* connection, bool hasMetaSchema = true );
    void ModDelElectricSchema( FdoFeatureSchemaCollection* pSchemas, bool hasMetaSchema = true );
    void ModDelAcadSchema( FdoFeatureSchemaCollection* pSchemas, bool hasMetaSchema = true );
	void ReAddElements( FdoIConnection* connection, bool hasMetaSchema = true );

	void ModErrors( FdoIConnection* connection );
	void ModErrors2( FdoIConnection* connection );

	void ModMetaClassSchema( FdoIConnection* connection );

/*
	void GetJoinTree( FdoRdbmsSchemaManager* sm );
*/
    virtual void GetClassCapabilities( FdoIConnection* connection );
    void CheckBaseProperties( FdoIConnection* connection );
    void CheckNonNumericForeignClass( FdoIConnection* connection );

    void CopySchemas( 
        FdoFeatureSchemaCollection* pSchemas,
        FdoFeatureSchemaCollection* pSchemas2
    );

    void CreateOverrideValueProperty( 
        FdoClassDefinition* pClass,
        FdoStringP suffix,
        FdoInt32 position,
        FdoInt32 depth
    );

    void CreateOverrideCollectionProperty( 
        FdoClassDefinition* pClass,
        FdoStringP suffix,
        FdoInt32 position,
        FdoInt32 depth
    );

    FdoRdbmsOvPhysicalSchemaMapping* CreateOverrides( FdoIConnection* connection, int passNum );
   	virtual FdoRdbmsOvPhysicalSchemaMapping* CreateOverrideDefaults( FdoIConnection* connection, int passNum );
    FdoRdbmsOvPhysicalSchemaMapping* CreateErrorOverrides( FdoIConnection* connection );
   	FdoRdbmsOvPhysicalSchemaMapping* CreateForeignBasedOverrides( FdoIConnection* connection );

    void CreateOverrideValueProperty( 
        FdoRdbmsOvPhysicalSchemaMapping* pOverrides,
        FdoRdbmsOvClassDefinition* pClass,
        FdoStringP suffix,
        FdoInt32 position,
        FdoInt32 depth
    );

    void CreateOverrideCollectionProperty( 
        FdoRdbmsOvPhysicalSchemaMapping* pOverrides,
        FdoRdbmsOvClassDefinition* pClass,
        FdoStringP suffix,
        FdoInt32 position,
        FdoInt32 depth
    );

    // Helper methods for overrides tests; need to be overriden for each provider:
    virtual void CreateRdbmsSpecificElements(FdoIConnection* connection, FdoString* wDatastore) { };
    virtual FdoRdbmsOvClassDefinition* CreateOvClassDefinition(FdoString *name = NULL);
    virtual void ClassesOvAdd(FdoRdbmsOvPhysicalSchemaMapping* pSchema, FdoRdbmsOvClassDefinition* pClass);
    virtual FdoRdbmsOvTable* CreateOvTable(FdoString* name);
    virtual FdoRdbmsOvColumn* CreateOvColumn(FdoString* name);
    virtual FdoRdbmsOvGeometricColumn* CreateOvGeometricColumn(FdoString* name);
    virtual FdoRdbmsOvDataPropertyDefinition* CreateOvDataPropertyDefinition(FdoString *name);
    virtual FdoRdbmsOvGeometricPropertyDefinition* CreateOvGeometricPropertyDefinition(FdoString *name);
    virtual void PropertiesOvAdd(FdoRdbmsOvClassDefinition* pClass, FdoRdbmsOvPropertyDefinition *pProp);
    virtual void ClassOvSetTable(FdoRdbmsOvClassDefinition* pClass, FdoRdbmsOvTable* pTable);
    virtual void TableOvSetTablespace(FdoRdbmsOvTable* pTable, FdoString *tablespace);
    virtual void DataPropOvSetColumn(FdoRdbmsOvDataPropertyDefinition* pDataProp, FdoRdbmsOvColumn* pDataColumn);
    virtual void GeometricPropOvSetColumn(FdoRdbmsOvGeometricPropertyDefinition* pGeomProp, FdoRdbmsOvGeometricColumn* pGeometricColumn);
    virtual void ColumnOvSetSequence(FdoRdbmsOvColumn* pColumn, FdoString *sequence);
    virtual FdoRdbmsOvObjectPropertyDefinition* CreateOvObjectPropertyDefinition(FdoString *name);
    virtual FdoRdbmsOvPropertyMappingSingle* CreateOvPropertyMappingSingle();
    virtual FdoRdbmsOvPropertyMappingConcrete* CreateOvPropertyMappingConcrete();
    virtual void ObjectPropertyOvSetMappingDefinition(FdoRdbmsOvObjectPropertyDefinition* pObProp, FdoRdbmsOvPropertyMappingDefinition* mapping);
    virtual void PropertyMappingOvSetInternalClass(FdoRdbmsOvPropertyMappingRelation* pMapping, FdoRdbmsOvClassDefinition* pClass);
    virtual void SchemaOvSetOwner(FdoRdbmsOvPhysicalSchemaMapping *mapping, FdoString* owner);

    virtual void VldClassCapabilities( int ltMode, int lckMode, FdoClassDefinition* pClass );

    virtual int GetLtLockMethod( FdoSmPhMgrP mgr )
    {
        return 0;
    }

    virtual bool CanApplyWithoutMetaSchema();

    virtual bool CanAddNotNullCol();

    virtual bool CanDropCol();

    virtual FdoStringP SchemaTestErrFile( int fileNum, bool isMaster );

    virtual FdoStringP SchemaNoMetaErrFile( int fileNum, bool isMaster );

    virtual FdoStringP SchemaOvErrFile( int fileNum, bool isMaster );

    virtual bool CompareGeometrySI();

    virtual bool CreateGeometrySICol();

    virtual FdoStringP GetValueColumnName();

    void WriteXmlOverrides(
        FdoIConnection* connection,
        bool includeDefaults,
        FdoString* schemaName,
        FdoString* fileName,
        FdoString* ownerSuffix = NULL
    );

    FdoStringP GenLtName( FdoStringP transName );
    void StartLongTransaction( FdoIConnection* connection, FdoStringP transName );
    void EndLongTransaction( FdoIConnection* connection );
    void RollbackLongTransaction( FdoIConnection* connection, FdoStringP transName );
    FdoStringP GetActiveLongTransaction( FdoIConnection* connection );

    void InsertObject( FdoIConnection* connection, bool conditional, FdoStringP schemaName, FdoString* className, ... );
    void DeleteObjects( FdoIConnection* connection, FdoStringP schemaName, FdoStringP className );

    void _logicalPhysicalBend( FdoString* inFile, FdoString* outFile, FdoStringP providerName );
    void _logicalPhysicalFormat( FdoString* inFile, FdoString* outFile, FdoStringP providerName );
    virtual FdoStringP LogicalPhysicalBend( FdoString* inFile );
    virtual FdoStringP LogicalPhysicalFormat( FdoString* inFile );

    FdoPtr<FdoFeatureSchema> GetDefaultSchema( FdoIConnection* connection );

    FdoStringP mDatastore;
	bool mIsLowerDatastoreName;

    static FdoString*      DB_NAME_SUFFIX;
    static FdoString*      DB_NAME_COPY_SUFFIX;
    static FdoString*      DB_NAME_OVERRIDE_SUFFIX;
    static FdoString*      DB_NAME_OVERRIDE_DEFAULT_SUFFIX;
    static FdoString*      DB_NAME_FOREIGN_SUFFIX;
    static FdoString*      DB_NAME_CONFIG_SUFFIX;
    static FdoString*      DB_NAME_NO_META_SUFFIX;

    static FdoString*      LT_NAME;
    static FdoString*      DB_NAME_LT_SUFFIX;

};

#endif // CPP_UNIT_FDOAPPLYSCHEMATEST_H
