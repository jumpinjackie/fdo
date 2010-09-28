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
#ifdef WIN32
#include <hash_map>
#else
#include <ext/hash_map>
#endif

#include "BinaryReader.h"
#include "BinaryWriter.h"
#include "SdfSchemaMergeContext.h"


class SchemaDb 
{
public:

    SchemaDb(SQLiteDataBase* env, const char* filename, bool bReadOnly, bool bCreate);
    virtual ~SchemaDb();

    // Retrieves the version of the current SDF file. 
    void GetSchemaVersion(unsigned char& major, unsigned char& minor);

    //FDO feature schema support
    void SetSchema(SdfISchemaMergeContextFactory* mergeFactory, FdoFeatureSchema* schema, bool ignoreStates);

    //returns pointer to cached schema object -- for internal use
    FdoFeatureSchema* GetSchema(FdoString *schemaName = NULL);    

    //returns a brand new copy of the schema -- for use when returning
    //to external caller ot resetting the local schema object
    FdoFeatureSchema* ReadSchema(FdoString *schemaName = NULL);

    //Coordinate system support
    void ReadCoordinateSystemRecord(BinaryWriter& wrt);
    void WriteCoordinateSystemRecord(BinaryWriter& wrt);

    //Other metadata support -- add here
    void ReadMetadata(unsigned char& major, unsigned char& minor);
    void WriteMetadata(unsigned char major, unsigned char minor);

    void FixPropertiesOrder( FdoFeatureSchema* schema, FdoFeatureSchema* refschema  );

	void CloseCursor();

private:
    // Update the old schemas based on the new Schema and ignoreStates setting.
    SdfSchemaMergeContextP MergeSchema(SdfISchemaMergeContextFactory* mergeFactory, FdoFeatureSchemaP oldSchema, FdoFeatureSchemaP newSchema, bool ignoreStates);
    
    // Handles class data updates that must be done before schema updates
    void PreAcceptChanges( SdfSchemaMergeContextP mergeContext );
    void PostAcceptChanges( SdfSchemaMergeContextP mergeContext );
    // Handles class data updates that must be done after schema updates
    void PostUpdatePhysical( SdfSchemaMergeContextP mergeContext );

    // Rolls back changes made by PreUpdatePhysical2
    void RollbackPhysical( SdfSchemaMergeContextP mergeContext );

    void ReadFeatureClass(REC_NO classRecno, FdoFeatureSchema* schema);
    void ReadGeometricPropertyDefinition(BinaryReader& rdr, FdoPropertyDefinitionCollection* pdc);
    void ReadDataPropertyDefinition(BinaryReader& rdr, FdoPropertyDefinitionCollection* pdc);
    void ReadObjectPropertyDefinition(BinaryReader& rdr, FdoPropertyDefinitionCollection* pdc);
	void ReadAssociationPropertyDefinition(BinaryReader& rdr, FdoPropertyDefinitionCollection* pdc);
    FdoDataValue* ReadDataValue(BinaryReader& rdr);
      
	void PostReadSchema( FdoFeatureSchema* schema );

    

    void WriteSchema(FdoFeatureSchema* schema);
    void WriteClassDefinition(REC_NO& recno, FdoClassDefinition* clas, FdoClassCollection* classes);
    void WriteDataPropertyDefinition(BinaryWriter& wrt, FdoDataPropertyDefinition* dpd);
    void WriteGeometricPropertyDefinition(BinaryWriter& wrt, FdoGeometricPropertyDefinition* dpd);
    void WriteObjectPropertyDefinition(BinaryWriter& wrt, FdoObjectPropertyDefinition* dpd);
	void WriteAssociationPropertyDefinition(BinaryWriter& wrt, FdoAssociationPropertyDefinition* apd);
    void WriteDataValue(BinaryWriter& wrt, FdoDataValue* dataValue);

private:
    SQLiteTable* m_db;
    SQLiteDataBase* m_env;
	bool  m_bHasAssociations;
    FdoFeatureSchema* m_schema;
    unsigned char m_majorVersion;
    unsigned char m_minorVersion;
	wchar_t* m_scName;
    bool m_bReadOnly;
};

