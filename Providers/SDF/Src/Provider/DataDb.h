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
#ifndef SDF_DATADB_H
#define SDF_DATADB_H

#include "BinaryWriter.h"
#include "SDF/SdfCompareHandler.h"
#include "BinaryReader.h"
#include "PhysName.h"

class PropertyIndex;


class DataDb: public SQLiteBTreeCompareHandler
{
public:

    DataDb(SQLiteDataBase* env, const char* filename, FdoString* dbname, bool bIsUTF8, bool bReadOnly, 
		FdoClassDefinition* fc, PropertyIndex* pi, SdfCompareHandler* CmpHandler);
    virtual ~DataDb();
    
    REC_NO InsertFeature(FdoClassDefinition* fc, PropertyIndex* pi, FdoPropertyValueCollection* pvc);

	REC_NO InsertFeatureExclusive(FdoClassDefinition* fc, PropertyIndex* pi, FdoIFeatureReader* reader, FdoPropertyValueCollection* defaultPvc);

    void DeleteFeature(REC_NO recno);

    void UpdateFeature(REC_NO recno, SQLiteData* data);
    
    inline int GetFeature(SQLiteData* key, SQLiteData* data)
    {
        return m_db->get(0, key, data, 0);
    }

	int GetFirstFeature( SQLiteData* key, SQLiteData* data );
    
	int GetNextFeature( SQLiteData* key, SQLiteData* data );

	int GetLastFeature( SQLiteData* key, SQLiteData* data );

	int GetPreviousFeature( SQLiteData* key, SQLiteData* data );

	int FindFeatureAt(SQLiteData* key, SQLiteData* data, FdoPropertyValueCollection* pvc);

	int GetFeatureAt(SQLiteData* key, SQLiteData* data, FdoPropertyValueCollection* pvc);

	int GetFeatureAt( SQLiteData* key, SQLiteData* data );

    int Cursor(SQLiteCursor** cursor, bool write);

    void CloseCursor();

	bool CurrentDataIsValid();

	bool NeedsAFlush(bool partial=false) { return m_db->needs_a_flush(partial); }

	void Flush() { m_db->flush(); }

	// The DataDb can be shared by many reader. The tag is used to identify the
	// last reader that used the DataDb.
	void SetTag( void* tag );

	void* GetTag();

	void SyncIdPool() { m_db->sync_id_pool(); }

	virtual int compare(int,const void*,int,const void*);

	void SetOrderingOptions( std::map<std::wstring, int> *opts );

    void Drop();

    FdoString* GetDbName();

private: 

	int compare(int size1, const void* data1, FdoPropertyValueCollection* pvc);

private:

    PhysName m_dbname;

    SQLiteTable* m_db;

	void* m_tag;
	
	REC_NO m_lastRec;

    BinaryWriter m_wrtData;

	FdoClassDefinition* m_Fc;
	
	PropertyIndex* m_Pi;

	SdfCompareHandler* m_CompareHandler;

	int *m_orderingOptions;

	FdoPtr<FdoDataPropertyDefinitionCollection> m_Ids;

	BinaryReader  m_Reader1;
	BinaryReader  m_Reader2;
	int*		  m_Offsets1;
	int*		  m_Offsets2;
};

#endif

