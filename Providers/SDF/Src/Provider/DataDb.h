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
#include "BinaryWriter.h"


class PropertyIndex;


class DataDb
{
public:

    DataDb(SQLiteDataBase* env, const char* filename, const char* dbname, bool bReadOnly);
    virtual ~DataDb();
    
    REC_NO InsertFeature(FdoClassDefinition* fc, PropertyIndex* pi, FdoPropertyValueCollection* pvc);
    void DeleteFeature(REC_NO recno);
    void UpdateFeature(REC_NO recno, SQLiteData* data);
    
    inline int GetFeature(SQLiteData* key, SQLiteData* data)
    {
        return m_db->get(0, key, data, 0);
    }

	int GetFirstFeature( SQLiteData* key, SQLiteData* data );
    
	int GetNextFeature( SQLiteData* key, SQLiteData* data );

    int Cursor(SQLiteCursor** cursor, bool write);

    void CloseCursor();

	bool CurrentDataIsValid();

	bool NeedsAFlush() { return m_db->needs_a_flush(); }

	void Flush() { m_db->flush(); }

	// The DataDb can be shared by many reader. The tag is used to identify the
	// last reader that used the DataDb.
	void SetTag( void* tag );

	void* GetTag();

	void SyncIdPool() { m_db->sync_id_pool(); }

private:

    SQLiteTable* m_db;

	void* m_tag;
	
	REC_NO m_lastRec;

    BinaryWriter m_wrtData;
};



